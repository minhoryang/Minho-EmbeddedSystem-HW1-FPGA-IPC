#include <stdio.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#ifndef outputdev_h
	#define outputdev_h
	#include "core/msgq.h"
	#include "core/gameloop.h"

	#define DISP_NUM 17
	#define OUT_CLEAN 0  // mbool
	#define OUT_7SEGMENTS_GPIO_1 1  // mvalue
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
	#define OUT_LCD_FPGA_CLEAN 16  // TODO?

	#define IO_GPL_BASE_ADDR 0x11000000
	#define FND_GPL2CON 0x0100
	#define FND_GPL2DAT 0x0104

	#define IO_GPE_BASE_ADDR 0x11400000
	#define FND_GPE3CON 0x00140
	#define FND_GPE3DAT 0x00144

	#define IO_BASE_ADDR 0x11400000
	#define CON_OFFSET 0x40
	#define DAT_OFFSET 0x44

	typedef struct _outputdef_data{
		key_t msgq_key;
		long long int c;

		// OUT BUFFER
		bool dotmatrix_a_1;  // TODO2
		// gpio
		int fd;
		void *gpl_addr, *gpe_addr, *baseaddr;
		unsigned long *gpe_con;
		unsigned long *gpe_dat;
		unsigned long *gpl_con;
		unsigned long *gpl_dat;
		unsigned long *led_con;
		unsigned long *led_dat;

		int seg7[4];
		bool led4[4];
		// fpga
		int fpga_dot, str_size;
		int fnd_dev, digit_size;
		int text_dev, text_size, chk_size;
		unsigned char data[4];
		char string[33];
		char *dotmatrix;
		int stringlen;
	}odata;

	int outputdev_main();
	bool outputdev_loop(void *aux);
	void outputdev_open(void *aux);
	void outputdev_close(void *aux);
#endif
