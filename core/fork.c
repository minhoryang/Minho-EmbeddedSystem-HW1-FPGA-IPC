#include "fork.h"

#ifdef fork_test
	int test();

	int main(){
		new_fork(test);
		new_fork(test);
		new_fork(test);
		printf("parent");
		return 0;
	}

	int test(){
		printf("child!");
		return 0;
	}
#endif

pid_t new_fork(int (*child)(void)){
	pid_t ret = fork();
	switch(ret){
		case -1:
			return -1;
		case 0:
			exit(child());
		default:
			return ret;
	}
}
