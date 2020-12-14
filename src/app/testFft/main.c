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

#include <vec.h>
#include <vecOps.h>
#include <dspConv.h>
#include <dspFourier.h>
#include <dspFilter.h>
#include <libMath.h>

double cmag(double complex arg)
{
  return sqrt(creal(arg) * creal(arg) + cimag(arg) * cimag(arg));
}

int main()
{
  cvec_o testSignal = new_cvec(2048);
  int32_t _n = 0;
  for (_n = 0; _n < testSignal.len; _n++)
  {
    testSignal.arr[_n] = sin(0.1 * (double)_n);
  }

  cvec_o testFft = fft(testSignal);

  FILE* f_testSignal = fopen("testSignal.csv", "w");
  FILE* f_testFft = fopen("testFft.csv", "w");

  for (_n = 0; _n < testSignal.len; _n++)
  {
    fprintf(f_testSignal, "%d, %lf + j%lf\n",
      _n, creal(testSignal.arr[_n]), cimag(testSignal.arr[_n]));
  }

  for (_n = 0; _n < testFft.len; _n++)
  {
    fprintf(f_testFft, "%d, %lf\n", _n, cmag(testFft.arr[_n]));
  } 

  return 0;
}

