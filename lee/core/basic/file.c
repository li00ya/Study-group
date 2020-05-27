#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/file.h>

#include "util/file.h"
#include "util/string.h"

#define HAND2FD(handle)      ((int32_t)(uvoid_t)handle)

static uint32_t util_trans_flag(uint32_t flag)
{
	uint32_t newflag = O_CLOEXEC | O_RDWR;
	
	if (flag & FILE_FLAG_CREATE) {
		newflag |= O_CREAT | O_TRUNC;
	}

	if (flag & FILE_FLAG_APPEND) {
		newflag |= O_APPEND;
	}
	
	if (flag & FILE_FLAG_SYNC) {
		flag |= O_SYNC;
	}
	
	return newflag;
}

int32_t util_file_open(file_handle_t* handle, int8_t* filename, uint32_t flag)
{
	int32_t fd;

	flag = util_trans_flag(flag);
	if (flag & O_CREAT) {
		fd = open(filename, flag, 0664);
	} else {
		fd = open(filename, flag);
	}
	if (fd < 0) {
		return -1;
	}

	*handle = (void*)(uvoid_t)fd;

	return 0;
}

int32_t util_file_close(file_handle_t handle)
{
	return close(HAND2FD(handle));
}

int32_t util_file_remove(int8_t* filename)
{
	return unlink(filename);
}

int32_t util_file_stat(int8_t* filename, file_stat_t* st)
{
	struct stat raw;

	if (stat(filename, &raw)) {
		return -1;
	}

	st->size = raw.st_size;

	return 0;
}

int32_t util_file_seek(file_handle_t handle, int32_t offset, int32_t type)
{
	static int32_t trans[FILE_SEEK_TYPE_MAX] = {SEEK_SET, SEEK_CUR, SEEK_END};

	return lseek(HAND2FD(handle), offset, trans[type]);
}

int32_t util_file_write(file_handle_t handle, void* buf, uint32_t len)
{
	return write(HAND2FD(handle), buf, len);
}

int32_t util_file_read(file_handle_t handle, void* buf, uint32_t len)
{
	return read(HAND2FD(handle), buf, len);
}

int32_t util_file_pwrite(file_handle_t handle, void* buf, uint32_t len, uint32_t pos)
{
	return pwrite(HAND2FD(handle), buf, len, pos);
}

int32_t util_file_pread(file_handle_t handle, void* buf, uint32_t len, uint32_t pos)
{
	return pread(HAND2FD(handle), buf, len, pos);
}

int32_t util_file_access(int8_t* filename, uint32_t flag)
{
	int32_t ret;

	switch (flag) {
	case FILE_FLAG_WRITE:
		ret = access(filename, W_OK);
		break;

	case FILE_FLAG_EXEC:
		ret = access(filename, X_OK);
		break;
	case FILE_FLAG_EXIST:
		ret = access(filename, F_OK);
		break;
	default:
		ret = access(filename, R_OK);
		break;
	}

	return ret;
}

int32_t util_file_rename(int8_t* oldfile, int8_t* newfile)
{
	return rename(oldfile, newfile);
}

int32_t util_file_flush(file_handle_t handle)
{
	return fsync(HAND2FD(handle));
}

int32_t util_file_mmap(file_handle_t handle, uint32_t size, uint64_t offset, void** addr)
{
	int32_t fd = HAND2FD(handle);
	uint32_t flag = MAP_SHARED | MAP_LOCKED;
	void* ptr;
	
	if (FILE_INVALID == handle) {
		flag |= MAP_ANONYMOUS;
	} else {
		flag |= MAP_POPULATE;
	}

	ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, flag, fd, (off_t)offset);
	if (MAP_FAILED == ptr) {
		return -1;
	}

	*addr = ptr;

	return 0;
}

int32_t util_file_munmap(file_handle_t handle, void* ptr, uint32_t size)
{
	return munmap(ptr, size);
}

int32_t util_file_query(file_handle_t* handle, uint32_t type)
{
	void* stdios[FILE_STDIO_MAX] = {(void*)0, (void*)1, (void*)2, stdin, stdout, stderr};

	if (type >= FILE_STDIO_MAX) {
		return -1;
	}

	*handle = stdios[type];

	return 0;
}

int32_t util_file_lock(file_handle_t handle, uint32_t flag)
{
	return flock(HAND2FD(handle), LOCK_EX);
}

int32_t util_file_unlock(file_handle_t handle)
{
	return flock(HAND2FD(handle), LOCK_UN);
}

int32_t util_file_truncate(file_handle_t handle, uint32_t size)
{
	return ftruncate(HAND2FD(handle), size);
}

int32_t util_file_fopen(file_handle_t* handle, int8_t* filename, uint32_t flag)
{
	int8_t* cmd = (FILE_FLAG_READ & flag) ? "rb" : "wb";
	FILE* fd = fopen(filename, cmd);

	if (NULL == fd) {
		return -1;
	}

	*handle = fd;

	return 0;
}

int32_t util_file_fclose(file_handle_t handle)
{
	return fclose(handle);
}

int32_t util_file_fgets(file_handle_t handle, void* buf, uint32_t size)
{
	if (NULL == fgets(buf, size, handle)) {
		return -1;
	}

	return 0;
}

int32_t util_file_ftell(file_handle_t handle)
{
	return ftell(handle);
}

int32_t util_dir_open(file_handle_t* handle, int8_t* filename)
{
	DIR* hdir = opendir(filename);

	if (NULL == hdir) {
		return -1;
	}

	*handle = hdir;

	return 0;
}

int32_t util_dir_close(file_handle_t handle)
{
	return closedir((DIR*)handle);
}

int32_t util_dir_read(file_handle_t handle, dir_info_t* info)
{
	struct dirent* ent;

	ent = readdir((DIR*)handle);
	if (NULL == ent) {
		return -1;
	}

	info->type = ent->d_type;
	util_strncpy(info->name, ent->d_name, sizeof(info->name));

	return 0;
}

int32_t util_dir_create(int8_t* filename)
{
	return mkdir(filename, 0644);
}

int32_t util_dir_destroy(int8_t* filename)
{
	return rmdir(filename);
}

