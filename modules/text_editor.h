#ifndef text_editor_h
	#define text_editor_h

	void text_editor_init(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key);
	void text_editor(struct bitmap *keys, struct FLAGS *flags, key_t msgq_key);
#endif
