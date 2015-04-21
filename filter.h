#ifndef FILTER_H
#define FILTER_H

void rec_filter(float* filter, int dim_x, int dim_y);
void apply_filter(int x_dim, int y_dim, float* image, float* filter);
void filter_test(int * results, float * elevation);
void mark_differences(int threshhold, float* image, float* baseline, int* out);
#endif
