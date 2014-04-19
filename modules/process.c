#include "modules/process.h"

#ifdef process_test
	#include <assert.h>
	#include "core/bitmap.c"
	#include "core/fork.c"
	#include "core/gameloop.c"
	#include "core/msgq.c"
	#include "modules/inputdev.c"

	int main(){
		process_main();
		return 0;
	}
#endif

int process_main(){  // to be forked.
	// data.
	pdata DB;
	printf("process\n");
	DB.msgq_key = msgq_open("process2");
	DB.init = false;
	DB.keys = NULL;
	DB.flags = NULL;
	input_init_keys(&(DB.keys));
	process_init_flags(&(DB.flags));

	DB.mode = 0;
	DB.modelist[0] = stop_watch;
	DB.modelist[1] = text_editor;
	DB.modeinitlist[0] = stop_watch_init;
	DB.modeinitlist[1] = text_editor_init;
	DB.modeinitlist[DB.mode](DB.keys, DB.flags, DB.msgq_key);

	// loop:
	gameloop(process_loop, &DB, PROCESS_FPS);

	// ~data.
	msgq_close(DB.msgq_key);
	return DB.retval;
}

bool process_loop(void *aux){
	pdata *DB = aux;
	//printf("1. get key & insert to bitmap;\n");
	{
		msg_pack *key = calloc(1, sizeof(msg_pack));
		if(msgq_recv(DB->msgq_key, MSG_TO_PROCESS, &key) != -1){
			printf("get key %d\n", key->mdata);
			bitmap_set(DB->keys, key->mdata, key->mbool);
		}
	}
	//printf("2. mode switch;\n");
	{
		// 0->1 right away.
		if(bitmap_get(DB->keys, IN_SWITCH_GPIO_SELECT)){
			// TODO ~init.
			{  // OUTPUT.
				msg_pack t;
				t.mtype = MSG_TO_OUTPUT;
				t.mdata = OUT_EXIT;
				msgq_send(DB->msgq_key, &t);
			}
			sleep(2);
			return true;
		}else if(bitmap_get(DB->keys, IN_SWITCH_GPIO_UP)){
			if(++(DB->mode) == MODE_MAX)
				DB->mode = 0;
			DB->init = true;
		}else if(bitmap_get(DB->keys, IN_SWITCH_GPIO_DOWN)){
			if(--(DB->mode) == -1)
				DB->mode = MODE_MAX - 1;
			DB->init = true;
		}
	}
	// 3. initialize variables
	{
		if(DB->init){
			input_init_keys(&(DB->keys));
			process_init_flags(&(DB->flags));
			DB->modeinitlist[DB->mode](DB->keys, DB->flags, DB->msgq_key);
			DB->init = false;
		}
	}
	// 4. mode(key, flags);
	DB->modelist[DB->mode](DB->keys, DB->flags, DB->msgq_key);
	return false;
}

void process_init_flags(struct FLAGS **flags){
	// XXX Twin with input_init_keys();
	if (*flags)
		free(*flags);
	*flags = (struct FLAGS *)calloc(1, sizeof(struct FLAGS));
}

#ifdef process_test
#endif

#include "modules/stop_watch.c"
#include "modules/text_editor.c"
