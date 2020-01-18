#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE	128

/**
**Three file descriptors are enabled when the program runs
@0:stdin
@1:stdout
@2:stderr
**/
int main(int ac, char *av[])
{
	int ret;
	char buf[BUFSIZE];
	char *p = "hello spring";

	memset(buf, 0, BUFSIZE);

	printf("input test data:\n");

	ret = read(0, buf, BUFSIZE);
	printf("command line data: %s", buf);

	printf("write to print screen\n");
	ret = write(1, p, strlen(p));
	printf("\nWrote %d bytes\n", ret);

	return 0;
}
