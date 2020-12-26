/*
 * reference LICENSE file provided.
 * 
 * main.c
 * Quick test application to test FFT.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>

#include <math/vec.h>
#include <math/vecOps.h>
#include <math/libMath.h>
#include <dsp/dspConv.h>
#include <dsp/dspFourier.h>
#include <dsp/dspFilter.h>

int main()
{
  int32_t _n = 0;
  cvec_o testSignal = new_cvec(60000);
  for (_n = 0; _n < testSignal.len; _n++)
  {
    testSignal.arr[_n] = sin(0.5 * (double)_n);
  }

  cvec_o testFft = fft(testSignal);

  cvec_o testIfft = fft(testFft);

  FILE* f_testSignal = fopen("testSignal.csv", "w");
  FILE* f_testFft = fopen("testFft.csv", "w");
  FILE* f_testIfft = fopen("testIfft.csv", "w");

  for (_n = 0; _n < testSignal.len; _n++)
  {
    fprintf(f_testSignal, "%d, %lf + j%lf\n",
      _n, creal(testSignal.arr[_n]), cimag(testSignal.arr[_n]));
  }

  for (_n = 0; _n < testFft.len; _n++)
  {
    fprintf(f_testFft, "%d, %lf\n", _n, cmag(testFft.arr[_n]));
  }

  for (_n = 0; _n < testIfft.len; _n++)
  {
    fprintf(f_testIfft, "%d, %lf + j%lf\n",
      _n, creal(testIfft.arr[_n]), cimag(testIfft.arr[_n]));
  }

  return 0;
}

