#ifndef inputdev_h
	#define inputdev_h
	#include "core/bitmap.h"
	#include "core/msgq.h"
	#include "core/gameloop.h"

	#define KEYS_NUM 16
	#define IN_SWITCH_GPIO_SELECT 0 
	#define IN_SWITCH_GPIO_UP 1
	#define IN_SWITCH_GPIO_DOWN 2 
	#define IN_BUTTON_GPIO_1 3
	#define IN_BUTTON_GPIO_2 4
	#define IN_BUTTON_GPIO_3 5
	#define IN_BUTTON_GPIO_4 6
	#define IN_BUTTON_FPGA_1 7
	#define IN_BUTTON_FPGA_2 8
	#define IN_BUTTON_FPGA_3 9
	#define IN_BUTTON_FPGA_4 10
	#define IN_BUTTON_FPGA_5 11
	#define IN_BUTTON_FPGA_6 12
	#define IN_BUTTON_FPGA_7 13
	#define IN_BUTTON_FPGA_8 14
	#define IN_BUTTON_FPGA_9 15

	void input_init_keys(struct bitmap **keys);

	typedef struct _inputdev_data{
		key_t msgq_key;
	}idata;

	int inputdev_main();  // XXX to be forked
	bool inputdev_loop(void *aux);
	void inputdev_open();
	void inputdev_close();
#endif
