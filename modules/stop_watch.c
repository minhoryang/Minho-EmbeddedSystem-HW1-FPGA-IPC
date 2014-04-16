#include "modules/stop_watch.h"

void stop_watch(struct bitmap *keys, struct bitmap *flags, key_t msgq_key){
	switch(bitmap_scan(keys, 0, KEYS_NUM, false)){
		case BITMAP_ERROR:
		default:
			break;
		case IN_BUTTON_GPIO_1:  // XXX NEVER HAPPEN IT, just test for linking.
		case IN_BUTTON_FPGA_1:
			msgq_send(msgq_key, MSG_TO_OUTPUT, 0);  // TODO
			break;
	}
}
