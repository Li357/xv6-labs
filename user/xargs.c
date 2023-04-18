#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
	char xargs[32][MAXARG]; // assume each argument can only be 32-char string
	char *argsp[MAXARG]; // pointer to args

	int i;
	for (i = 0; i < MAXARG; i++)
		argsp[i] = xargs[i]; // match pointers

	for (i = 1; i < argc; i++)
		strcpy(xargs[i - 1], argv[i]);

	int pos = argc - 1; // first argc - 1 args already provided by xargs, rest are from stdin
	char *p = xargs[pos];

	while (read(0, p, 1)) {
		if (*p == ' ') {
			*p = '\0';
			p = xargs[++pos];
		} else if (*p == '\n') {
			*p = '\0';
			argsp[pos + 1] = 0;
			pos = argc - 1;
			p = xargs[pos];

			if (fork()) {
				wait(0);
			} else {
				exec(argsp[0], argsp);
			}
		} else {
			p++;
		}
	}
	exit(0);
}
