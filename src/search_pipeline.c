#include "search_pipeline.h"
#include "data.h"
#include "img.h"
#include "compare.h"
#include <stdlib.h>

img_t* get_sub_img(img_t* img, int x, int y) {
	int bounds[4];
	img_t* edge = img_edge(img, 3*6*6);
	img_fill_gaps(edge, 3);
	img_find_bounds(edge, x, y, 50, 50, bounds);
	img_t* sub = img_sub(img, bounds);

	img_to_bmp(sub, "/tmp/sub");
	return sub;
}

char* search(char** argv) {
	map_t* map =	map_data(argv[1]);
	img_t* img =	img_new(argv[2]);
	int x =			atoi(argv[3]);
	int y =			atoi(argv[4]);
	img_t* sub =	get_sub_img(img, x, y);
	void (*func) (img_t*, void*);
	int (*cmp_func) (unsigned char*, unsigned char*);

	switch (map->val_type) {
		case VAL_AVG:
			func = img_avg;
			cmp_func = NULL;
			break;
		case VAL_UINT_RGB:
			func = img_uint;
			cmp_func = dist2;
		case VAL_AVG_COL_4:
			func = img_avg_col_4;
			cmp_func = dist2_quad;
	}
	unsigned char* val = malloc(map->val_size);
	func(sub, val);
	int index;
	int min = 0xFFFFFF;
	int tmp;
	for (int i = 0; i < map->num_paths; ++i) {
		tmp = cmp_func(val, (unsigned char*) map->vals+map->val_size*i);
		if (tmp < min) {
			min = tmp;
			index = i;
		}
	}

	return map->paths + map->path_len*index;
}
