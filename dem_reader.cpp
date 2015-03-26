#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "pnmio.h"


int main(int argc, char** argv)
{
	if (argc != 2) // first should be program name, second is _dem.raw file.
	{
		printf("Incorrect usage. %s RAW_FILE\n", argv[0]);
		return 1;
	}

	errno = 0;
	FILE* rawfile = fopen(argv[1], "rb"); // read binarily
	if (!rawfile)
	{
		printf("File open failed, error code %d: %s", errno, strerror(errno));
		return 1;
	}

	// Good! We've got the file. We could check if it's the right length,
	// but that's unnecessary at the moment.

	// WARNING: MAGIC NUMBERS AHEAD
	float elevation[250000];
	for (int row = 0; row < 500; row++) // there should be 500x500 points of data
	{
		fread(&elevation[row*500], 4, 500, rawfile);
	}

	write_float_img_pgm("dem_out.pgm", elevation, 500, 500);
}