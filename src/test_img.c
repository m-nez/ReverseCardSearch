#include "img.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>

int main(int argc , char** argv) {
	SDL_Surface* s = IMG_Load(argv[1]);
	printf("%d\n", s->format->format);
}
