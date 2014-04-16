#include "modules/stop_watch.h"

#ifdef stop_watch_test
	#include "core/bitmap.c"
	#include "core/msgq.c"
	#include "modules/process.c"

	int main(){
		// TODO
		return 0;
	}
#endif

void stop_watch(struct bitmap *keys, struct bitmap *flags, key_t msgq_key){
	switch(bitmap_scan(keys, 0, KEYS_NUM, false)){
		case BITMAP_ERROR:
		default:
			break;
		case IN_SWITCH_GPIO_SELECT:  // XXX NEVER HAPPEN IT, just test for linking.
			msgq_send(msgq_key, MSG_TO_OUTPUT, 0);  // TODO
			break;
	}
}

#ifdef stop_watch_test
#endif
