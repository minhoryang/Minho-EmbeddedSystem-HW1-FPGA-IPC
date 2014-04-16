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
	DB.msgq_key = msgq_open("process");
	DB.init = false;
	DB.keys = NULL;
	DB.flags = NULL;
	input_init_keys(&(DB.keys));
	process_init_flags(&(DB.flags));

	DB.mode = 0;
	DB.modelist[0] = stop_watch;
	DB.modelist[1] = NULL;

	// loop:
	gameloop(process_loop, &DB, 30);

	// ~data.
	msgq_close(DB.msgq_key);
	return DB.retval;
}

static bool process_loop(void *aux){
	pdata *DB = aux;
	// 1. get key & insert to bitmap;
	{
		int key;
		if(msgq_recv(DB->msgq_key, MSG_TO_PROCESS, &key) != -1)  // TODO replace &key, &press
			bitmap_mark(DB->keys, key);  // TODO replace bitmap_set( * , * , * )
	}
	// 2. mode switch;
	{
		// 0->1 right away.
		if(bitmap_get(DB->keys, IN_SWITCH_GPIO_SELECT)){
			// TODO ~init.
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
			DB->init = false;
		}
	}
	// 4. mode(key, flags);
	DB->modelist[DB->mode](DB->keys, DB->flags, DB->msgq_key);
	return false;
}

static void process_init_flags(struct bitmap **flags){
	// XXX Twin with input_init_keys();
	if (*flags)
		bitmap_destroy(*flags);
	*flags = bitmap_create(FLAG_NUM);
}

#ifdef process_test
#endif

#include "modules/stop_watch.c"
#include "modules/text_editor.c"
