#include "img.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>

img_t* img_new(const char* fname) {

	SDL_Surface* s = IMG_Load(fname);

	img_t* img = malloc(sizeof(img_t));
	img->w = s->w;
	img->h = s->h;
	img->data = malloc(img->w * img->h * 3); /* Only supports RGB imges */
	/*
	if (s->pitch == 3 * img->w) {
		memcpy(img->data, s->pixels, img->w * img->h * 3);
	} else { / Skip the alpha channel /
		int i, j;
		unsigned char* p = s->pixels;
		for(i = 0; i < img->w * img->h; ++i) {
			for(j = 0; j < 3; ++j) {
				img->data[i*3+j] = p[i*4+j];
			}
		}
	}
*/
	unsigned int pixel;
	for (int i = 0; i < s->w*s->h; ++i) {
		pixel = *((unsigned int*) ((char*) s->pixels + s->format->BytesPerPixel * i));
		SDL_GetRGB(		pixel,
						s->format,
						img->data + 3*i + 0, 
						img->data + 3*i + 1, 
						img->data + 3*i + 2);
		}

	SDL_FreeSurface(s);
	return img;
}

void img_free(img_t* img) {
	free(img->data);
	free(img);
}

void img_avg(img_t* img, unsigned char* val) {
	int i;
	int num_channels = 3;
	int count = img->w * img->h * num_channels;
	double avg = 0;
	for (i = 0; i < count; ++i) {
		avg += img->data[i];
	}
	*val = avg / count;
}

void img_avg_col_4(img_t* img, unsigned char* val) {
	int bounds[4][4] = {
		{0,			img->w/2,	0,			img->h/2},
		{img->w/2,	img->w,		0,			img->h/2},
		{0,			img->w/2,	img->h/2,	img->h},
		{img->w/2,	img->w,		img->h/2,	img->h}
	};
	int i, j, x, y;
	int avg[12] = {};
	for (i = 0; i < 4; ++i) {
		for (y = bounds[i][2]; y < bounds[i][3]; ++y) {
			for (x = bounds[i][0]; x < bounds[i][1]; ++x) {
				for(j = 0; j < 3; ++j) {
					avg[i*3+j] += img->data[3*(y*img->w + x) + j];
				}
			}
		}

		for(j = 0; j < 3; ++j) {
			avg[i*3+j] /= (bounds[i][1] - bounds[i][0]) * (bounds[i][3] - bounds[i][2]);
			val[i*3+j] = avg[i*3+j];
		}
	}
	
}

void img_uint(img_t* img, unsigned int* val) {
	double rgb[3] = {};
	unsigned char out[4] = {};
	int i, j;

	for (i = 0; i < img->w*img->h; ++i) {
		for (j = 0; j < 3; ++j) {
			rgb[j] += img->data[i*3+j];
		}
	}

	for (j = 0; j < 3; ++j) {
		out[j] = rgb[j] / (img->w*img->h);
	}

	*val = *(unsigned int*) out;
}



static inline double dist2(unsigned char* v1, unsigned char* v2) {
	double s = 0;
	int i = 0;
	for (i = 0; i < 3; ++i) {
		s += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}
	return s;
}

void img_find_corners(img_t* img, unsigned int x, unsigned int y, unsigned int* corners) {
	unsigned char avg_rgb[4];
	int i, j;
	unsigned char tmp;
	double m = -1;
	double t;
	img_uint(img, (unsigned int*) avg_rgb);
	printf("%d %d %d\n", 
			avg_rgb[0],
			avg_rgb[1],
			avg_rgb[2]
			);

	for(i = 0; i < img->w*img->h; ++i) {
		t = dist2(avg_rgb, &img->data[i*3]);
		t /= 3*255*255;
		t *= 255;
		if (t > m) {
			m = t;
		}
		tmp = t;
		for(j = 0; j <3; ++j) {
			img->data[i*3+j] = tmp;
		}
	}
	printf("%g\n", m);

}

img_t* img_similar(img_t* img) {
	img_t* s = malloc(sizeof(img_t));
	s->data = malloc(3*img->w*img->h);
	s->w = img->w;
	s->h = img->h;

	return s;
}


img_t* img_sub(img_t* img, int* bounds) {
	int i, j, k;
	img_t* s = malloc(sizeof(img_t));
	s->w = bounds[1] - bounds[0] + 1;
	s->h = bounds[3] - bounds[2] + 1;
	s->data = malloc(3 * s->w * s->h);
	for(i = 0; i <= (bounds[1] - bounds[0]); ++i) {
		for(j = 0; j <= (bounds[3] - bounds[2]); ++j) {
			for(k = 0; k < 3; ++k) {
				s->data[3*(j*s->w+i) + k] = img->data[3*((j + bounds[2])*img->w + bounds[0] + i) + k];
			}
		}
	}

	return s;
}

img_t* img_dx(img_t* img) {
	int i, j;
	img_t* imgd = img_similar(img);

	imgd->data[0] = 127;
	imgd->data[1] = 127;
	imgd->data[2] = 127;
	for(i = 1; i < img->w*img->h; ++i) {
		for(j = 0; j < 3; ++j) {
			imgd->data[i*3 + j] = (img->data[i*3+j] - img->data[(i-1)*3+j])/2 + 127;
		}
	}
	return imgd;
}

