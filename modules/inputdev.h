#ifndef inputdev_h
	#define inputdev_h
	#include "core/bitmap.h"

	#define KEYS_NUM 4
	#define IN_SWITCH_GPIO_SELECT 0 
	#define IN_SWITCH_GPIO_UP 1
	#define IN_SWITCH_GPIO_DOWN 2 
	void input_init_keys(struct bitmap **keys);

#endif
