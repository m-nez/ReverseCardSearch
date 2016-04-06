#pragma once

#define VAL_AVG 0
#define VAL_UINT_RGB 1
#define VAL_AVG_COL_4 2

typedef struct {
	char* paths;
	void* vals;
	unsigned int val_type;
	unsigned int val_size;
	unsigned int path_len;
	unsigned int num_paths;
} map_t;

int write_data(const char* paths, const char* fname, unsigned char type);
map_t* map_data(const char* fname);
