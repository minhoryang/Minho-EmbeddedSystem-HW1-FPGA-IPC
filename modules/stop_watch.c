void stop_watch_init(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key){
	printf("hello stop_Watch\n");
	flags->c = 0;
	flags->min = 0;
	flags->sec = 0;
	flags->start = false;
	flags->start_c = 0;
	flags->ispause = false;
	flags->paused_3_4 = true;
	{  // clean
		msg_pack i;
		i.mtype = MSG_TO_OUTPUT;
		i.mdata = OUT_CLEAN;
		msgq_send(msgq_key, &i);
	}
	{  // turn on LED1
		msg_pack i;
		i.mtype = MSG_TO_OUTPUT;
		i.mdata = OUT_LED_GPIO_1;
		i.mbool = true;
		msgq_send(msgq_key, &i);
	}
}

void stop_watch(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key){
	++flags->c;
	if(flags->ispause){
		if(!(flags->c % PROCESS_FPS)){
			if(flags->paused_3_4){ // 3
				{  // 3 on
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_LED_GPIO_3;
					i.mbool = true;
					msgq_send(msgq_key, &i);
				}
				{  // 4 off
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_LED_GPIO_4;
					i.mbool = false;
					msgq_send(msgq_key, &i);
				}
				flags->paused_3_4 = false;
			}else{  // 4
				{  // 3 off
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_LED_GPIO_3;
					i.mbool = false;
					msgq_send(msgq_key, &i);
				}
				{  // 4 on
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_LED_GPIO_4;
					i.mbool = true;
					msgq_send(msgq_key, &i);
				}
				flags->paused_3_4 = true;
			}
		}
	}
	if(bitmap_get(keys, IN_BUTTON_GPIO_2)){  // RESET
		bitmap_set(keys, IN_BUTTON_GPIO_2, false);
		stop_watch_init(keys, flags, msgq_key);
	}else if(bitmap_get(keys, IN_BUTTON_GPIO_3)){  // PAUSE
		bitmap_set(keys, IN_BUTTON_GPIO_3, false);
		if(flags->ispause){
			flags->ispause = false;
			{  // 3 on
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_LED_GPIO_3;
				i.mbool = true;
				msgq_send(msgq_key, &i);
			}
			{  // 4 on
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_LED_GPIO_4;
				i.mbool = true;
				msgq_send(msgq_key, &i);
			}
		}else{
			if(flags->start){
				flags->ispause = true;
				flags->paused_3_4 = true;
			}
		}
	}else if(bitmap_get(keys, IN_BUTTON_GPIO_4)){  // START
		bitmap_set(keys, IN_BUTTON_GPIO_4, false);
		if(!flags->start){
			{  // 3 on
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_LED_GPIO_3;
				i.mbool = true;
				msgq_send(msgq_key, &i);
			}
			{  // 4 on
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_LED_GPIO_4;
				i.mbool = true;
				msgq_send(msgq_key, &i);
			}
			flags->ispause = false;
			flags->start = true;
		}
	}
	if(!flags->ispause && flags->start)
	{
		++flags->start_c;
		if(!(flags->start_c % PROCESS_FPS))
		{
			flags->sec++;
			if(flags->sec == 60){
				flags->sec = 0;
				flags->min++;
			}
			{  // 10 min
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_7SEGMENTS_GPIO_1;
				i.mvalue = flags->min / 10;
				i.mbool = true;
				msgq_send(msgq_key, &i);
			}
			{  // 1 min
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_7SEGMENTS_GPIO_2;
				i.mvalue = flags->min % 10;
				i.mbool = true;
				msgq_send(msgq_key, &i);
			}
			if(flags->min == 60){
				flags->min = 0;
			}
			{  // 10 sec
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_7SEGMENTS_GPIO_3;
				i.mvalue = flags->sec / 10;
				i.mbool = true;
				msgq_send(msgq_key, &i);
			}
			{  // 1 sec
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_7SEGMENTS_GPIO_4;
				i.mvalue = flags->sec % 10;
				i.mbool = true;
				msgq_send(msgq_key, &i);
			}
		}
	}
}