img_t* img_dy(img_t* img) {
	int i, j;
	img_t* imgd = img_similar(img);


	for(i = 0; i < img->w*3; ++i) {
		imgd->data[i] = 127;
	}

	for(i = img->w; i < img->w*img->h; ++i) {
		for(j = 0; j < 3; ++j) {
			imgd->data[i*3 + j] = (img->data[i*3+j] - img->data[(i-img->w)*3+j])/2 + 127;
		}
	}

	return imgd;
}

img_t* img_mix(img_t* img1, img_t* img2, double f) {
	img_t* img = img_similar(img1);
	int i;
	for(i = 0; i < img->w*img->h*3; ++i) {
		img->data[i] = img1->data[i] * (1 - f) + img2->data[i] * f;
	}
	return img;
}

double static inline sqr(unsigned char* v) {
	int i;
	double s = 0;
	for (i = 0; i < 3; ++i) {
		s += v[i] * v[i];
	}
	return s;
}

img_t* img_edge(img_t* img, unsigned int min_dist) {
	img_t* dx = img_dx(img);
	img_t* dy = img_dy(img);
	img_t* m = img_similar(img);
	int i, j;
	unsigned char avg[3] = {127, 127, 127};
	
	for(i = 0; i < m->w*img->h; ++i) {
		if (dist2(avg, dx->data + i * 3) > min_dist) {
			for(j = 0; j < 3; ++j) {
				m->data[i*3 + j] = ~0;
			}
		} else {
			for(j = 0; j < 3; ++j) {
				m->data[i*3 + j] = 0;
			}
		}
	}

	for(i = 0; i < m->w*img->h; ++i) {
		if (dist2(avg, dy->data + i * 3) > min_dist) {
			for(j = 0; j < 3; ++j) {
				m->data[i*3 + j] = ~0;
			}
		}
	}

	img_free(dx);
	img_free(dy);
	return m;
}

void img_find_bounds(img_t* img, int x, int y, int min_x, int min_y, int* bounds) {
	int i, j;
	int bound_index = x;
	int len = -img->w;

	for (i = x; i < img->w; ++i) {
		if (img->data[3*(img->w*y+i)] == 0) {
			len++;
		} else {
			if (len >= min_x) {
				break;
			} else {
				bound_index = i;
				len = 0;
			}
		}
	}
	bounds[1] = bound_index;

	bound_index = x;
	len = -img->w;
	for (i = x; i >= 0; --i) {
		if (img->data[3*(img->w*y+i)] == 0) {
			len++;
		} else {
			if (len >= min_x) {
				break;
			} else {
				bound_index = i;
				len = 0;
			}
		}
	}
	bounds[0] = bound_index;

	bound_index = y;
	len = -img->h;
	for (i = y; i >= 0; --i) {
		if (img->data[3*(img->w*i+x)] == 0) {
			len++;
		} else {
			if (len >= min_y) {
				break;
			} else {
				bound_index = i;
				len = 0;
			}
		}
	}
	bounds[2] = bound_index;

	bound_index = y;
	len = -img->h;
	for (i = y; i < img->h; ++i) {
		if (img->data[3*(img->w*i+x)] == 0) {
			len++;
		} else {
			if (len >= min_y) {
				break;
			} else {
				bound_index = i;
				len = 0;
			}
		}
	}
	bounds[3] = bound_index;

	for (i = bounds[0]; i < bounds[1]; ++i) {
		for (j = bounds[2]; j < bounds[3]; ++j) {
			img->data[3*(img->w*j+i) + 1] = 255;
		}
	}
}

void img_flood_fill(img_t* img, unsigned int x, unsigned int y) {
	unsigned int* que = malloc(sizeof(unsigned int) * 2 * 1000 * 1000);
	unsigned int len = 0;
	que[0] = x;
	que[1] = y;
	len = 1;

	unsigned int nx, ny, w, e;
	int i;

	while(len) {
		nx = que[(len-1)*2];
		ny = que[(len-1)*2 + 1];
		len--;

		for(w = nx; w >= 0; w--) {
			if (img->data[(ny*img->w + w)*3] != 0) {
				break;
			}
		}

		for(e = nx; e < img->w; e++) {
			if (img->data[(ny*img->w + e)*3] != 0) {
				break;
			}
		}

		for(i = w + 1; i < e; ++i) {
			img->data[(ny*img->w +i)*3] = 127;
			
			if (img->data[((ny+1)*img->w + i)*3] == 0) {
				que[len*2] = i;
				que[len*2 + 1] = ny + 1;
				len++;
			}
			if (img->data[((ny-1)*img->w + i)*3] == 0) {
				que[len*2] = i;
				que[len*2 + 1] = ny - 1;
				len++;
			}
		}
	}

	free(que);
}

void img_fill_gaps(img_t* img, unsigned int radius) {
	int i, j, k, l;
	char found;
	for(i = 0; i < img->h - radius; ++i) {
		for(j = 0; j < img->w - radius; ++j) {
			found = 0;
			for(k = 1; k <= radius; ++k) {
				for(l = 1; l <= radius; ++l) {
					if (img->data[((i+k)*img->w + j + l)*3] == 255) {
						found = 1;
						break;
					}
				}
				if (found) {
					break;
				}
			}
			if(found) {
				for(k = 0; k < 3; ++k) {
					img->data[(i*img->w + j)*3 + k] = 255; /* Set to white */
				}
			}
		}
	}
}

void img_to_bmp(img_t* img, const char* fname) {
	SDL_Surface* s = SDL_CreateRGBSurfaceFrom(
			img->data, img->w, img->h,24,img->w*3,
			0x0000ff,0x00ff00,0xff0000,0);
	SDL_SaveBMP(s, fname);
	SDL_FreeSurface(s);
}
