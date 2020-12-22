/*
 * reference LICENSE file provided.
 * 
 * dspConv.c.
 * Implements dspConv.h.
 * 
 */

#include "dspConv.h"

dvec_o conv(dvec_o xn, dvec_o hn) 
{
  // vars.
  int64_t n = 0;
  int64_t m = 0;
  dvec_o rn = new_dvec(xn.len);
 
  // run. 
  for (n = 0; n < rn.len; n++)
  {
    for (m = 0; m < hn.len; m++)
    {
      if (n - m < 0)
        continue;
      rn.arr[n] += xn.arr[n - m] * hn.arr[m];
    }
  }

  // ret.
  return rn;
}

dvec_o corr(dvec_o xn, dvec_o hn)
{
  // vars.
  int64_t n = 0;
  int64_t m = 0;
  dvec_o rn = new_dvec(xn.len);

  // run. 
  for (n = 0; n < rn.len; n++)
  {
    for (m = 0; m < hn.len; m++)
    {
      if ((n + m) >= rn.len)
        break;
      rn.arr[n] += xn.arr[n + m] * hn.arr[m];
    }
  }

  // ret. 
  return rn;
}

