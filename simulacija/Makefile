MODULES	:= ./fxpt ./xt_hifi4 ./HAOS_sim
MKFILES	= $(addsuffix /Makefile, $(MODULES))

.PHONY: all clean

all: $(MKFILES)
	$(foreach module, $(MODULES), make -C $(module) all;)

clean: $(MKFILES)
	$(foreach module, $(MODULES), make -C $(module) clean;)
