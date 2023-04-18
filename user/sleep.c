#include "kernel/types.h"
#include "user/user.h"

const char *err_msg = "Invalid usage! Expected: sleep <ticks>\n";

int
main(int argc, char *argv[])
{
  int ticks;
  
  if (argc != 2) {
		write(1, err_msg, strlen(err_msg));
	} else {
		ticks = atoi(argv[1]);
	  sleep(ticks);
	}
	exit(0);
}	
