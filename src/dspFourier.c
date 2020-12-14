/*
 * reference LICENSE file provided.
 * 
 * dspFourier.c.
 * Implements dspFourier.h.
 * 
 */

#include "dspFourier.h"

cvec_o fft(cvec_o arg_xn)
{
  // vars.
  cvec_o sel_xn;
  u_int64_t newLength = 2;

  // arg check: ensure power-of-2 length.
  //  a. determine sufficient power-of-2 length.
  //  b. allocate that much space.
  //  c. copy content from arg_xn.
  //  d. zero-pad to remaining length.
  //  'arg_xn' is no longer relevant; use 'sel_xn'.
  //
  while (newLength < arg_xn.len) { newLength <<= 1; } // a.
  sel_xn = new_cvec(newLength); // b.

  u_int64_t  _n = 0; // c. + d.
  for (_n = 0; _n < arg_xn.len; _n++) { sel_xn.arr[_n] = arg_xn.arr[_n]; }
  for (_n = arg_xn.len; _n < newLength; _n++) { sel_xn.arr[_n] = 0; }

  // vars.
  cvec_o Xk = new_cvec(sel_xn.len);
  cvec_o Xkbuf = new_cvec(sel_xn.len);

  // initial loop: initialize X[k].
  //
  u_int64_t _k = 0;
  for (_k = 0; _k < (Xk.len >> 1); _k++)
  {
    Xk.arr[_k] =
      arg_xn.arr[_k] + arg_xn.arr[_k + (arg_xn.len >> 1)];

    Xk.arr[_k + (Xk.len >> 1)] =
      arg_xn.arr[_k] - arg_xn.arr[_k + (arg_xn.len >> 1)];
  }
  
  int32_t depth = (int)(log10((double)Xk.len) / log10(2.0)) - 1;
  int32_t y_int = 0;
  int32_t gap = Xk.len >> 1;
  int32_t inst_N = 4;
  int32_t m = 0;
  double complex tw = 0;
    int32_t tw_m = 0;
    double tw_p = 0;
  
  // printf("%d, %d, %d, %d, %d\r\n", depth, y_int, gap, inst_N, m);
  
  // run depth-count down.
  while (depth > 0)
  {
    // printf("Depth is %d\r\n", depth);

    for (y_int = 0; y_int < (gap >> 1); y_int++)
    {
      for (m = 0; m < (inst_N >> 1); m++)
      {
        tw_p = -2.0 * M_PI * (double)(tw_m) / (double)(inst_N);
	tw = cos(tw_p) + I * sin(tw_p);
        
        Xkbuf.arr[(gap >> 1) * m + y_int] =
          Xk.arr[gap * m + y_int] +
	    (tw * Xk.arr[gap * m + (gap >> 1) + y_int]);

        Xkbuf.arr[(gap >> 1) * m + y_int + (Xk.len >> 1)] =
          Xk.arr[gap * m + y_int] -
	    (tw * Xk.arr[gap * m + (gap >> 1) + y_int]);
        
        // printf("Storing to: %d, %d\r\n",
	//   (gap>>1)*m+y_int, (gap>>1)*m+y_int+(Xk.len>>1));
	//
        // printf("Reading from: %d, %d\r\n",
	//   gap*m+y_int, gap*m+(gap>>1)+y_int);
	//
        // printf("%d / %d\r\n", tw_m, inst_N);
	//
        
        tw_m++;
      } //printf("m loop done\r\n");
      tw_m = 0;
    } //printf("y_int loop done\r\n");
    
    Xkbuf = copy_cvec(Xk);
    
    depth--;
    inst_N <<= 1;
    gap >>= 1;
  }

  // scale down X[k] result.
  //
  int32_t _m = 0;
  double scaleDown = 1.0 / (double)(Xk.len);
  for (_m = 0; _m < Xk.len; _m++) { Xk.arr[_m] *= scaleDown; }

  // ret.
  return Xk;
}

