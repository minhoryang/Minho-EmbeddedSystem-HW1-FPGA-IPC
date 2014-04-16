void text_editor_init(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key){
	flags->typed = 0;
	{
		int i;
		for(i=0;i<9;i++)
			flags->key[i] = false;
	}
	flags->flag_a_1 = true;
	flags->last_letter = -1;
	flags->last_letter_times = 0;
	{  // clean
		msg_pack i;
		i.mtype = MSG_TO_OUTPUT;
		i.mdata = OUT_CLEAN;
		i.mbool = true;
		msgq_send(msgq_key, &i);
	}
}
void text_editor(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key){
	{  //0->1 count
		int i, c=0;
		for(i=0;i<9;i++){
			if(!flags->key[i] && bitmap_get(keys, IN_BUTTON_FPGA_1 + i)){
				c++;
				flags->key[i] = true;
			}
		}
		if(c>0){
			// COUNTER print
			flags->typed += c;
			{  // 1000
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_7SEGMENTS_FPGA_1;
				i.mbool = true;
				i.mvalue = c / 1000;
				msgq_send(msgq_key, &i);
			}
			{  // 100
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_7SEGMENTS_FPGA_2;
				i.mbool = true;
				i.mvalue = (c / 100) % 10;
				msgq_send(msgq_key, &i);
			}
			{  // 10
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_7SEGMENTS_FPGA_3;
				i.mbool = true;
				i.mvalue = (c / 10) % 10;
				msgq_send(msgq_key, &i);
			}
			{  // 1
				msg_pack i;
				i.mtype = MSG_TO_OUTPUT;
				i.mdata = OUT_7SEGMENTS_FPGA_4;
				i.mbool = true;
				i.mvalue = c % 10;
				msgq_send(msgq_key, &i);
			}
		}
	}
	{  //1->0 wording
		bool pressed[9];
		int j, c=0;
		for(j=0;j<9;j++)
			pressed[j] = false;
		for(j=0;j<9;j++){
			if(flags->key[j] && !bitmap_get(keys, IN_BUTTON_FPGA_1 + j)){
				c++;
				flags->key[j] = false;
				pressed[j] = true;
			}
		}
		{  // pressed rules;
#define P(i) pressed[(i)-1]
			if(P(2) & P(3)){  // custom mode!
				// TODO
			}else
			if(P(4) & P(5)){  // clear + counter save.
				int b = flags->typed;
				text_editor_init(keys, flags, msgq_key);
				flags->typed = b;
			}else
			if(P(5) & P(6)){  // A<->1
				flags->flag_a_1 = !flags->flag_a_1;
				flags->last_letter = -1;
				flags->last_letter_times = 0;
				if(flags->flag_a_1){  // print A
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_DOTMATRIX_FPGA_A;
					i.mbool = true;
					msgq_send(msgq_key, &i);
				}else{  // print 1
					msg_pack i;
					i.mtype = MSG_TO_OUTPUT;
					i.mdata = OUT_7SEGMENTS_FPGA_1;
					i.mbool = true;
					msgq_send(msgq_key, &i);
				}
			}else
			if(P(8) & P(9)){  // shutdown
				// TODO
			}else
			if(c==1){  // 1 button
				if(flags->flag_a_1){  // A
					char letr[9][3] = {{'.', 'Q', 'Z'}, {'A', 'B', 'C'}, {'D', 'E', 'F'}, {'G', 'H', 'I'},
						{'J', 'K', 'L'}, {'M', 'N', 'O'}, {'P', 'R', 'S'}, {'T', 'U', 'V'}, {'W', 'X', 'Y'}};
					for(j=0;j<9;j++){
						if(pressed[j]){  // j
							if(flags->last_letter == j){  // jj
								if(flags->last_letter_times == 3){
									flags->last_letter_times = 1;
									{
										msg_pack i;
										i.mtype = MSG_TO_OUTPUT;
										i.mdata = OUT_LCD_FPGA_CHAR;
										i.mbool = false; // XXX no overwrite;
										i.mvalue = letr[j][flags->last_letter_times - 1];
										msgq_send(msgq_key, &i);
									}
								}else{
									flags->last_letter_times++;
									{
										msg_pack i;
										i.mtype = MSG_TO_OUTPUT;
										i.mdata = OUT_LCD_FPGA_CHAR;
										i.mbool = true; // XXX overwrite;
										i.mvalue = letr[j][flags->last_letter_times - 1];
										msgq_send(msgq_key, &i);
									}
								}
							}else{
								flags->last_letter = j;
								flags->last_letter_times = 1;
								{
									msg_pack i;
									i.mtype = MSG_TO_OUTPUT;
									i.mdata = OUT_LCD_FPGA_CHAR;
									i.mbool = false; // XXX no overwrite;
									i.mvalue = letr[j][flags->last_letter_times - 1];
									msgq_send(msgq_key, &i);
								}
							}
							break;
						}
					}
				}else
				if(!flags->flag_a_1){  // 1
					for(j=0;j<9;j++){
						if(pressed[j]){
							msg_pack i;
							i.mtype = MSG_TO_OUTPUT;
							i.mdata = OUT_LCD_FPGA_CHAR;
							i.mbool = false; // XXX overwrite;
							i.mvalue = '0' + j;
							msgq_send(msgq_key, &i);
							break;
						}
					}
				}
			}
		}
	}
}
