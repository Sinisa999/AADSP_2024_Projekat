include ../common.mk

TARGET	:= fxpt
LIBRARY	:= libfxpt.a
OBJECTS	= src/fxpt.o src/main.o

CFLAGS	+= -I ./include

.PHONY: all clean

all: $(TARGET) $(LIBRARY)

clean:
	rm -f $(OBJECTS) $(TARGET) $(LIBRARY)

$(TARGET): src/fxpt.o src/main.o
	$(CC) $(CFLAGS) -static -o $@ $^

$(LIBRARY): src/fxpt.o
	ar rcs $@ src/fxpt.o

