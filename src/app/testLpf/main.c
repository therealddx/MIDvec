/*
 * reference LICENSE file provided.
 * 
 * main.c
 * Quick test application to test filters.
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
  const u_int64_t BUF_LEN = 65536;
  int32_t _n = 0;

  // low-freq signal: 0.5 rad/sample.
  //
  dvec_o testSignalLf = new_dvec(BUF_LEN);
  for (_n = 0; _n < testSignalLf.len; _n++)
  {
    testSignalLf.arr[_n] = sin(0.5 * (double)_n);
  }

  // high-freq signal: 1.0 rad/sample.
  //
  dvec_o testSignalHf = new_dvec(BUF_LEN);
  for (_n = 0; _n < testSignalHf.len; _n++)
  {
    testSignalHf.arr[_n] = sin(1.0 * (double)_n);
  }

  // sum of signals.
  //
  dvec_o testSignalSum = add_dvec(testSignalLf, testSignalHf);
  cvec_o c_testSignalSum = upcast_dvec(testSignalSum);

  // make an LPF.
  //
  dvec_o testLpf = generate_lowpass_filter(0.5, 0.51, 0.001);
  cvec_o c_testLpf = upcast_dvec(testLpf);

  // execute the LPF.
  //
  dvec_o testSignalSumFiltered = conv(testSignalSum, testLpf);
  cvec_o c_testSignalSumFiltered = upcast_dvec(testSignalSumFiltered);

  // FFT before / after filtering.
  //
  cvec_o beforeFft = fft(c_testSignalSum);
  cvec_o afterFft = fft(c_testSignalSumFiltered);

  // print reports.
  //
  FILE* f_beforeFft = fopen("beforeFft.csv", "w");
  FILE* f_afterFft = fopen("afterFft.csv", "w");

  for (_n = 0; _n < beforeFft.len; _n++)
  {
    fprintf(f_beforeFft, "%d, %lf\n", _n, cmag(beforeFft.arr[_n]));
  }

  for (_n = 0; _n < afterFft.len; _n++)
  {
    fprintf(f_afterFft, "%d, %lf\n", _n, cmag(afterFft.arr[_n]));
  }

  // delete.
  fclose(f_beforeFft);
  fclose(f_afterFft);

  del_dvec(testSignalLf);
  del_dvec(testSignalHf);

  del_dvec(testSignalSum);
  del_cvec(c_testSignalSum);

  del_dvec(testLpf);
  del_cvec(c_testLpf);

  del_dvec(testSignalSumFiltered);
  del_cvec(c_testSignalSumFiltered);

  del_cvec(beforeFft);
  del_cvec(afterFft);

  // ret.
  return 0;
}

