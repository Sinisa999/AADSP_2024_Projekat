include ../common.mk

TARGET		:= haos
OBJECTS		= $(shell find . -type f -name '*.c*' | sed 's/\.(c\|cpp\|cxx)$$/o/')
HEADERS_C	= $(shell find . -type f -name '*.h' | xargs dirname | sort | uniq)
HEADERS_CXX	= $(shell find . -type f -name '*.h*' | xargs dirname | sort | uniq)
INCLUDE_C	= $(addprefix -I, $(HEADERS_C))
INCLUDE_CXX	= $(addprefix -I, $(HEADERS_CXX))

CFLAGS		+= $(INCLUDE_C) -I ../fxpt/include -I ../xt_hifi4/include
CXXFLAGS	+= $(INCLUDE_CXX) -I ../fxpt/include -I ../xt_hifi4/include

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS) ../xt_hifi4/libxt_hifi4.a ../fxpt/libfxpt.a
	$(CXX) $(CXXFLAGS) -static -o $@ $^ -L ../xt_hifi4 -lxt_hifi4 -L ../fxpt -lfxpt
