#include "modules/inputdev.h"

#ifdef inputdev_test
	#include "core/bitmap.c"
	#include "core/msgq.c"
	#include "core/gameloop.c"
	int main(){
		// TODO2 add test.
		return 0;
	}
#endif

void input_init_keys(struct bitmap **keys){
	// XXX Twin with process_init_flags();
	if (*keys)
		bitmap_destroy(*keys);
	*keys = bitmap_create(KEYS_NUM);
}

int inputdev_main(){  // XXX to be forked
	idata DB;
	DB.msgq_key = msgq_open("process");
	inputdev_open();

	gameloop(inputdev_loop, &DB, 30);

	inputdev_close();
	msgq_close(DB.msgq_key);
	return 0;
}

bool inputdev_loop(void *aux){
	//idata *DB = aux;
	// 1. get keys
	// 2. send it.
	//msgq_send(DB->msgq_key, MSG_TO_PROCESS, 0);
	return false;
}

void inputdev_open(){
	// TODO2 divide
}

void inputdev_close(){
	// TODO2 divide
}

#ifdef inputdev_test
#endif
