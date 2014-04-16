#include "main.h"

int main(){
	new_fork(inputdev_main);
	new_fork(outputdev_main);
	process_main();
	return 0;
}
