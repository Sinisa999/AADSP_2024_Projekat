/*
 * bitripper_sim.h
 *
 *  Created on: Aug 12, 2024
 *      Author: katarinac
 */

#ifndef __BITRIPPER_H__
#define __BITRIPPER_H__

#define MAX_BITS 32
#define FIFO0_SIZE 2048

typedef struct {
  int currentWord;
  int bitsRemaining;
  int *inputReadPtr;
  int *inputEndAddrP1;
  int *inputBaseAddr;
  int reserved[2];
  int *writePtr;
} BitRipper_t;

extern int inputFIFO0[FIFO0_SIZE];

extern BitRipper_t currStateStruct;
extern BitRipper_t auxStateStruct;
extern BitRipper_t mainStructBackup;
extern bool inAuxState;
extern int alignmentInfo;
extern int overflowCntFIFO0;

// initializes the BitRipper - will be called by the haOS
extern void BitRipper_init(BitRipper_t &currStateStruct);
// extracts passed number of needed bits from current word
extern int BitRipper_extractBits(int bitsNeeded);
// saves the state corresponding to the MainPtr
extern void BitRipper_saveMainState();
// restores the state corresponding to the MainPtr
extern void BitRipper_restoreMainState();
// loads MainState from the AuxState
extern void BitRipper_loadMainState();
// saves the current state
extern void BitRipper_saveAuxState();
// loads the current state from auxStateStruct
extern void BitRipper_loadAuxState();
// gives the dipstick in bits between specified states (i7 main; i6 aux)
extern int BitRipper_bitCntStates(BitRipper_t &currStateStruct,
                                  int *inputFIFO0);
// gives the dipstick in bits between specified states
extern int BitRipper_bitCntMainState(BitRipper_t &currStateStruct);
// peeks passed number of needed bits from current word
extern int BitRipper_peek(int bitsNeeded);
// skips the specified number of bits
extern void BitRipper_skipBits(int bitsNum);
extern int BitRipper_readDipstick();
// saves the alignment information
extern void BitRipper_saveAlignment(int bitsOffset);
// performs a common calculation used for aligntoXXX routines
extern int BitRipper_alignCommon();
// aligns to a 8-bit boundary w.r.t the saved alignment-info
extern void BitRipper_align2byte();
// aligns to a 16-bit boundary w.r.t the saved alignment-info
extern void BitRipper_align2word();
// aligns to a 32-bit boundary w.r.t the saved alignment-info
extern void BitRipper_align2DWord();

#endif /* __BITRIPPER_H__ */
