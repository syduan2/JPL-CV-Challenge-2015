#include <cmath>
#include <stdio.h>
#include <assert.h>

#include "bruteforce.h"
#include "defs.h"
#include "vec3d.h"

#define FOOT_SIDELEN_ELEV_PX (3)
#define BOT_RADIUS_ELEV_PX (8)
#define PI (3.14159265)
#define M_PER_ELEV_PX (0.2)

void clear_border(int * results)
{
	for (int y = 0; y < IMG_LEN; y++)
	{
		for (int x = 0; x < BORDER; x++)
		{
			results[y*IMG_LEN + x] = 0;
		}
		for (int x = IMG_LEN - BORDER; x < IMG_LEN; x++)
		{
			results[y*IMG_LEN + x] = 0;
		}
	}

	for (int x = BORDER; x < IMG_LEN-BORDER; x++)
	{
		for (int y = 0; y < BORDER; y++)
		{
			results[y*IMG_LEN + x] = 0;
		}
		for (int y = IMG_LEN - BORDER; y < IMG_LEN; y++)
		{
			results[y*IMG_LEN + x] = 0;
		}
	}
}

int roughness_test(int * results, float * elevation)
{
	/** A really simple catch for the first challenge
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
			clear_border(results);
		}
	}
	return 0;
}

vec3d get_vector(float* elevation, int x, int y)
{
	vec3d result;
	result.x = x*M_PER_ELEV_PX;
	result.y = y*M_PER_ELEV_PX;
	result.z = elevation[y*ELEV_LEN+x]*0.02; //tuning
	return result;
}

vec3d place_foot(float * elevation, int x, int y)
{
	// return the vector of the highest point in the range
	int radius = (FOOT_SIDELEN_ELEV_PX - 1)/2;
	vec3d result;
	result.z = -INFINITY;
	for (int x_offset = -radius; x_offset <= radius; x_offset++)
	{
		for (int y_offset = -radius; y_offset <= radius; y_offset++)
		{
			if (x==100 && y==100)
			{
				printf("x_offset=%d, y_offset=%d\n", x_offset, y_offset);
			}
			if (result.z < elevation[(y_offset+y)*ELEV_LEN+(x+x_offset)])
			{
				result = get_vector(elevation, x_offset+x, y_offset+y);
			}
		}
	}
	return result;
}

bool place_robot(float* elevation, int x, int y)
{
	// Can we place the robot there? True or false?

	// for each offset in a range of 30
	for (int i = 0; i < 15; i++)
	{
		// initialze vectors of foot placement
		vec3d feet[4];
		// for each direction (0, 90, 180, 270)
		for (int j = 0; j < 4; j++)
		{
			// get vectors of foot placement
			int x_offset = (int)(BOT_RADIUS_ELEV_PX*sin(j*PI/2 + i*PI/30));
			int y_offset = (int)(BOT_RADIUS_ELEV_PX*cos(j*PI/2 + i*PI/30));
			if (x == 100 && y == 100)
			{
				printf("x_offset = %d, y_offset = %d\n", x_offset, y_offset);
			}
			feet[j] = place_foot(elevation, x + x_offset, y + y_offset);
		}
		// for each combination of three vectors
		for (int j = 0; j < 4; j++)
		{
			// first, check if the slope is good
			// find the normal vector for everything but the selected one
			vec3d grounded[3];
			int grounded_index = 0;
			for (int k = 0; k < 4; k++)
			{
				if (k != j)
				{
					grounded[grounded_index++] = feet[k];
				}
			}
			vec3d norm = find_normal(grounded[1], grounded[2], grounded[0]);

			if (norm.dot(feet[j]-grounded[0])/norm.dot(norm) <= 0) // foot is higher than it can be
			{

			if (norm.tilt() > PI/18) return false;
			// second, check if the roughness pierces the bottom
			/*
			int radius = BOT_RADIUS_ELEV_PX;
			for (int x_offset = -radius; x_offset <= radius; x_offset++)
				{
				for (int y_offset = -radius; y_offset <= radius; y_offset++)
				{
					int x_total = x + x_offset; // yes, that's column.
					int y_total = y + y_offset;
					if (x_offset*x_offset + y_offset*y_offset < radius*radius)
						// make vector
						vec3d point = get_vector(elevation, x_total, y_total);
						// see how far it goes
						float roughness = norm.dot(point-grounded[0])/norm.magn();
						if (roughness > 2.0) // tuning
						{
							//fprintf(stderr, "%f\n", roughness);
							return false;
						}
					}
				}
			}*/
			}
		}
	}

	return true;
}

int bruteforce_test(int * results, float * elevation)
{
	clear_border(results);
	// for each point
	//for (int y = BORDER/2; y < ELEV_LEN-BORDER/2; y++)
	for (int y = BORDER/2; y < ELEV_LEN/4; y++)
	{
		printf("At y = %d\n", y);
		//for (int x = BORDER/2; x < ELEV_LEN-BORDER/2; x++)
		for (int x = BORDER/2; x < ELEV_LEN/4; x++)
		{
			int color = place_robot(elevation, x, y) ? 1 : 0;
			results[2*y*IMG_LEN+2*x] = color;
			results[2*y*IMG_LEN+IMG_LEN+2*x] = color;
			results[2*y*IMG_LEN+2*x+1] = color;
			results[2*y*IMG_LEN+2*x+1+IMG_LEN] = color;
		}
	}
}