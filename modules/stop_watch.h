#include <stdbool.h>

#ifndef stop_watch_h
	#define stop_watch_h
	#include "core/bitmap.h"
	#include "core/msgq.h"
	#include "modules/inputdev.h"
	#include "modules/process.h"

	void stop_watch(struct bitmap *keys, struct bitmap *flags, key_t msgq_key);
#endif
