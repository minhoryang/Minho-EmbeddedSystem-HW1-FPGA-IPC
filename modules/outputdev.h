#ifndef outputdev_h
	#define outputdev_h
	#include "core/msgq.h"
	#include "core/gameloop.h"

	#define DISP_NUM 17
	#define OUT_CLEAN 0
	#define OUT_7SEGMENTS_GPIO_1 1
	#define OUT_7SEGMENTS_GPIO_2 2
	#define OUT_7SEGMENTS_GPIO_3 3
	#define OUT_7SEGMENTS_GPIO_4 4
	#define OUT_LED_GPIO_1 5
	#define OUT_LED_GPIO_2 6
	#define OUT_LED_GPIO_3 7
	#define OUT_LED_GPIO_4 8
	#define OUT_7SEGMENTS_FPGA_1 9
	#define OUT_7SEGMENTS_FPGA_2 10
	#define OUT_7SEGMENTS_FPGA_3 11
	#define OUT_7SEGMENTS_FPGA_4 12
	#define OUT_DOTMATRIX_FPGA_A 13  // TODO2
	#define OUT_DOTMATRIX_FPGA_1 14  // TODO2
	#define OUT_LCD_FPGA_CHAR 15
	#define OUT_LCD_FPGA_CLEAN 16

	typedef struct _outputdef_data{
		key_t msgq_key;
		// OUT BUFFER
		int counter;
		char text[41];  // TODO
		bool dotmatrix_a_1;  // TODO2
	}odata;

	int outputdev_main();
	bool outputdev_loop(void *aux);
	void outputdev_open();
	void outputdev_close();
#endif
