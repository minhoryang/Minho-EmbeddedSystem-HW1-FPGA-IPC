#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#ifndef bitmap_h
	#define bitmap_h
	typedef unsigned long elem_type;
	struct bitmap{
		size_t bit_cnt;
		elem_type *bits;
	};
	#define ELEM_BITS (sizeof (elem_type) * CHAR_BIT)
	#define DIV_ROUND_UP(X, STEP) (((X) + (STEP) - 1) / (STEP))
	#define BITMAP_ERROR (SIZE_MAX)

	inline size_t byte_cnt(size_t bit_cnt);
	inline size_t elem_cnt(size_t bit_cnt);
	inline size_t elem_idx (size_t bit_idx) ;
	inline elem_type bit_mask (size_t bit_idx); 
	struct bitmap * bitmap_create (size_t bit_cnt) ;
	void bitmap_destroy (struct bitmap *b) ;
	size_t bitmap_size (const struct bitmap *b);
	void bitmap_set (struct bitmap *b, size_t idx, bool value) ;
	void bitmap_mark (struct bitmap *b, size_t bit_idx) ;
	bool bitmap_contains (const struct bitmap *b, size_t start, size_t cnt, bool value); 
	bool bitmap_any (const struct bitmap *b, size_t start, size_t cnt) ;
	size_t bitmap_scan (const struct bitmap *b, size_t start, size_t cnt, bool value); 
	void bitmap_set_all (struct bitmap *b, bool value) ;
	void bitmap_set_multiple (struct bitmap *b, size_t start, size_t cnt, bool value) ;
	void bitmap_reset (struct bitmap *b, size_t bit_idx) ;
	bool bitmap_get (const struct bitmap *b, size_t idx) ;
#endif
