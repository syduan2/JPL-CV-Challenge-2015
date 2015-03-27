#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "pnmio.h"

#define ELEV_LEN 500
#define IMG_LEN 1000
#define BORDER 21

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
	for (int i = 0; i < IMG_LEN*IMG_LEN; i++) {results[i] = 1;}

	// a really simple catch for the first challenge
	/**
	 * So the program iterates over all the elevation bits, then if it's 
	 * greater than some threshold, it and all of its nearby pixels are marked
	 * black.
	 *
	 * The factors of two are because there are 500px along the elevation map,
	 * but 1000px along the output images
	 */
	for (int row = 0; row < ELEV_LEN; row++)
	{
		for (int column = 0; column < ELEV_LEN; column++)
		{
			// all of these are pretty much magic numbers we have to tune
			float radius = 0;
			if(elevation[row*ELEV_LEN+column] >= 18.78) 
				radius = 18.5;

			// color all pixels within radius of (column, row) black
			if (radius != 0)
			{
				for (int x_offset = -radius; x_offset <= radius; x_offset++)
				{
					for (int y_offset = -radius; y_offset <= radius; y_offset++)
					{
						int x = column*2 + x_offset; // yes, that's column.
						int y = row*2 + y_offset;
						if (x_offset*x_offset + y_offset*y_offset < radius*radius
							&& x >= 0 && x < IMG_LEN
							&& y >= 0 && y < IMG_LEN) 
						{
							results[y*IMG_LEN + x] = 0; 
							// Yes, y*IMG_LEN. Remeber that y = row
						}
					}
				}
			}
			// define something like a 21-pixel border around the image
			if (column*2 < BORDER || column*2 > IMG_LEN - BORDER
				|| row*2 < BORDER || row*2 > IMG_LEN - BORDER)
			{
				results[row*2*IMG_LEN + column*2] = 0;
				results[(row*2+1)*IMG_LEN + column*2] = 0;
				results[row*2*IMG_LEN + column*2 + 1] = 0;
				results[(row*2+1)*IMG_LEN + column*2 + 1] = 0;
			}
		}
	}

	write_int_img_pgm_bw(argv[3], results, 1000, 1000);
}