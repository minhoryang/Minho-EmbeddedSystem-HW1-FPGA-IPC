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
	inputdev_open(&DB);

	gameloop(inputdev_loop, &DB, 30);

	inputdev_close(&DB);
	msgq_close(DB.msgq_key);
	return 0;
}

bool inputdev_loop(void *aux){
	idata *DB = aux;
	// 1. get keys
	{  // GPIO event
		size_t t = read(DB->gpio_fp, DB->gpio, sizeof(struct input_event) * 32);
		if(t > sizeof(struct input_event)){
			if(DB->gpio[0].value == 1){
				int t = -1;
				switch(DB->gpio[0].code){
					case VALUE_SWITCH_GPIO_SELECT:
						t = IN_SWITCH_GPIO_SELECT;
						break;
					case VALUE_SWITCH_GPIO_UP:
						t = IN_SWITCH_GPIO_UP;
						break;
					case VALUE_SWITCH_GPIO_DOWN:
						t = IN_SWITCH_GPIO_DOWN;
						break;
					case VALUE_BUTTON_GPIO_1:
						t = IN_BUTTON_GPIO_1;
						break;
					case VALUE_BUTTON_GPIO_2:
						t = IN_BUTTON_GPIO_2;
						break;
					case VALUE_BUTTON_GPIO_3:
						t = IN_BUTTON_GPIO_3;
						break;
					case VALUE_BUTTON_GPIO_4:
						t = IN_BUTTON_GPIO_4;
						break;
					default:
						break;
				}
				if(t != -1)
				{
					msg_pack i;
					i.mtype = MSG_TO_PROCESS;
					i.mdata = t;
					i.mbool = true;
					msgq_send(DB->msgq_key, &i);
				}
			}
		}
	}
	{  // FPGA
		unsigned char push_sw_buff[9];
		int i;
		read(DB->fpga_fp, &push_sw_buff, sizeof(push_sw_buff));
		for(i=0;i<9;i++){
			if(!DB->pushed_fpga[i] && push_sw_buff[i]){
				msg_pack j;
				j.mtype = MSG_TO_PROCESS;
				j.mdata = IN_BUTTON_FPGA_1 + i;
				msgq_send(DB->msgq_key, &j);
			}
			DB->pushed_fpga[i] = push_sw_buff[i];
		}	
	}
	return false;
}

void inputdev_open(void *aux){
	idata *DB = aux;
	DB->gpio_fp = open("/dev/input/event1", O_RDONLY | O_NONBLOCK);
	DB->fpga_fp = open("/dev/fpga_push_switch", O_RDONLY | O_NONBLOCK);
}

void inputdev_close(void *aux){
	idata *DB = aux;
	close(DB->gpio_fp);
	close(DB->fpga_fp);
}


#ifdef inputdev_test
#endif
