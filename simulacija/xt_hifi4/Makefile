include ../common.mk

TARGET		:= xt_hifi4
LIBRARY		:= libxt_hifi4.a
OBJECTS_LIB	= $(shell find . -type f -not -name 'main.c*' -name '*.c*' | sed 's/\.(c\|cpp\|cxx)$$/o/')
OBJECTS		= $(OBJECTS_LIB) src/main.o

CXXFLAGS+= -I ../fxpt/include -I ./include

.PHONY: all clean

all: $(TARGET) $(LIBRARY)

clean:
	rm -f $(OBJECTS) $(TARGET) $(LIBRARY)

$(TARGET): $(OBJECTS) ../fxpt/libfxpt.a
	$(CXX) $(CXXFLAGS) -static -o $@ $^ -L ../fxpt -lfxpt

$(LIBRARY): $(OBJECTS_LIB)
	ar rcs $@ $(OBJECTS_LIB)
