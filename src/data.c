#include "data.h"
#include "img.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int write_data(const char* paths, const char* fname, unsigned char type) {
	FILE* in = fopen(paths, "r");
	FILE* out = fopen(fname, "wb");

	char buff[FILENAME_MAX];
	
	unsigned int num_paths;
	unsigned int path_len;

	unsigned int type_size;
	void* val;
	void (*func) (img_t*, void*);
	img_t* img;

	int i;

	fscanf(in, "%u", &num_paths);
	fscanf(in, "%u", &path_len);
	path_len += 1;

	fwrite(&num_paths,	sizeof(unsigned int), 1, out);
	fwrite(&type,		sizeof(unsigned int), 1, out);
	fwrite(&path_len,	sizeof(unsigned int), 1, out);

	switch (type) {
		case VAL_AVG:
			type_size = 1;
			func = img_avg;
			break;
		case VAL_UINT_RGB:
			type_size = 4;
			func = img_uint;
		case VAL_AVG_COL_4:
			type_size = 12;
			func = img_avg_col_4;
	}

	val = malloc(type_size);

	/* Read the trailing newlinew after path len */
	fgets(buff, FILENAME_MAX, in);
	
	for (i = 0; i < num_paths; ++i) {
		fgets(buff, path_len + 2, in);
		buff[strlen(buff)-1] = 0;
		buff[path_len] = 0;
		printf("%s", buff);
		fwrite(buff, sizeof(char), path_len, out);
		img = img_new(buff);
		func(img, val);
		img_free(img);
		fwrite(val, type_size, 1, out);
	}

	fclose(in);
	fclose(out);

	return 0;
}

map_t* map_data(const char* fname) {
	FILE* in = fopen(fname, "rb");

	unsigned int num_paths;
	unsigned int val_type;
	unsigned int path_len;
	
	int i;

	fread(&num_paths, sizeof(unsigned int), 1, in);
	fread(&val_type, sizeof(unsigned int), 1, in);
	fread(&path_len, sizeof(unsigned int), 1, in);

	map_t* map = malloc(sizeof(map_t));
	map->paths = malloc(path_len * num_paths);
	map->path_len = path_len;
	map->val_type = val_type;
	map->num_paths = num_paths;
	switch (val_type) {
		case VAL_AVG:
			map->val_size = 1;
			break;
		case VAL_UINT_RGB:
			map->val_size = 4;
			break;
		case VAL_AVG_COL_4:
			map->val_size = 12;
			break;

	}

	map->vals = malloc(map->val_size * map->num_paths);
	for(i = 0; i < num_paths; ++i) {
		fread(map->paths + i*path_len, path_len, 1, in);
		fread(map->vals+i*map->val_size, map->val_size, 1, in);
		//printf("%s %d\n", map->paths + i*path_len, *((unsigned char*)map->vals+i*map->val_size));
	}

	return map;
}
