#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE	128

void fork1(int ret, char *arg)
{
	printf("this child process:\n"
		   "  Read %d bytes:[%s]\n", ret, arg);
}

int main(int ac, char **av)
{
	int ret;
	int pipid[2];
	char *p = "hello boy";
	char buf[BUFSIZE];
	pid_t pid;

	memset(buf, 0, BUFSIZE);

	if (0 == pipe(pipid)) {
		pid = fork();

		if (-1 == pid) {
			fprintf(stderr, "Fork Failure");
			return -1;
		} else if (0 == pid) {
			ret = read(pipid[0], buf, BUFSIZE);
			fork1(ret, buf);
		} else {
			ret = write(pipid[1], p, strlen(p));
			printf("Parent process Write %d bytes: %s\n", ret, p);
		}
	}

	return 0;
}
