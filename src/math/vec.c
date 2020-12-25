/*
 * reference LICENSE file provided.
 *
 * vec.c.
 * implements vec.h.
 *
 */

#include "vec.h"

dvec_o copy_dvec(dvec_o arg_dvec)
{
  dvec_o toReturn = new_dvec(arg_dvec.len);

  u_int64_t _n = 0;
  for (_n = 0; _n < arg_dvec.len; _n++)
    { toReturn.arr[_n] = arg_dvec.arr[_n]; }

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

void rcopy_dvec(dvec_o* arg_src, dvec_o* rtn_dst)
{
  // arg check.
  if (arg_src == NULL || rtn_dst == NULL)
  {
    fprintf(stderr, "rcopy_dvec: null reference");
    assert(0);
  }

  // copy.
  u_int64_t _n = 0;
  for (_n = 0; _n < arg_src->len && _n < rtn_dst->len; _n++)
  {
    rtn_dst->arr[_n] = arg_src->arr[_n];
  }
}

void rcopy_cvec(cvec_o* arg_src, cvec_o* rtn_dst)
{
  // arg check.
  if (arg_src == NULL || rtn_dst == NULL)
  {
    fprintf(stderr, "rcopy_cvec: null reference");
    assert(0);
  }

  // copy.
  u_int64_t _n = 0;
  for (_n = 0; _n < arg_src->len && _n < rtn_dst->len; _n++)
  {
    rtn_dst->arr[_n] = arg_src->arr[_n];
  }
}

dvec_o new_dvec(u_int64_t arg_len)
{
  dvec_o toReturn;
  
  toReturn.len = arg_len;
  toReturn.arr = (double*)malloc(sizeof(double) * toReturn.len);
  u_int64_t _n;
  for (_n = 0; _n < toReturn.len; _n++) { toReturn.arr[_n] = 0; }

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

void del_dvec(dvec_o arg_dvec)
{
  free(arg_dvec.arr);
}

void del_cvec(cvec_o arg_cvec)
{
  free(arg_cvec.arr);
}

cvec_o upcast_dvec(dvec_o arg_dvec)
{
  // vars.
  cvec_o toReturn = new_cvec(arg_dvec.len);
  int32_t _n = 0;

  // copy.
  for (_n = 0; _n < arg_dvec.len; _n++)
  {
    toReturn.arr[_n] = arg_dvec.arr[_n] + I * 0;
  }

  // ret.
  return toReturn;
}

