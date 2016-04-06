#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "img.h"
int main(int argc, char** argv) {
	if (argc < 5) {
		printf("Usage: %s <image> <database> <x> <y>\n", argv[0]);
		return 1;
	}

	int bounds[4];
	unsigned int x, y;
	x = atoi(argv[3]);
	y = atoi(argv[4]);
	
	img_t* img = img_new(argv[1]);
	map_t* map = map_data(argv[2]);

	img_t* edge = img_edge(img, 3*6*6);
	img_fill_gaps(edge, 3);
	img_find_bounds(edge, x, y, 50, 50, bounds);
	img_t* sub = img_sub(img, bounds);
	//img_flood_fill(edge, x, y);
	img_to_bmp(edge, "/tmp/edge");
	img_to_bmp(sub, "/tmp/sub");

	return 0;
}
