#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef process_h
	#define process_h
	#include "core/bitmap.h"
	#include "core/fork.h"
	#include "core/gameloop.h"
	#include "core/msgq.h"
	#include "modules/inputdev.h"
	#include "modules/stop_watch.h"
	#include "modules/text_editor.h"

	typedef void (*MODEFUNC)(struct bitmap *keys, struct bitmap *flags, key_t msgq_key);

	#define MODE_MAX 2
	#define FLAG_NUM 5

	typedef struct _process_data{
		key_t msgq_key;
		int retval;
		struct bitmap *keys;
		struct bitmap *flags;
		bool init;
		int mode;
		MODEFUNC modelist[MODE_MAX];
	}pdata;

	int process_main();  // to be forked.
	static bool process_loop(void *aux);
	static void process_init_flags(struct bitmap **flags);


#endif
