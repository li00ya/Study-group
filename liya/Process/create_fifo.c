#include <unistd.h>
#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int ac, char **av)
{
	int ret = mkfifo("/tmp/my_fifo", 0777);
	if (0 == ret) printf("my fifo create sucess\n");

	return 0;
}
