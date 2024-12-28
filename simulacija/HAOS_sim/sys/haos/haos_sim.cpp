/*
 * haos_sim.c
 *
 *  Created on: Apr 10, 2024
 *      Author: pekez
 */

#include "bitripper_sim.h"
#include "haos.h"
#include "wavefile.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <xtensa/tie/xt_hifi4.h>

#define NO_SOURCE (0x08000000)

enum HAOS_ROUTINE {
  PREKICK,
  POSTKICK,
  TIMER,
  FRAME,
  BRICK,
  AFAP,
  BACKGROUND,
  POSTMALLOC,
  PREMALLOC
};

// one block of audio data
typedef ae_f32 HAOS_BrickBuffer_t[BRICK_SIZE];

// type used to represent a sequence of incoming host comm messages
typedef std::list<unsigned int> HAOS_HostCommQ_t;

typedef std::list<HAOS_OdtEntry_t> HAOS_ModuleTable_t;

// the global IOBUFFER which is accessed through the public IOBUFFER_PTR array
HAOS_BrickBuffer_t IOBUFFER[NUMBER_OF_IO_CHANNELS][IO_BUFFER_PER_CHAN_MODULO];

// the global TEMPBUFFER which is accessed through the public TEMPBUFFER_PTR
// array
HAOS_BrickBuffer_t TEMPBUFFER[NUMBER_OF_IO_CHANNELS];

// contains all of the module MIFs and IDs to support host-comm, and to
// call the module entry points
HAOS_ModuleTable_t masterModuleTable;

extern HAOS_OdtEntry_t *PcmDecoder_odtPtr;
extern HAOS_OdtEntry_t *Mp3Decoder_odtPtr;
extern HAOS_OdtEntry_t *AudioManager_odtPtr;
extern int *HAOS_fillInputFIFO0(int *writePtr);

//====================== functions internal to the library
//======================
static void callAllModules(HAOS_ROUTINE entryPoint); // forward
static void readPrekickConfigs();                    // forward
// static void doMallocs();                                // forward
static void initVariables();                            // forward
static bool openFile();                                 // forward
static void writeToFile();                              // forward
static bool flushFrameToFile();                         // forward
static void updatePtrs();                               // forward
static void copyFrameData(HAOS_FrameData_t *frameData); // forward
// static int decodeSample(FILE *rdFile);

// file I/O parameters
std::string outputPath;
std::string inputPath;
WAVWRITE_HANDLE *inputHandle;
WAVWRITE_HANDLE *outputHandle;

bool fillFIFO = false; // to check if FIFO0 is empty
bool firstOpen = true; // to check if input file is already opened for reading
int inputFIFO0[FIFO0_SIZE] = {0}; // FIFO0 buffer - read input samples
int bitsPerSample = 0;
int samplesPerChannel = 0;
int sampleRate = 0;
int runCycles = 0;
int nOutChannels = 0;
int nInputChannels = 0;
static int outputSampleRate = 48000; // init value for sample rate
static std::string cfgPath;          // prekick .cfg filename
static int fg2bg_ratio = 16;         // 16 for PCM decoder, 72 for MP3 decoder
static int validChMask = 0;

//------------------------------- public variables
//------------------------------------------
HAOS_FrameData_t HAOS_frameData = {
    1,     // inputChannelMask
    1,     // outputChannelMask
    48000, // sampleRate
    1      // decodeInfo
};

HAOS_PcmSamplePtr_t __HAOS_IOBUFFER_INP_PTRS[NUMBER_OF_IO_CHANNELS] = {
    IOBUFFER[0][0],  IOBUFFER[1][0],  IOBUFFER[2][0],
    IOBUFFER[3][0],  IOBUFFER[4][0],  IOBUFFER[5][0],
    IOBUFFER[6][0],  IOBUFFER[7][0],  IOBUFFER[8][0],
    IOBUFFER[9][0],  IOBUFFER[10][0], IOBUFFER[11][0],
    IOBUFFER[12][0], IOBUFFER[13][0], IOBUFFER[14][0],
    IOBUFFER[15][0]

};

