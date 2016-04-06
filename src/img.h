#pragma once

typedef struct {
	unsigned char* data;
	int w;
	int h;
} img_t;

/* These operation may be destructive to the data */

img_t* img_new(const char* fname);
void img_free(img_t* img);

void img_avg(img_t* img, unsigned char* val);
void img_avg_col_4(img_t* img, unsigned char* val);
void img_uint(img_t* img, unsigned int* val);

/* Corners should have enough space to contain 8 uints */
void img_find_corners(img_t* img, unsigned int x, unsigned int y, unsigned int* corners);

img_t* img_similar(img_t* img);
/* bounds should have 4 elements: left, right, up, down (left < right , up < down) */
img_t* img_sub(img_t* img, int* bounds);
img_t* img_dx(img_t* img);
img_t* img_dy(img_t* img);

/* Linear interpolation between 2 images,
 * f = 0 means only img1, f = 1 means only img2 */
img_t* img_mix(img_t* img1, img_t* img2, double f);

img_t* img_edge(img_t* img, unsigned int min_dist);

void img_flood_fill(img_t* img, unsigned int x, unsigned int y);
void img_find_bounds(img_t* img, int x, int y, int min_x, int min_y, int* bounds);
void img_fill_gaps(img_t* img, unsigned int radius);
void img_to_bmp(img_t* img, const char* fname);
