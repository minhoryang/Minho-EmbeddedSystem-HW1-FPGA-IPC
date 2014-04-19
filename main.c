#include "main.h"

int main(){
	printf("main\n");
	new_fork(inputdev_main);
	new_fork(outputdev_main);
	process_main();
	return 0;
}
