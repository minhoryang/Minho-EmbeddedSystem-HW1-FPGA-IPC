#include "fork.h"

#ifdef fork_test
	int test();

	int main(){
		new_fork(test);
		new_fork(test);
		new_fork(test);
		printf("parent");
		// TODO : Add more test.
		return 0;
	}

	int test(){
		printf("child!");
		return 0;
	}
#endif

pid_t new_fork(int (*child)(void)){
	// TODO discuss SIGNAL.
	// XXX As far as I tested,
	//    1) if parent exit safely, parent didn't wait childs.
	//    2) if parent exit unexpectedly, parent kill childs also.
	// need to 
	//    1) parent say goodbye to childs.
	//    2) chld say goodbay to parent.
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
