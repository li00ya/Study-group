
ROOT_PATH := $(CURDIR)

export ROOT_PATH

###########################################################################################
# product configure
###########################################################################################

build_date := $(shell date +%Y%m%d%H%M%S)

version := 1.0
ifeq ("$(origin VERSION)", "command line")
version := $(VERSION)
endif

product := develop
ifeq ("$(origin PRODUCT)", "command line")
product := $(PRODUCT)
endif

export PRODUCT_NAME=$(product)

LEE_REMOVE := core include experience macro Process Test
LEE_PATH := $(ROOT_PATH)/lee

lee_available := $(wildcard lee/*)
lee_available := $(notdir $(lee_available))
lee_total := $(filter-out $(LEE_REMOVE), $(lee_available))

###########################################################################################
# compiler configure
###########################################################################################

cfg_lee := $(shell echo $(lee_total) | tr a-z A-Z)

-DCONFIG_LEE_ :=

extra_cflags := -DPRODUCT_NAME=\"$(product)\" -DPRODUCT_VERSION=\"$(version)\"
extra_cflags += -DBUILD_DATETIME=\"$(build_date)\"
extra_cflags += $(addprefix -DCONFIG_LEE_, $(cfg_lee))

CFLAGS += -Wall -O2 -g
CFLAGS += -I. -I$(LEE_PATH)/include
export CFLAGS

#generate core library
top_lib := top.o
export TOP_LIB=$(ROOT_PATH)/$(top_lib)

LDFLAGS += -lm -lpthread
#LDFLAGS += -Wl,--gc-sections -lrt
export LDFLAGS

CROSS_COMPILE =
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
CC = $(CROSS_COMPILE)gcc
CPP = $(CC) -E
AR = $(CROSS_COMPILE)ar
NM = $(CROSS_COMPILE)nm

STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

###########################################################################################
# directory configure
###########################################################################################

include scripts/Makefile.include

coredirs := $(wildcard $(LEE_PATH)/core/*)
coreobjs := $(addsuffix /$(ldobjs), $(coredirs))

subdirs := $(strip $(coredirs))
subobjs := $(strip $(coreobjs))

prog_target := $(addprefix lee/, $(lee_total))

###########################################################################################
# generate programe
###########################################################################################


.PHONY: prepare all clean help upgrade FORCE
.PHONY: prog_build prog_clean top_clean

all : prepare prog_build

clean : top_clean prog_clean

prog_build : $(top_lib)
	@for d in $(prog_target); do make $(gen)=$$d; done

prog_clean:
	@for d in $(prog_target); do make $(clean)=$$d; done

$(top_lib) : $(subdirs) FORCE
	@for d in $(subdirs); do make $(build)=$$d; done
	@$(LD_ECHO)
	@$(LD) -r -o $(top_lib) $(subobjs)

top_clean : $(subdirs)
	@for d in $(subdirs); do make $(clean)=$$d; done
	@rm -rf $(top_lib)

upgrade : prog_build


prepare :


FORCE:

help: prepare
	@echo "command: make [PRODUCT=product] [ARCH=x86_64] [VERSION=1.0]"
	@echo "product: optional.listed as below.default is develop"
	@echo "         $(all_product)"
	@echo "   arch: optional.listed as below.default is set by product cfg"
	@echo "         $(all_arch)"
	@echo "version: optional. the format is major.minor"
	@echo ""

