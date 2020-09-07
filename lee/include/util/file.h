#ifndef __UTIL_FILE_H__
#define __UTIL_FILE_H__

#include "util/types.h"

enum
{
	FILE_FLAG_CREATE = 0x1,
	FILE_FLAG_APPEND = 0x2,
	FILE_FLAG_WRITE = 0x4,
	FILE_FLAG_READ  = 0x8,
	FILE_FLAG_EXIST = 0x10,
	FILE_FLAG_EXEC  = 0x20,
	FILE_FLAG_SYNC = 0x40,
};

typedef void* file_handle_t;

#define FILE_INVALID    ((file_handle_t)(-1))

int32_t util_file_open(file_handle_t* handle, int8_t* filename, uint32_t flag);
int32_t util_file_close(file_handle_t handle);
int32_t util_file_remove(int8_t* filename);

typedef struct
{
	uint32_t size;
}file_stat_t;
int32_t util_file_stat(int8_t* filename, file_stat_t* st);

enum
{
	FILE_SEEK_TYPE_SET,
	FILE_SEEK_TYPE_CUR,
	FILE_SEEK_TYPE_END,
	FILE_SEEK_TYPE_MAX
};
int32_t util_file_seek(file_handle_t handle, int32_t offset, int32_t type);

int32_t util_file_write(file_handle_t handle, void* buf, uint32_t len);
int32_t util_file_read(file_handle_t handle, void* buf, uint32_t len);

int32_t util_file_pwrite(file_handle_t handle, void* buf, uint32_t len, uint32_t pos);
int32_t util_file_pread(file_handle_t handle, void* buf, uint32_t len, uint32_t pos);

int32_t util_file_access(int8_t* filename, uint32_t flag);

int32_t util_file_rename(int8_t* oldfile, int8_t* newfile);

int32_t util_file_flush(file_handle_t handle);

int32_t util_file_mmap(file_handle_t handle, uint32_t size, uint64_t offset, void** addr);

int32_t util_file_munmap(file_handle_t handle, void* ptr, uint32_t size);

enum
{
	FILE_STDIO_INPPUT,
	FILE_STDIO_OUTPUT,
	FILE_STDIO_ERROR,
	FILE_STDIO_SINPUT,
	FILE_STDIO_SOUTPUT,
	FILE_STDIO_SERROR,
	FILE_STDIO_MAX
};
int32_t util_file_query(file_handle_t* handle, uint32_t type);

int32_t util_file_lock(file_handle_t handle, uint32_t flag);

int32_t util_file_unlock(file_handle_t handle);

int32_t util_file_truncate(file_handle_t handle, uint32_t size);

/* stream operation */
int32_t util_file_fopen(file_handle_t* handle, int8_t* filename, uint32_t flag);

int32_t util_file_fclose(file_handle_t handle);

int32_t util_file_fgets(file_handle_t handle, void* buf, uint32_t size);

int32_t util_file_ftell(file_handle_t handle);

/* dir operation */
int32_t util_dir_open(file_handle_t* handle, int8_t* filename);
int32_t util_dir_close(file_handle_t handle);

#define FILE_NAME_LEN_MAX    256
typedef struct
{
	uint32_t type;
	int8_t name[FILE_NAME_LEN_MAX];
}dir_info_t;
int32_t util_dir_read(file_handle_t handle, dir_info_t* info);

int32_t util_dir_create(int8_t* filename);

int32_t util_dir_destroy(int8_t* filename);

#endif
