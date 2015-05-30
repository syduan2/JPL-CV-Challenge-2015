#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "pnmio.h"
#include "bruteforce.h"
#include "defs.h"
#include "filter.h"

void print_usage(char** argv)
{
	printf("Incorrect usage. %s IMAGE ELEVATION OUTPUT\n", argv[0]);
}

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		print_usage(argv);
		return 1;
	}

	FILE* visual_file = fopen(argv[1], "rb");
	if (!visual_file)
	{
		printf("Failed to open file \"%s\", error code %d: %s.\n",
			argv[1], errno, strerror(errno));
		print_usage(argv);
		return 1;
	}

	FILE* elevation_file = fopen(argv[2], "rb");
	if (!elevation_file)
	{
		printf("Failed to open file \"%s\", error code %d: %s.\n",
			argv[2], errno, strerror(errno));
		print_usage(argv);
		return 1;
	}

	// Good! We've got the files. We could check if it's the right length,
	// but that's unnecessary at the moment.

	// Read the elevation data into a float array called elevation
	float elevation[ELEV_LEN*ELEV_LEN];
	for (int row = 0; row < ELEV_LEN; row++) // there should be 500x500 points
	{
		fread(&elevation[row*ELEV_LEN], 4, ELEV_LEN, elevation_file);
	}

	// set the results array to zero
	int results[IMG_LEN*IMG_LEN];
	for (int i = 0; i < IMG_LEN*IMG_LEN; i++)
		results[i] = 1;

	//bruteforce_test(results, elevation);
	filter_test(results, elevation);
	write_int_img_pgm_bw(argv[3], results, 1000, 1000);
}
