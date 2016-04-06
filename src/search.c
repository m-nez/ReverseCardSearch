#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "img.h"
#include "compare.h"
#include "search_pipeline.h"

int main(int argc, char** argv) {
	if (argc < 5) {
		printf("Usage: %s <database> <image> <x> <y>\n", argv[0]);
		return 0;
	}

	printf("%s\n", search(argv));

	return 0;
}
