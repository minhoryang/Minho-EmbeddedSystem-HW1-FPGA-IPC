#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef fork_h
	#define fork_h
	pid_t new_fork(int (*child)(void));
#endif
