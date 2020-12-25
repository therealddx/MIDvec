/*
 * reference LICENSE file provided.
 *
 * vecOps.c.
 * Implements vecOps.h.
 *
 */

#include "vecOps.h"

dvec_o mul_dvec(dvec_o arg_v1, dvec_o arg_v2)
{
  // arg check: vector lengths.
  if (arg_v1.len != arg_v2.len)
  {
    fprintf(stderr, "mul_dvec: bad vector lengths\n");
    assert(0);
  }

  // vars.
  dvec_o toReturn = new_dvec(arg_v1.len);
  int32_t _n = 0;

  // multiply.
  for (_n = 0; _n < arg_v1.len; _n++)
  {
    toReturn.arr[_n] = arg_v1.arr[_n] * arg_v2.arr[_n];
  }

  // ret.
  return toReturn;
}

cvec_o mul_cvec(cvec_o arg_v1, cvec_o arg_v2)
{
  // arg check: vector lengths.
  if (arg_v1.len != arg_v2.len)
  {
    fprintf(stderr, "mul_cvec: bad vector lengths\n");
    assert(0);
  }

  // vars.
  cvec_o toReturn = new_cvec(arg_v1.len);
  int32_t _n = 0;

  // multiply.
  for (_n = 0; _n < arg_v1.len; _n++)
  {
    toReturn.arr[_n] = arg_v1.arr[_n] * arg_v2.arr[_n];
  }

  // ret.
  return toReturn;
}

dvec_o add_dvec(dvec_o arg_v1, dvec_o arg_v2)
{
  // arg check: vector lengths.
  if (arg_v1.len != arg_v2.len)
  {
    fprintf(stderr, "add_dvec: bad vector lengths\n");
    assert(0);
  }

  // vars.
  dvec_o toReturn = new_dvec(arg_v1.len);
  int32_t _n = 0;

  // add.
  for (_n = 0; _n < arg_v1.len; _n++)
  {
    toReturn.arr[_n] = arg_v1.arr[_n] + arg_v2.arr[_n];
  }

  // ret.
  return toReturn;
}

cvec_o add_cvec(cvec_o arg_v1, cvec_o arg_v2)
{
  // arg check: vector lengths.
  if (arg_v1.len != arg_v2.len)
  {
    fprintf(stderr, "add_cvec: bad vector lengths\n");
    assert(0);
  }

  // vars.
  cvec_o toReturn = new_cvec(arg_v1.len);
  int32_t _n = 0;

  // add.
  for (_n = 0; _n < arg_v1.len; _n++)
  {
    toReturn.arr[_n] = arg_v1.arr[_n] + arg_v2.arr[_n];
  }

  // ret.
  return toReturn;
}

