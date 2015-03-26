#include <stdio.h>
#include <errno.h>
#include <string.h>

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
	// but that's unnecessary atm.

	// Black to white for highest bit? 10m high is the max.

	// WARNING: MAGIC NUMBERS AHEAD
	float max = -1.;
	float min = 100.;
	float florida[250000];
	unsigned char temp[4];
	for (int row = 0; row < 500; row++) // there should be 500x500 points of data
	{
		fread(&florida[row*500], 4, 500, rawfile);
	}

	write_float_img_pgm("dem_out.pgm", florida, 500, 500);
}