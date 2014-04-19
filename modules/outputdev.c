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
	printf("outputdev\n");
	DB.msgq_key = msgq_open("process2");
	outputdev_open(&DB);

	gameloop(outputdev_loop, &DB, 300);

	outputdev_close(&DB);
	msgq_close(DB.msgq_key);
	return 0;
}

bool outputdev_loop(void *aux){
	odata *DB = aux;
	char FPGA_1_A[3][10] = {
		{0x0c,0x1c,0x1c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x1e},
		{0x3E,0x7F,0x63,0x63,0x63,0x7F,0x7F,0x63,0x63,0x63},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
	};
	unsigned long fnd_num[11] = {0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0xC1, 0x1F, 0x01, 0x09, 0x00};

	++DB->c;
	// get request
	{
		msg_pack *key = calloc(1, sizeof(msg_pack));
		if(msgq_recv(DB->msgq_key, MSG_TO_OUTPUT, &key) != -1){
			printf("output %d\n", key->mdata);
			switch(key->mdata){
				case OUT_CLEAN:
					// TODO
					DB->seg7[0] = 0;
					DB->seg7[1] = 0;
					DB->seg7[2] = 0;
					DB->seg7[3] = 0;  // TODO
					DB->led4[0] = false;
					DB->led4[1] = false;
					DB->led4[2] = false;
					DB->led4[3] = false;
					//*DB->led_dat = 0x00;
					*DB->led_con |= 0x11110000;
					DB->data[0] = 0;
					DB->data[1] = 0;
					DB->data[2] = 0;
					DB->data[3] = 0;
					write(DB->fnd_dev, &DB->data, 4);
					DB->dotmatrix = FPGA_1_A[2];
					write(DB->fpga_dot, DB->dotmatrix, DB->str_size);
					memset(DB->string, ' ', sizeof(DB->string));
					DB->stringlen = 0;
					write(DB->text_dev, DB->string, 32);
					break;
				case OUT_7SEGMENTS_GPIO_1:
					DB->seg7[0] = key->mvalue;
					break;
				case OUT_7SEGMENTS_GPIO_2:
					DB->seg7[1] = key->mvalue;
					break;
				case OUT_7SEGMENTS_GPIO_3:
					DB->seg7[2] = key->mvalue;
					break;
				case OUT_7SEGMENTS_GPIO_4:
					DB->seg7[3] = key->mvalue;
					break;
				case OUT_LED_GPIO_1:
					DB->led4[0] = key->mbool;
					break;
				case OUT_LED_GPIO_2:
					DB->led4[1] = key->mbool;
					break;
				case OUT_LED_GPIO_3:
					DB->led4[2] = key->mbool;
					break;
				case OUT_LED_GPIO_4:
					DB->led4[3] = key->mbool;
					break;

				case OUT_7SEGMENTS_FPGA_1:
					DB->data[0] = key->mvalue + '0';
					write(DB->fnd_dev, &DB->data, 4);
					break;
				case OUT_7SEGMENTS_FPGA_2:
					DB->data[1] = key->mvalue + '0';
					write(DB->fnd_dev, &DB->data, 4);
					break;
				case OUT_7SEGMENTS_FPGA_3:
					DB->data[2] = key->mvalue + '0';
					write(DB->fnd_dev, &DB->data, 4);
					break;
				case OUT_7SEGMENTS_FPGA_4:
					DB->data[3] = key->mvalue + '0';
					write(DB->fnd_dev, &DB->data, 4);
					break;
				case OUT_DOTMATRIX_FPGA_A:
					DB->dotmatrix = FPGA_1_A[1];
					write(DB->fpga_dot, DB->dotmatrix, DB->str_size);
					break;
				case OUT_DOTMATRIX_FPGA_1:
					DB->dotmatrix = FPGA_1_A[0];
					write(DB->fpga_dot, DB->dotmatrix, DB->str_size);
					break;
				case OUT_LCD_FPGA_CHAR:
					{
						if(key->mbool){
							if(DB->stringlen<32){
								DB->string[DB->stringlen++] = key->mvalue;
							}else{  // push
								int i;
								for(i=1;i<32;i++){
									DB->string[i-1] = DB->string[i];
								}
								DB->string[31] = key->mvalue;
							}
						}else{  // overwrite
							DB->string[DB->stringlen-1] = key->mvalue;
						}
					}
					write(DB->text_dev, DB->string, 32);
					break;
			}
		}
	}
	{  // print
		{  // gpio
			{  // 7segment;
				switch(DB->c%4){
					case 0:
						*DB->gpe_dat = 0x02;
						*DB->gpl_dat = fnd_num[DB->seg7[0]];
						break;
					case 1:
						*DB->gpe_dat = 0x04;
						*DB->gpl_dat = fnd_num[DB->seg7[1]];
						break;
					case 2:
						*DB->gpe_dat = 0x10;
						*DB->gpl_dat = fnd_num[DB->seg7[2]];
						break;
					case 3:
						*DB->gpe_dat = 0x80;
						*DB->gpl_dat = fnd_num[DB->seg7[3]];
						break;
				}
			}
			{  // LED
				int t = 0xF0;
				if(DB->led4[0])
					t -= 0x10;  //000010000;
				if(DB->led4[1])
					t -= 0x20;  // 000100000;
				if(DB->led4[2])
					t -= 0x40;  // 001000000;
				if(DB->led4[3])
					t -= 0x80;  //010000000;
				*DB->led_dat = t;
			}
		}
	}
	return false;
}

