#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "pnmio.h"

#define ELEV_LEN 500
#define VIS_LEN 1000
#define BORDER 21

void print_usage(char** argv)
{
	printf("Incorrect usage. %s VISUAL ELEVATION OUTPUT\n", argv[0]);
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
	int results[VIS_LEN*VIS_LEN]; 
	for (int i = 0; i < VIS_LEN*VIS_LEN; i++) {results[i] = 1;}

	// a really simple catch for the first challenge
	// OH GOD SO MANY FOR-LOOPS.
	for (int row = 0; row < ELEV_LEN; row++)
	{
		for (int column = 0; column < ELEV_LEN; column++)
		{
			// TODO: fix the ordering, I think.
			if (column*2 < BORDER || column*2 > VIS_LEN - BORDER
				|| row*2 < BORDER || row*2 > VIS_LEN - BORDER)
			{
				results[row*2*VIS_LEN + column*2] = 0;
				results[(row*2+1)*VIS_LEN + column*2] = 0;
				results[row*2*VIS_LEN + column*2 + 1] = 0;
				results[(row*2+1)*VIS_LEN + column*2 + 1] = 0;
				continue;
			}
			// the radius of the lander is about 1.7m 
			// (17 pix in the regular map)
			if(elevation[row*ELEV_LEN+column] >= 19.1) // tune this
			{
				for (int x_offset = -17; x_offset <= 17; x_offset++)
				{
					for (int y_offset = -17; y_offset <= 17; y_offset++)
					{
						int x = column*2 + x_offset; // yes, that's column.
						int y = row*2 + y_offset;
						if (x_offset*x_offset + y_offset*y_offset < 17*17
							&& x >= 0 && x < VIS_LEN
							&& y >= 0 && y < VIS_LEN) 
						{
							results[y*VIS_LEN + x] = 0; 
							// Yes, this is the right order. Remeber that y = row
						}
					}
				}
			}
		}
	}

	write_int_img_pgm_bw(argv[3], results, 1000, 1000);
}