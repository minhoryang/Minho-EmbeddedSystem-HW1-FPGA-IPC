void stop_watch_init(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key){
	flags->c = 0;
	flags->min = 0;
	flags->sec = 0;
	flags->start = (time_t)NULL;
	flags->ispause = false;
	flags->paused_3_4 = true;
	{  // clean
		msg_pack i;
		i.mtype = MSG_TO_OUTPUT;
		i.mdata = OUT_CLEAN;
		i.mbool = true;
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
		if(flags->c % 30){
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
			}
		}
	}
	switch(bitmap_scan(keys, 0, KEYS_NUM, false)){
		case BITMAP_ERROR:
		default:
			break;
		case IN_BUTTON_GPIO_2:  // RESET
			stop_watch_init(keys, flags, msgq_key);
			break;
		case IN_BUTTON_GPIO_3:  // PAUSE
			if(flags->ispause){
				time(&flags->start);
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
				flags->ispause = true;
				flags->start = (time_t)NULL;
			}
			break;
		case IN_BUTTON_GPIO_4:  // START
			time(&flags->start);
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
			break;
	}
	if(flags->start)
	{
		time_t now;
		time(&now);
		if(difftime(now, flags->start) >= 0.99){
			flags->sec++;
			if(flags->sec > 60){
				flags->sec = 0;
				flags->min++;
				{  // 10 sec
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_7SEGMENTS_GPIO_1;
					i.mvalue = flags->sec / 10;
					i.mbool = true;
					msgq_send(msgq_key, &i);
				}
				{  // 1 sec
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_7SEGMENTS_GPIO_2;
					i.mvalue = flags->sec % 10;
					i.mbool = true;
					msgq_send(msgq_key, &i);
				}
			}
			if(flags->min > 60){
				flags->min = 0;
				{  // 10 sec
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_7SEGMENTS_GPIO_3;
					i.mvalue = flags->min / 10;
					i.mbool = true;
					msgq_send(msgq_key, &i);
				}
				{  // 1 sec
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_7SEGMENTS_GPIO_4;
					i.mvalue = flags->min % 10;
					i.mbool = true;
					msgq_send(msgq_key, &i);
				}
			}
		}
	}
}