HAOS_PcmSamplePtr_t __HAOS_IOBUFFER_PTRS[NUMBER_OF_IO_CHANNELS] = {

    IOBUFFER[0][0],  IOBUFFER[1][0],  IOBUFFER[2][0],  IOBUFFER[3][0],
    IOBUFFER[4][0],  IOBUFFER[5][0],  IOBUFFER[6][0],  IOBUFFER[7][0],
    IOBUFFER[8][0],  IOBUFFER[9][0],  IOBUFFER[10][0], IOBUFFER[11][0],
    IOBUFFER[12][0], IOBUFFER[13][0], IOBUFFER[14][0], IOBUFFER[15][0]};

const HAOS_PcmSamplePtr_t __HAOS_TEMPBUFFER_PTRS[NUMBER_OF_IO_CHANNELS] = {
    TEMPBUFFER[0],  TEMPBUFFER[1],  TEMPBUFFER[2],  TEMPBUFFER[3],
    TEMPBUFFER[4],  TEMPBUFFER[5],  TEMPBUFFER[6],  TEMPBUFFER[7],
    TEMPBUFFER[8],  TEMPBUFFER[9],  TEMPBUFFER[10], TEMPBUFFER[11],
    TEMPBUFFER[12], TEMPBUFFER[13], TEMPBUFFER[14], TEMPBUFFER[15]};

int __HAOS_PPM_VALID_CHANNELS = 0;
int __HAOS_PPM_INPUT_FS = 0;
bool __HAOS_AT_EOF = false;
int __HAOS_IO_FREE = IO_BUFFER_SIZE_PER_CHAN;

//============================= public functions ==============================
void *HAOS_getMcvPointer(int moduleID) {
  HAOS_ModuleTable_t::const_iterator mb, me;

  for (mb = masterModuleTable.begin(), me = masterModuleTable.end(); mb != me;
       ++mb) {
    if (mb->moduleID == moduleID) {
      return mb->MIF->MCV;
    }
  }

  return 0;
}
//==============================================================================

//====================== functions internal to the library =====================
// run() is the main loop of the simulated OS, it does the scheduling the same
// as the Coyote OS.
static void run() {
  int m_framesPerMs = FRAMES_PER_MS;
  long long m_totalBricksRun = 0;

  // BitRipper structure initialization
  BitRipper_init(currStateStruct);

  // prefill IOBUFFER and do any other variable initialization
  initVariables();

  // execute prekick() entry points
  callAllModules(PREKICK);
  // mcv se setuje na default vrendosti, posle toga readPrekickConfigs setuje na
  // one koje su u cfg fajlu
  //  read configuration file, if any
  readPrekickConfigs();
  

  // execute postkick() entry points
  callAllModules(POSTKICK);

  // execute timer() entry points first time after postKick()
  callAllModules(TIMER);

#ifdef __HAOS_RUN_EOF
  openFile();

  // runCycles = ceil((samplesPerChannel * 1.0) / BRICK_SIZE / fg2bg_ratio);
  runCycles = ceil((bitsPerSample * 1.0) / BRICK_SIZE / fg2bg_ratio);
  for (int i = 0; i < runCycles; i++) {
    for (int brick = fg2bg_ratio; brick > 0; brick--) {
      if ((m_totalBricksRun % fg2bg_ratio) == 0) {
        // call the frame function
        callAllModules(FRAME);

        if (--m_framesPerMs == 0) {
          // call the timer function (assume timer set to 1ms)
          callAllModules(TIMER);
          m_framesPerMs = FRAMES_PER_MS;
        }
      }

      // call brick function
      callAllModules(BRICK);
      m_totalBricksRun++;

      // write to output file
      writeToFile();
    }

    // call the background function
    callAllModules(BACKGROUND);

    // do this so that during the simulation the file can be opened manually
    flushFrameToFile();

    // update IO buffer pointers
    updatePtrs();
  }

  cl_wavwrite_close(outputHandle);

#else
  openFile();
  while (!__HAOS_AT_EOF) {
    for (int brick = fg2bg_ratio; brick > 0; brick--) {
      if ((m_totalBricksRun % fg2bg_ratio) == 0) {
        // call the frame function
        callAllModules(FRAME);

        if (--m_framesPerMs == 0) {
          // call the timer function (assume timer set to 1ms)
          callAllModules(TIMER);
          m_framesPerMs = FRAMES_PER_MS;
        }
      }

      // call brick function
      if (!__HAOS_AT_EOF) {
        for (int i = 0; i < IO_BUFFER_PER_CHAN_MODULO; i++) {
          __HAOS_PPM_VALID_CHANNELS = HAOS_frameData.outputChannelMask;
          callAllModules(BRICK);
          m_totalBricksRun++;

          // write to output file
          writeToFile();

          // update IO buffer pointers
          updatePtrs();
        }
      }
    }

    // call the background function
    callAllModules(BACKGROUND);

    // do this so that during the simulation the file can be opened manually
    flushFrameToFile();
  }

#endif
}

