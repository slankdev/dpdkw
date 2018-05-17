ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif
RTE_TARGET ?= x86_64-native-linuxapp-gcc
include $(RTE_SDK)/mk/rte.vars.mk
APP = out
SRCS-y := main.c
CFLAGS += $(WERROR_FLAGS) 
EXTRA_CFLAGS += -O3 -g -Wfatal-errors -I$(HOME)/dpdkw
include $(RTE_SDK)/mk/rte.extapp.mk

run:
	sudo ./build/out
