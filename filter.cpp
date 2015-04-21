#include <cmath>
#include <stdio.h>
#include <assert.h>
#include "filter.h"
#include "defs.h"
#include "pnmio.h"
#include <map>
#define FILTER_SIZE 17
using namespace std;
void rec_filter(float* filter, int dim){
  int i;
  for(i = 0; i < dim*dim; i++){
    filter[i] = 1.0/dim/dim;
  }
}
void gauss_filter(float* filter, int dim){
  float sigma = dim/3.0;
  float r, s = 2.0 * sigma * sigma;

  // sum is for normalization
  float sum = 0.0;

  // generate 5x5 kernel
  for (int x = -dim/2; x <= dim/2; x++)
  {
      for(int y = -dim/2; y <= dim/2; y++)
      {
          r = sqrt(x*x + y*y);
          filter[x + dim/2 + dim*(y + dim/2)] = (exp(-(r*r)/s))/(M_PI * s);
          sum += filter[x + dim/2 + dim*(y + dim/2)];
      }
  }

  // normalize the Kernel
  for(int i = 0; i < dim; ++i)
      for(int j = 0; j < dim; ++j)
          filter[i+j*dim] /= sum;
}
//dim is the width of the filter
void apply_filter(int dim, float* image, float* filter, float* out){
  int i, j, x, y;
  for(i = 0; i < IMG_LEN; i++){
    for(j = 0; j < IMG_LEN; j++){
      float sum=0;
      out[i + j*IMG_LEN] = 0;
      for(x = 0; x < dim; x++){
        for(y = 0; y < dim; y++){
          if(x + i > dim/2 && x + i < IMG_LEN + dim/2 &&
            y + j > dim/2 && y + j < IMG_LEN + dim/2){
            out[i + j*IMG_LEN] += image[i+x-dim/2 + (j+y-dim/2)*IMG_LEN] * filter[x + y*dim];
          }
        }
      }
    }
  }
}
void apply_mode_filter(int dim, float* image, int bins, float* out){
  int i, j, x, y;
  pair<float,int>* avg;
  for(i = 0; i < IMG_LEN; i++){
    for(j = 0; j < IMG_LEN; j++){
      float sum=0;
      map<int,pair<float, int> > frequencyCount;
      out[i + j*IMG_LEN] = 0;
      for(x = 0; x < dim; x++){
        for(y = 0; y < dim; y++){
          if(x + i > dim/2 && x + i < IMG_LEN + dim/2 &&
            y + j > dim/2 && y + j < IMG_LEN + dim/2){
              if(frequencyCount.find((int)image[i+x-dim/2 + (j+y-dim/2)*IMG_LEN]/100*bins)
                != frequencyCount.end()){
                  avg=&frequencyCount[(int)image[i+x-dim/2 + (j+y-dim/2)*IMG_LEN]/100*bins];
                  avg->first=(avg->first*avg->second + image[i+x-dim/2 + (j+y-dim/2)*IMG_LEN])/(avg->second+1);
                  avg->second++;
              }
              else{
                frequencyCount[(int)image[i+x-dim/2 + (j+y-dim/2)*IMG_LEN]/100*bins] =
                  pair<float, int>(image[i+x-dim/2 + (j+y-dim/2)*IMG_LEN],1);
              }
          }
        }
      }
      float currentMax = 0;
      unsigned arg_max = 0;
      unsigned count_max=0;
      for(map<int,pair<float, int> >::iterator it = frequencyCount.begin(); it != frequencyCount.end(); ++it ){
          if (it ->second.second > count_max) {
              count_max=it ->second.second;
              arg_max = it->first;
              currentMax = it->second.first;
          }
      }
      out[i + j*IMG_LEN]=currentMax;
    }
  }
}

// orig_len must be a factor of new_len
void expand_img(int orig_len, int new_len, float* old_img, float* new_img){
  int i, j, factor;
  factor=new_len/orig_len;
  for(i=0; i<new_len; i++){
    for(j=0; j<new_len; j++){
      new_img[i + j*new_len]=old_img[i/factor+j/factor*orig_len];
    }
  }
}
void mark_differences(float threshhold, int dim, float* image, float* baseline, int* out){
  //todo make it circular zzz
  int i, j, x, y;
  float img_val, base_val, difference;
  for(i = 0; i < IMG_LEN * IMG_LEN; i++) out[i]=1;
  for(i = 0; i < IMG_LEN; i++){
    for(j = 0; j < IMG_LEN; j++){
      for(x = 0; x < dim; x++){
        for(y = 0; y < dim; y++){
          if(x + i > dim/2 && x + i < IMG_LEN + dim/2 &&
            y + j > dim/2 && y + j < IMG_LEN + dim/2){
              img_val = image[i+x-dim/2 + (j+y-dim/2)*IMG_LEN];
              base_val = baseline[i+x-dim/2 + (j+y-dim/2)*IMG_LEN];
              difference = img_val - base_val;
              if(difference > threshhold || difference < -threshhold)
                out[x + y*IMG_LEN] = 0;
          }
        }
      }
    }
  }
}
void filter_test(int * results, float * elevation){
  float filter[FILTER_SIZE * FILTER_SIZE];
  float *avg_img=new float[IMG_LEN * IMG_LEN];
  float *expanded_elev=new float[IMG_LEN*IMG_LEN];
  float *out=new float[IMG_LEN*IMG_LEN];
  gauss_filter(filter, FILTER_SIZE);
  write_float_img_pgm("filter", filter, FILTER_SIZE, FILTER_SIZE);
  expand_img(ELEV_LEN, IMG_LEN, elevation, expanded_elev);
  apply_mode_filter(FILTER_SIZE, expanded_elev, 3, avg_img);
  for(int i=0;i<IMG_LEN*IMG_LEN;i++){
    out[i]=expanded_elev[i]-avg_img[i];
    if(out[i]<0) out[i]=0;
  }
  write_float_img_pgm("orig", expanded_elev, IMG_LEN, IMG_LEN);
  write_float_img_pgm("test_avg", avg_img, IMG_LEN, IMG_LEN);
  write_float_img_pgm("output", out, IMG_LEN, IMG_LEN);
  //mark_differences(.25, FILTER_SIZE, elevation, avg_img, results);
  delete avg_img;
  delete expanded_elev;
}