void outputdev_open(void *aux){
	odata *DB = aux;
	DB->c = 0;
	{  // GPIO
		if((DB->fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
			printf("/dev/mem open error");

		DB->gpl_addr = (unsigned long *)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, DB->fd, IO_GPL_BASE_ADDR);
		if(DB->gpl_addr != NULL){
			DB->gpl_con = (unsigned long *)(DB->gpl_addr + FND_GPL2CON);
			DB->gpl_dat = (unsigned long *)(DB->gpl_addr + FND_GPL2DAT);
		}
		if(*DB->gpl_con == (unsigned long)-1 || *DB->gpl_dat == (unsigned long)-1)
			printf("mmap error");

		DB->gpe_addr = (unsigned long *)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, DB->fd, IO_GPE_BASE_ADDR);
		if(DB->gpe_addr != NULL){
			DB->gpe_con = (unsigned long *)(DB->gpe_addr + FND_GPE3CON);
			DB->gpe_dat = (unsigned long *)(DB->gpe_addr + FND_GPE3DAT);
		}
		if(*DB->gpe_con == (unsigned long)-1 || *DB->gpe_dat == (unsigned long)-1)
			printf("mmap error");

		// open and initialize LED driver
		DB->baseaddr = (unsigned long *)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, DB->fd, IO_BASE_ADDR);
		if(DB->baseaddr != NULL){
			DB->led_con = (unsigned long *)(DB->baseaddr + CON_OFFSET);
			DB->led_dat = (unsigned long *)(DB->baseaddr + DAT_OFFSET);
		}
		if(*DB->led_con == (unsigned long)-1 || *DB->led_dat == (unsigned long)-1)
			printf("mmap error");
		*DB->led_con |= 0x11110000;
	}
	{  // FPGA
		if((DB->fpga_dot = open("/dev/fpga_dot", O_WRONLY /*| O_NONBLOCK*/)) < 0)
			printf("/dev/fpga_dot open error");
		DB->str_size = 10;

		if((DB->fnd_dev = open("/dev/fpga_fnd", O_RDWR /*| O_NONBLOCK*/)) < 0)
			printf("/dev/fpga_fnd open error");
		memset(DB->data, 0, sizeof(DB->data));

		if((DB->text_dev = open("/dev/fpga_text_lcd", O_WRONLY /*| O_NONBLOCK*/)) < 0)
			printf("/dev/fpga_text_lcd open error");
		memset(DB->string, ' ', 32); //sizeof(DB->string));
	}
}

void outputdev_close(void *aux){
	odata *DB = aux;
	// GPIO
	{
		*DB->gpe_dat = 0x96;
		*DB->gpl_dat = 0x03;
		*DB->led_dat = 0xE0;
		munmap(DB->gpe_addr, 4096);
		munmap(DB->gpl_addr, 4096);
		close(DB->fd);
	}
	{ // FPGA
		char clear[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		write(DB->fpga_dot, clear, DB->str_size);
		int i;
		for(i=0;i<4;i++)
			DB->data[i] = '0';
		write(DB->fnd_dev, &DB->data, 4);
		for(i=0;i<32;i++)
			DB->string[i] = ' ';
		write(DB->text_dev, DB->string, 32);

		// close all device driver
		close(DB->fpga_dot);
		close(DB->fnd_dev);
		close(DB->text_dev);
	}
}
