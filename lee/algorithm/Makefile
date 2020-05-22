
#INCLUDE_DIR += -I$(ROOT_DIR)/include

#获取子目录名
#此命令是把ls -l 输出结果中每行文件属性以d开头的第九列字符返回 
SUBDIRS := $(shell ls -l | grep ^d | awk '{print $$9}')

#以下同根目录下的makefile的相同代码的解释
CUR_SOURCE := $(wildcard *.c)
CUR_OBJS := $(patsubst %.c, %.o, $(CUR_SOURCE))

all:$(SUBDIRS) $(CUR_OBJS)

$(SUBDIRS): ECHO
	make -C $@

$(CUR_OBJS): %.o:%.c
	$(CC) $(LDFLAGS) $(INCLUDE_DIR) $(CFLAGS) -c $^ -o $(ROOT_DIR)/$(OBJS_DIR)/$@

ECHO:
	@echo $(SUBDIRS)
