#ifndef stop_watch_h
	#define stop_watch_h

	void stop_watch_init(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key);
	void stop_watch(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key);
#endif
