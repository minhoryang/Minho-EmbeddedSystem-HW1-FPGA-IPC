#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>

#ifndef gameloop_h
	#define gameloop_h

	void gameloop(bool (*GAMEFUNC)(void *), void *aux, int FPS);
#endif
