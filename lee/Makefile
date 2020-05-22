#记住当前工程的根目录路径
ROOT_DIR = $(CURDIR)

#设置编译器
.PHONY:clean all

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

INCLUDE_DIR := -I . -I $(ROOT_DIR)/include
CFLAGS := -Wall -O2 -g
LDFLAGS := -lm
RM := rm -rf

export INCLUDE_DIR CFLAGS LDFLAGS

#debug文件夹里的makefile文件需要最后执行，所以这里需要执行的子目录要排除obj文件夹，这里使用awk排除了debug文件夹，读取剩下的文件夹
SUBDIRS = $(shell ls -l | grep ^d | awk '{if($$9 != "obj") if($$9 != "include") print $$9}')

#最终bin文件的名字，可以更改为自己需要的
BIN = app

#目标文件所在的目录
OBJS_DIR = obj

#bin文件所在的目录
BIN_DIR = ./

#获取当前目录下的c文件集，放在变量CUR_SOURCE中
CUR_SOURCE := $(wildcard *.c)

#将对应的c文件名转为o文件后放在下面的CUR_OBJS变量中
CUR_OBJS := $(patsubst %.c, %.o, $(CUR_SOURCE))

#将以下变量导出到子shell中，本次相当于导出到子目录下的makefile中
export CC BIN OBJS_DIR BIN_DIR ROOT_DIR

#注意这里的顺序，需要先执行SUBDIRS最后才能是DEBUG
all: $(SUBDIRS) $(CUR_OBJS) $(APP)

#递归执行子目录下的makefile文件，这是递归执行的关键
$(SUBDIRS): ECHO
	make -C $@
APP: ECHO
	make -C obj
ECHO:
#	@echo $(SUBDIRS)
#将c文件编译为o文件，并放在指定放置目标文件的目录中即OBJS_DIR

$(CUR_OBJS): %.o : %.c
	$(CC) -c $^ -o $(ROOT_DIR)/$(OBJS_DIR)/$@
clean:
	$(RM) $(OBJS_DIR)/*.o
	$(RM) $(BIN)