static void usage(const char *programName) {
  std::cout << "usage: " << programName << " [options]" << std::endl
            << "  where options are:" << std::endl
            << "    -fg2bg <ratio of brick to background entry point calls> - "
               "default is 16"
            << std::endl
            << "    -cfg <cfg file pathname> : pathname of host comm messages "
               "to send prekick"
            << std::endl
            << "    -input <input audio WAV file pathname> : pathname of audio "
               "file to use as input"
            << std::endl
            << "           WAV file channels are mapped to IOBUFFER channels "
               "based on the CS498XX PCM decoder MCV"
            << std::endl
            << "           settings for index 0x0001 through 0x0010. Those "
               "default to mapping each WAV channel"
            << std::endl
            << "           to the next IOBUFFER channel, in ascending order "
               "from 0 to 15"
            << std::endl
            << "    -output <output audio WAV file pathname> : pathname of "
               "audio file to write as output"
            << std::endl
            << "           Channels in the WAV file are mapped from the "
               "IOBUFFER based on the valid channel mask"
            << std::endl
            << "           OS variable. The first valid channel is written to "
               "WAV channel 0, the 2nd valid channel"
            << std::endl
            << "           is written to WAV channel 1, etc." << std::endl
            << "    -osamplesize <output sample size in bits> - default is 16"
            << std::endl
            << "    -ofs <output sample rate> - default is 48000" << std::endl;
}
//==============================================================================

static void HAOS_parseCmdLine(int argc, const char *argv[]) {
  std::string programName = argv[0];
  programName.erase(0, programName.find_last_of("\\/") + 1);
  std::string arg;

  for (int i = 1; i < argc;) {
    arg = argv[i++];

    if (arg.find("--help") == 0) {
      usage(programName.c_str());
      exit(0);
    } else if (arg.find("-fg2bg") == 0) {
      if (i < argc) {
        std::istringstream is(argv[i++]);
        is >> fg2bg_ratio;
      } else {
        usage(programName.c_str());
        exit(1);
      }
    } else if (arg.find("-cfg") == 0) {
      if (i < argc) {
        cfgPath = argv[i++];
      } else {
        usage(programName.c_str());
        exit(1);
      }
    } else if (arg.find("-input") == 0) {
      if (i < argc) {
        inputPath = argv[i++];
      } else {
        usage(programName.c_str());
        exit(1);
      }
    } else if (arg.find("-output") == 0) {
      if (i < argc) {
        outputPath = argv[i++];
      } else {
        usage(programName.c_str());
        exit(1);
      }
    } else if (arg.find("-osample") == 0) {
      if (i < argc) {
        std::istringstream is(argv[i++]);
        is >> bitsPerSample;
      } else {
        usage(programName.c_str());
        exit(1);
      }
    } else if (arg.find("-ofs") == 0) {
      if (i < argc) {
        std::istringstream is(argv[i++]);
        is >> outputSampleRate;
      } else {
        usage(programName.c_str());
        exit(1);
      }
    }
  }
}
//==============================================================================

// prefill IOBUFFER and do any other variable initialization
static void initVariables() { memset(IOBUFFER, 0, sizeof(IOBUFFER)); }
//==============================================================================

