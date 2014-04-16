#include "modules/inputdev.h"

#ifdef input_test
	#include "core/bitmap.c"
	int main(){
		// TODO add test.
		return 0;
	}
#endif

void input_init_keys(struct bitmap **keys){
	// XXX Twin with process_init_flags();
	if (*keys)
		bitmap_destroy(*keys);
	*keys = bitmap_create(KEYS_NUM); // TODO num of keys
}

#ifdef input_test
#endif
