/*
 * reference LICENSE file provided.
 *
 * vec.c.
 * implements vec.h.
 *
 */

#include "vec.h"

dvec_o new_dvec(u_int64_t arg_len)
{
  dvec_o toReturn;
  
  toReturn.len = arg_len;
  toReturn.arr = (double*)malloc(sizeof(double) * toReturn.len);
  u_int64_t _n;
  for (_n = 0; _n < toReturn.len; _n++) { toReturn.arr[_n] = 0; }

  return toReturn;
}

dvec_o copy_dvec(dvec_o arg_dvec)
{
  dvec_o toReturn = new_dvec(arg_dvec.len);

  u_int64_t _n = 0;
  for (_n = 0; _n < arg_dvec.len; _n++)
    { toReturn.arr[_n] = arg_dvec.arr[_n]; }

  return toReturn;
}

cvec_o new_cvec(u_int64_t arg_len)
{
  cvec_o toReturn;

  toReturn.len = arg_len;
  toReturn.arr =
    (double complex*)malloc(sizeof(double complex) * toReturn.len);
  u_int64_t _n;
  for (_n = 0; _n < toReturn.len; _n++) { toReturn.arr[_n] = 0; }

  return toReturn;
}

cvec_o copy_cvec(cvec_o arg_cvec)
{
  cvec_o toReturn = new_cvec(arg_cvec.len);

  u_int64_t _n = 0;
  for (_n = 0; _n < arg_cvec.len; _n++)
    { toReturn.arr[_n] = arg_cvec.arr[_n]; }

  return toReturn;
}

void del_dvec(dvec_o arg_dvec)
{
  free(arg_dvec.arr);
}

void del_cvec(cvec_o arg_cvec)
{
  free(arg_cvec.arr);
}