static void callAllModules(HAOS_ROUTINE entryPoint) {
  HAOS_ModuleTable_t::const_iterator mb, me;
#ifdef __MP3DEC_
  if (entryPoint == FRAME || entryPoint == BRICK) {
    callAllModules(AFAP);
  }
#else
  for (mb = masterModuleTable.begin(), me = masterModuleTable.end(); mb != me;
       ++mb) {
    HAOS_Mct_t *HAOS_mctPtr = mb->MIF->MCT;
    switch (entryPoint) {
    case PREKICK: {
      if (HAOS_mctPtr->Prekick != 0) {
        HAOS_mctPtr->Prekick(mb->MIF);
      }
      break;
    }
    case POSTKICK: {
      if (HAOS_mctPtr->Postkick != 0) {
        HAOS_mctPtr->Postkick();
      }
      break;
    }
    case TIMER: {
      if (HAOS_mctPtr->Timer != 0) {
        HAOS_mctPtr->Timer();
      }
      break;
    }
    case FRAME: {
      if (HAOS_mctPtr->Frame != 0) {
        HAOS_mctPtr->Frame();
      }
      break;
    }
    case BRICK: {
      if (HAOS_mctPtr->Brick != 0) {
        HAOS_mctPtr->Brick();
      }
      break;
    }
    case AFAP: {
      if (HAOS_mctPtr->AFAP != 0) {
        HAOS_mctPtr->AFAP();
      }
      break;
    }
    case BACKGROUND: {
      if (HAOS_mctPtr->Background != 0) {
        HAOS_mctPtr->Background();
      }
      break;
    }
    case POSTMALLOC: {
      if (HAOS_mctPtr->Postmalloc != 0) {
        HAOS_mctPtr->Postmalloc();
      }
      break;
    }
    case PREMALLOC: {
      if (HAOS_mctPtr->Premalloc != 0) {
        HAOS_mctPtr->Premalloc();
      }
      break;
    }
    }
  }
}
#endif
  //==============================================================================
  static std::string trim(std::string const &str) {
    if (str.empty()) {
      return str;
    }

    std::size_t first = str.find_first_not_of(' ');
    std::size_t last = str.find_last_not_of(' ');

    return str.substr(first, last - first + 1);
  }
  //==============================================================================

  static void readCfgFile(const std::string pathName,
                          HAOS_HostCommQ_t &message) {
    std::ifstream is;
    is.open(pathName.c_str());

    if (is.is_open()) {
      std::string line;

      while (std::getline(is, line)) {
        if (line.find("#include") == 0) {
          // process include file
          std::string includePath = trim(line.substr(8, std::string::npos));
          readCfgFile(includePath, message);
        } else if (line.find("#") == 0) {
          // this is a comment, ignore it
        } else if (!line.empty()) {
          // assume these are hex digits - parse them into the message queue
          std::istringstream iss(line);

          while (!iss.eof()) {
            unsigned int msg;
            iss >> std::hex >> msg;
            message.push_back(msg);
          }
        }
      }
      is.close();
    } else {
      std::cerr << "Unable to open cfg file '" << pathName.c_str() << "'"
                << std::endl;
    }
  }
  //==============================================================================

  static void processHostComm(const HAOS_HostCommQ_t &messageQ) {
    HAOS_HostCommQ_t::const_iterator mb, me;

    for (mb = messageQ.begin(), me = messageQ.end(); mb != me;) {
      unsigned int cmd = *mb++;

      // parse the command word
      int moduleID = (cmd >> 24) & 0x7f;
      int opCode = (cmd >> 22) & 0x3;
      int numWords = ((cmd >> 16) & 0x1f) + 1;
      int offset = cmd & 0xffff;

      unsigned int *mcv = (unsigned int *)HAOS_getMcvPointer(moduleID);

      switch (opCode) {
      case 3: // read
      {
        // TODO: figure out if we want to support reads
        break;
      }
      default: // write, OR, or AND
      {
        // for each word of the payload, do the right thing
        while (mb != me && numWords > 0) {
          int value = *mb++;
          numWords--;

          // ovo ne valja,
          // kada se koriste double i float

          if (mcv != 0) {
            // messages are for a resident module
            switch (opCode) {
            case 0: {
              mcv[offset] = value;
              break;
            }
            case 1: {
              mcv[offset] |= value;
              break;
            }
            case 2: {
              mcv[offset] &= value;
              break;
            }
            }
          }
          ++offset;
        }
        break;
      }
      }
    }
  }
  //==============================================================================

  static void readPrekickConfigs() {
    HAOS_HostCommQ_t message;

    if (!cfgPath.empty()) {
      readCfgFile(cfgPath, message);
      // message is now a list of unsigned int messages
      processHostComm(message);
    }
  }
  //==============================================================================

  static bool openFile() {
    // if an output file is specified, try to open it and be prepared to read it
    if (!outputPath.empty()) {
      // figure out nChannels based on valid channel mask
      unsigned int mask = __HAOS_PPM_VALID_CHANNELS;
      validChMask = __HAOS_PPM_VALID_CHANNELS;

      while (mask != 0) {
        if (mask & 1) {
          nOutChannels++;
        }

        mask >>= 1;
      }

      outputHandle = cl_wavwrite_open(const_cast<char *>(outputPath.c_str()),
                                      bitsPerSample, nOutChannels, sampleRate);

      if (outputHandle == NULL) {
        std::cerr << "Unable to open output file '" << outputPath << "'"
                  << std::endl;
      } else {
        return true;
      }
    }

    return false;
  }
  //==============================================================================

  static void writeToFile() {
    int samplesToWrite = BRICK_SIZE;
    // int secondPart = 0;
    // if valid channel mask is changed figure out nOutChannels based on new
    // valid channel mask
    if (validChMask != __HAOS_PPM_VALID_CHANNELS) {
      unsigned int mask = __HAOS_PPM_VALID_CHANNELS;
      validChMask = __HAOS_PPM_VALID_CHANNELS;
      nOutChannels = 0;

      while (mask != 0) {
        if (mask & 1) {
          nOutChannels++;
        }

        mask >>= 1;
      }

      cl_wavwrite_update_wave_header(outputHandle, bitsPerSample, nOutChannels,
                                     outputSampleRate);
    }

    for (int sample = 0; sample < samplesToWrite; sample++) {
      // write one sample for every channel in HAOS_IOBUFFER
      for (int channel = 0; channel < nOutChannels; channel++) {
        cl_wavwrite_sendsample(outputHandle,
                               __HAOS_IOBUFFER_PTRS[channel][sample]);
      }
    }
  }
  //==============================================================================

  static bool flushFrameToFile() {
    int sampleNumber = cl_wavwrite_sample_number(outputHandle);
    cl_wavwrite_close(outputHandle);
    outputHandle = cl_wavwrite_reopen(const_cast<char *>(outputPath.c_str()),
                                      sampleNumber);

    if (outputHandle == NULL) {
      std::cerr << "Unable to open output file <flush> '" << outputPath << "'"
                << std::endl;
      return false;
    }

    return true;
  }
  //==============================================================================

  static void updatePtrs() {
    static int readCnt = 0;

    __HAOS_IO_FREE += BRICK_SIZE;
    readCnt++;

    if (readCnt % IO_BUFFER_PER_CHAN_MODULO == 0) {
      readCnt = 0;
    }

    for (int i = 0; i < NUMBER_OF_IO_CHANNELS; i++) {
      __HAOS_IOBUFFER_PTRS[i] = IOBUFFER[i][readCnt];
    }
  }
  //==============================================================================

  static void copyFrameData(HAOS_FrameData_t * frameData) {
    HAOS_frameData.inputChannelMask = frameData->inputChannelMask;
    HAOS_frameData.outputChannelMask = frameData->outputChannelMask;
    __HAOS_PPM_VALID_CHANNELS = frameData->outputChannelMask;
    __HAOS_PPM_INPUT_FS = frameData->sampleRate;
    HAOS_frameData.sampleRate = frameData->sampleRate;
    outputSampleRate = frameData->sampleRate;
    HAOS_frameData.decodeInfo = frameData->decodeInfo;
  }
  //==============================================================================

  void HAOS_copyBrickToIO(HAOS_CopyToIOPtrs_t * copyToIOPtrs) {
    static int writeCnt = 0;

    // copy frame data if present
    if (copyToIOPtrs->frameData != NULL) {
      copyFrameData(copyToIOPtrs->frameData);
    }

    // copy bricks to IO
    for (int i = 0; i < NUMBER_OF_IO_CHANNELS; i++) {
      // check if channel is present
      if (copyToIOPtrs->IOBufferPtrs[i]) {
        for (int j = 0; j < BRICK_SIZE; j++) {
          __HAOS_IOBUFFER_INP_PTRS[i][j] = copyToIOPtrs->IOBufferPtrs[i][j];
        }
      } else // channel not present
      {
        for (int j = 0; j < BRICK_SIZE; j++) {
          __HAOS_IOBUFFER_INP_PTRS[i][j] = 0;
        }
      }
    }

    // update counts
    __HAOS_IO_FREE -= BRICK_SIZE;

    // update haos inp ptrs
    writeCnt++;

    if (writeCnt % IO_BUFFER_PER_CHAN_MODULO == 0) {
      writeCnt = 0;
    }

    for (int i = 0; i < NUMBER_OF_IO_CHANNELS; i++) {
      __HAOS_IOBUFFER_INP_PTRS[i] = IOBUFFER[i][writeCnt];
    }
  }
  //==============================================================================

  //=============== exported functions for apps using the library
  //================
  void HAOS_init(int argc, const char *argv[]) {
    HAOS_parseCmdLine(argc, argv);
    // make sure the PCM decoder module is first if other decoder is not
    // specified
    masterModuleTable.push_back(*PcmDecoder_odtPtr);
  }
  //==============================================================================
  void HAOS_add_modules(void *moduleList) {
    HAOS_OdtEntry_t *odt = (HAOS_OdtEntry_t *)moduleList;
    while (odt != NULL) {
      if (odt->MIF != NULL) {
        masterModuleTable.push_back(*odt);
      } else {
        break;
      }
      odt++;
    }
  }
  //==============================================================================
  void HAOS_run() {
    // make sure the am module is last
    masterModuleTable.push_back(*AudioManager_odtPtr);
    // start emulation
    run();
  }

  int *HAOS_fillInputFIFO0(int *writePtr) {
    int sample = 0;
    // if an input file is specified, try to open it and be prepared to read it
    if (!inputPath.empty()) {
      if (firstOpen) {
        inputHandle = cl_wavread_open(const_cast<char *>(inputPath.c_str()));
        if (inputHandle == NULL) {
          std::cerr << "Unable to open input file '" << inputPath
                    << "': file does not exist or is not a WAV file"
                    << std::endl;
        } else {
          // input file already open flag
          firstOpen = false;
          std::cout << "Wave file opened!" << std::endl;
          bitsPerSample = cl_wavread_bits_per_sample(inputHandle);
          sampleRate = cl_wavread_frame_rate(inputHandle); // check this out
          nInputChannels = cl_wavread_getnchannels(inputHandle);
          samplesPerChannel = cl_wavread_number_of_frames(inputHandle);
          __HAOS_AT_EOF = samplesPerChannel <= 0;
          if (__HAOS_AT_EOF) {
            cl_wavread_close(inputHandle);
          }
        }
      }

      if (!__HAOS_AT_EOF) {
        // int currSample = 0;
        int samplesToRead = FIFO0_SIZE < (samplesPerChannel * nInputChannels)
                                ? FIFO0_SIZE
                                : (samplesPerChannel * nInputChannels);
        // read one sample
        for (; sample < samplesToRead; sample++) {
          inputFIFO0[sample] = cl_wavread_recvsample(inputHandle);
          writePtr++;
        }

        for (; sample < FIFO0_SIZE; sample++) {
          inputFIFO0[sample] = 0;
          writePtr++;
        }
        std::cout << "FIFO0 successfully filled." << std::endl;

        // check for EOF condition
        if (samplesPerChannel <= 0) {
          __HAOS_AT_EOF = true;
          cl_wavread_close(inputHandle);
        }
      }
    } else
      std::cout << "Cannot open file for read." << std::endl;
    return writePtr;
  }
  //==============================================================================
