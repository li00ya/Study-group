
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

LEE_REMOVE := core include
LEE_PATH := $(ROOT_PATH)/lee

lee_available := $(wildcard lee/*)
lee_available := $(notdir $(lee_available))
lee_total := $(filter-out $(LEE_REMOVE), $(lee_available))

###########################################################################################
# compiler configure
###########################################################################################

cfg_lee := $(shell echo $(lee_total) | tr a-z A-Z)

extra_cflags := -DPRODUCT_NAME=\"$(product)\" -DPRODUCT_VERSION=\"$(version)\"
extra_cflags += -DBUILD_DATETIME=\"$(build_date)\"
extra_cflags += $(addprefix -DCONFIG_LEE_TOTAL, $(cfg_lee))

PROG_CFLAGS += -Wall -O2
PROG_CFLAGS += -std=gnu99 -ffunction-sections -fdata-sections
PROG_CFLAGS += -I. -I$(LEE_PATH)/include
PROG_CFLAGS += $(extra_cflags)
export PROG_CFLAGS

#generate core library
top_lib := libtop.a
export TOP_LIB=$(ROOT_PATH)/$(top_lib)
export TOP_CMD = -L$(ROOT_PATH) -Wl,--whole-archive -ltop -Wl,--no-whole-archive

LDFLAGS += -lm -lpthread
#LDFLAGS += -Wl,--gc-sections -lrt
export LDFLAGS

CROSS_COMPILE :=

override LD := $(CROSS_COMPILE)ld
override CC := $(CROSS_COMPILE)gcc
override AR := $(CROSS_COMPILE)ar
override STRIP := $(CROSS_COMPILE)strip

export LD CC AR STRIP

###########################################################################################
# directory configure
###########################################################################################

include scripts/Makefile.include
core_remove := $(LEE_PATH)/core/链接函数文件夹
core_total := $(wildcard $(LEE_PATH)/core/*)
coredirs := $(filter-out $(core_remove), $(core_total))
coreobjs := $(addsuffix /$(ldobjs), $(coredirs))

subdirs := $(strip $(coredirs))
subobjs := $(strip $(coreobjs))

lee_target := $(addprefix lee/, $(lee_total))
prog_target := $(wildcard prog/*)

###########################################################################################
# generate programe
###########################################################################################


.PHONY: prepare all clean help upgrade FORCE
.PHONY: lee_build lee_clean prog_build prog_clean top_clean

all : prepare lee_build prog_build

clean : top_clean lee_clean prog_clean

lee_build : $(top_lib)
	@for d in $(lee_target); do make $(gen)=$$d; done

lee_clean:
	@for d in $(lee_target); do make $(clean)=$$d; done

prog_build : $(top_lib)
	@for d in $(prog_target); do make $(gen)=$$d; done

prog_clean:
	@for d in $(prog_target); do make $(clean)=$$d; done

$(top_lib) : $(subdirs) FORCE
	@for d in $(subdirs); do make $(build)=$$d; done
	@$(AR_ECHO)
	@$(AR_CMD) $(subobjs)

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
	@echo "   lee: optional.listed as below.default is set by product cfg"
	@echo "         $(lee_available)"
	@echo "version: optional. the format is major.minor"
	@echo ""

