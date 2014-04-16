#include "modules/outputdev.h"

#ifdef outputdev_test
	#include "core/msgq.c"
	#include "core/gameloop.c"
	int main(){
		return 0;
	}
#endif

int outputdev_main(){
	odata DB;
	DB.msgq_key = msgq_open("process");
	outputdev_open();

	gameloop(outputdev_loop, &DB, 30);

	outputdev_close();
	msgq_close(DB.msgq_key);
	return 0;
}

bool outputdev_loop(void *aux){
	//odata *DB = aux;
	return false;
}

void outputdev_open(){
	;
}

void outputdev_close(){
	;
}
