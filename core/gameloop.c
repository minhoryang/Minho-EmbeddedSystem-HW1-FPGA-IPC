#include "gameloop.h"

#ifdef gameloop_test
	#include "fork.c"

	bool test2(void *aux){
		usleep(1000000);
		return true;
	}
	int test(){
		gameloop(&test2, NULL, 120);
		return 0;
	}
	int main(){
		new_fork(test);
		new_fork(test);
		return 0;
	}
#endif

static long diff(struct timeval *t1, struct timeval *t2)
{
    return (((t1->tv_sec - t2->tv_sec) * 1000000) +
            (t1->tv_usec - t2->tv_usec));
}

void gameloop(bool (*GAMEFUNC)(void *), void *aux, int FPS){
	struct timeval end, start;
	double waitTime = 1000 * 1000 / FPS, overflow = 0;

	// XXX FPS Counter Init.
	struct timeval init1;
	long long int cnt = 0;
	gettimeofday(&init1, NULL);

	while(true){
		gettimeofday(&start, NULL);

		if(GAMEFUNC(aux))
			break;

		usleep((unsigned int)(waitTime - overflow));

		gettimeofday(&end, NULL);

		if((overflow = (double)diff(&end, &start) - waitTime) < 0.0)
			overflow = 0.0;

		// XXX FPS Print.
		++cnt;
		if(!(cnt % (FPS * 5)))
			printf("%s fps %lf\n", __FILE__, cnt / (diff(&end, &init1) / (double)1000000));
	}
}
