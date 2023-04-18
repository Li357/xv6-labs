#include "kernel/types.h"
#include "user/user.h"

const char byte = 'Q';

int
main(void)
{
	int pfd[2];
	pipe(pfd);

	char recvbyte;

	if (fork() == 0) {
		int pid = getpid();
		read(pfd[0], &recvbyte, 1);
		printf("%d: received ping\n", pid);
		write(pfd[1], &recvbyte, 1);
	} else {
		int pid = getpid();
		write(pfd[1], &byte, 1);
		read(pfd[0], &recvbyte, 1);
		printf("%d: received pong\n", pid);	
	}
	exit(0);
}
