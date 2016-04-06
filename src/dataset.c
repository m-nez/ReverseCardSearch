#include "data.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc < 4) {
		printf("Usage: %s <paths> <database> <type>\n", argv[0]);
		printf("\tpaths - path to a file containing paths to images\n");
		printf("\t\tPaths file contains a line with the number of paths,\n");
		printf("\t\tthe length of the longest path\n");
		printf("\t\tand as many paths as stated in the firs line.\n");
		printf("\tdatabase - output database\n");
		printf("\ttype - integer <0, 2>\n");
		printf("\t\t0 - Average of all channels (1 byte)\n");
		printf("\t\t1 - Average color of all pixels (4 bytes)\n");
		printf("\t\t2 - Average color of all pixels in 4 quadrants (12 bytes)\n");
		return 1;
	}
	
	write_data(argv[1], argv[2], atoi(argv[3]));
	return 0;
}
