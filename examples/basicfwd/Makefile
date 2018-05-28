
ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif
RTE_TARGET ?= x86_64-native-linuxapp-gcc
include $(RTE_SDK)/mk/rte.vars.mk

APP = out
SRCS-y := main.c
CFLAGS += $(WERROR_FLAGS)

ifeq ($(CONFIG_RTE_TOOLCHAIN_GCC),y)
CFLAGS_main.o += -Wno-return-type
endif
EXTRA_CFLAGS += -O3 -g -Wfatal-errors
include $(RTE_SDK)/mk/rte.extapp.mk

run: 
	./build/out

gdb:
	gdb ./build/out
	
