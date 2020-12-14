/*
 * reference LICENSE file provided.
 * 
 * main.c
 * Rudimentary application to test convolution.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>

#include <vec.h>
#include <vecOps.h>
#include <dspConv.h>
#include <dspFourier.h>
#include <dspFilter.h>
#include <libMath.h>

int main()
{
  dvec_o runningAverageFilter = new_dvec(10);
  int32_t _n = 0;
  for (_n = 0; _n < runningAverageFilter.len; _n++)
  {
    runningAverageFilter.arr[_n] =
      1.0 / (double)(runningAverageFilter.len);
  }

  dvec_o testSignal = new_dvec(1000);
  int32_t _j = 0;
  for (_j = 0; _j < testSignal.len; _j++)
  {
    testSignal.arr[_j] = 
      fabs((double)_j - ((double)testSignal.len / 2.0));
  }

  dvec_o convSignal = conv(testSignal, runningAverageFilter);

  FILE* f_testSignal = fopen("testSignal.csv", "w");
  FILE* f_convSignal = fopen("convSignal.csv", "w");

  int32_t _k = 0;
  for (_k = 0; _k < testSignal.len; _k++)
  {
    fprintf(f_testSignal, "%d, %lf\n", _k, testSignal.arr[_k]);
  }

  int32_t _l = 0;
  for (_l = 0; _l < convSignal.len; _l++)
  {
    fprintf(f_convSignal, "%d, %lf\n", _l, convSignal.arr[_l]);
  }

  fclose(f_testSignal);
  fclose(f_convSignal);

  del_dvec(runningAverageFilter);
  del_dvec(testSignal);
  del_dvec(convSignal);
}

