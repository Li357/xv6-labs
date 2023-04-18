#include "kernel/types.h"
#include "user/user.h"

void primes(int);

int
main(void)
{
	int p0[2];
	pipe(p0);

	for (int i = 2; i <= 35; i++) {
		write(p0[1], &i, sizeof(i));
	}
	close(p0[1]);
	primes(p0[0]);
	exit(0);
}

void
primes(int lp)
{
	int p;
	read(lp, &p, sizeof(p));
	printf("prime %d\n", p);
	
	int p1[2];
	pipe(p1);

	int n = -1;
	while (read(lp, &n, sizeof(n))) {
		if (n % p != 0) {
			write(p1[1], &n, sizeof(n));
		}
	}
	close(lp);
	close(p1[1]);

	if (n > -1) {
		if (fork() == 0) {
			primes(p1[0]);
		} else {
			wait(0);
		}
	}
}			


