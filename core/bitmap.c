#include "bitmap.h"

#ifdef bitmap_test
	int main(){
		// TODO Add tests.
		return 0;
	}
#endif

inline size_t byte_cnt(size_t bit_cnt){
	return sizeof(elem_type) * elem_cnt(bit_cnt);
}

inline size_t elem_cnt(size_t bit_cnt){
	return DIV_ROUND_UP(bit_cnt, ELEM_BITS);
}

inline size_t elem_idx (size_t bit_idx) 
{
	return bit_idx / ELEM_BITS;
}

inline elem_type bit_mask (size_t bit_idx) 
{
	return (elem_type) 1 << (bit_idx % ELEM_BITS);
}

struct bitmap * bitmap_create (size_t bit_cnt) 
{
	struct bitmap *b = malloc (sizeof *b);
	if (b != NULL)
	{
		b->bit_cnt = bit_cnt;
		b->bits = malloc (byte_cnt (bit_cnt));
		if (b->bits != NULL || bit_cnt == 0)
		{
			bitmap_set_all (b, false);
			return b;
		}
		free (b);
	}
	return NULL;
}
void bitmap_destroy (struct bitmap *b) 
{
	if (b != NULL) 
	{
		free (b->bits);
		free (b);
	}
}
size_t bitmap_size (const struct bitmap *b)
{
	return b->bit_cnt;
}
void bitmap_set (struct bitmap *b, size_t idx, bool value) 
{
	if (value)
		bitmap_mark (b, idx);
	else
		bitmap_reset (b, idx);
}
void bitmap_mark (struct bitmap *b, size_t bit_idx) 
{
	size_t idx = elem_idx (bit_idx);
	elem_type mask = bit_mask (bit_idx);
	b->bits[idx] |= mask;
}

bool bitmap_contains (const struct bitmap *b, size_t start, size_t cnt, bool value) 
{
	size_t i;

	for (i = 0; i < cnt; i++)
		if (bitmap_get (b, start + i) == value)
			return true;
	return false;
}

bool bitmap_any (const struct bitmap *b, size_t start, size_t cnt) 
{
	return bitmap_contains (b, start, cnt, true);
}
size_t bitmap_scan (const struct bitmap *b, size_t start, size_t cnt, bool value) 
{
	if (cnt <= b->bit_cnt) 
	{
		size_t last = b->bit_cnt - cnt;
		size_t i;
		for (i = start; i <= last; i++)
			if (!bitmap_contains (b, i, cnt, !value))
				return i; 
	}
	return BITMAP_ERROR;
}
void bitmap_set_all (struct bitmap *b, bool value) 
{
	bitmap_set_multiple (b, 0, bitmap_size (b), value);
}
void bitmap_set_multiple (struct bitmap *b, size_t start, size_t cnt, bool value) 
{
	size_t i;

	for (i = 0; i < cnt; i++)
		bitmap_set (b, start + i, value);
}
void bitmap_reset (struct bitmap *b, size_t bit_idx) 
{
	size_t idx = elem_idx (bit_idx);
	elem_type mask = bit_mask (bit_idx);
	b->bits[idx] &= ~mask;
}
bool bitmap_get (const struct bitmap *b, size_t idx) 
{
	return (b->bits[elem_idx (idx)] & bit_mask (idx)) != 0;
}
#ifdef bitmap_test
#endif
