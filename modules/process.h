#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef process_h
	#define process_h
	#include "core/bitmap.h"
	#include "core/fork.h"
	#include "core/gameloop.h"
	#include "core/msgq.h"
	#include "modules/inputdev.h"
	#include "modules/outputdev.h"

	#define MODE_MAX 2
	#define FLAG_NUM 5

	struct FLAGS{
		// timer
		long long int c;
		int min;
		int sec;
		time_t start;
		bool ispause;
		bool paused_3_4;
		// text
		int typed;
		bool key[9];
		bool flag_a_1;
		//bool command_got;  // TODO
		int last_letter;
		int last_letter_times;
	};

	typedef void (*MODEFUNC)(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key);

	typedef struct _process_data{
		key_t msgq_key;
		int retval;
		struct bitmap *keys;
		struct FLAGS *flags;
		bool init;
		int mode;
		MODEFUNC modelist[MODE_MAX];
		MODEFUNC modeinitlist[MODE_MAX];
	}pdata;

	int process_main();  // to be forked.
	bool process_loop(void *aux);
	void process_init_flags(struct FLAGS **flags);


	#include "modules/stop_watch.h"
	#include "modules/text_editor.h"
#endif
