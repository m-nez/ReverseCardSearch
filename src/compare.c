#include "compare.h"
#include <stdlib.h>

int dist2(unsigned char* x, unsigned char* y) {
	int i;
	int s = 0;
	for(i = 0; i < 3; ++i) {
		s += ((int) x[i] - (int) y[i]) * ((int) x[i] - (int) y[i]);
	}
	return s;
}

int dist(unsigned char* x, unsigned char* y) {
	int i;
	int s = 0;
	for(i = 0; i < 3; ++i) {
		s += abs((int) x[i] - (int) y[i]);
	}
	return s;
}

double dot(unsigned char* x, unsigned char* y) {
	int i;
	int s = 0;
	for(i = 0; i < 3; ++i) {
		s += (int)x[i] * (int)y[i];
	}
	return s;
}

double mag2(unsigned char* x) {
	double s = 0;
	int i;
	for (i = 0; i< 3; ++i) {
		s += x[i] * x[i];
	}
	return s;
}

double cos2(unsigned char* x, unsigned char* y) {
	double c = dot(x, y);
	c *= c;
	c /= mag2(x);
	c /= mag2(y);
	return c;
}

int dist2_n(unsigned char* x, unsigned char* y, int n) {
	int i;
	int s = 0;
	for(i = 0; i < n; ++i) {
		s += ((int) x[i] - (int) y[i]) * ((int) x[i] - (int) y[i]);
	}
	return s;
}

int dist2_quad(unsigned char* x, unsigned char* y) {
	return dist2_n(x, y, 12);
}

int color_vec_quad(unsigned char* x, unsigned char* y) {
	int i;
	int s = 4 * 100000;
	double c;
	for (i = 0; i < 4; ++i) {
		c = cos2(x + 3*i, y + 3*i);
		c *= 100000;
		s -= c;
	}
	return s;
}
