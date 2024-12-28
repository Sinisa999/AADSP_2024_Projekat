ifdef NDEBUG
OPT_LEVEL	:= -flto -O3 -march=native -mtune=native
else
OPT_LEVEL	:= -g -O0 -march=native -mtune=native
endif

CC		:= clang
CXX		:= clang++
CSTD		:= c17
CXXSTD		:= c++17
CFLAGS		+= $(OPT_LEVEL) -Wall -std=$(CSTD)
CXXFLAGS	+= $(OPT_LEVEL) -Wall -std=$(CXXSTD)
