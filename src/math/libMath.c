/*
 * reference LICENSE file provided.
 *
 * libMath.c.
 * Implements libMath.h.
 *
 */

#include "libMath.h"

double cmag(double complex x)
{
  return sqrt(creal(x) * creal(x) + cimag(x) * cimag(x));
}

double sinc(double x)
{
  if (x == 0)
    return 1;
  else
    return sin(x) / x;
}

double sincpi(double x)
{
  return sinc(x * M_PI);
}

double sigmoid(double x)
{
  return 1.0 / (1.0 + exp(-x));
}

double sigmoid_ddx(double x)
{
  return (sigmoid(x) * (1.0 - sigmoid(x)));
}

double map_val
  ( double lo_1
  , double hi_1
  , double lo_2
  , double hi_2
  , double val
  )
{
  //Map val from [lo_1, hi_1] to [lo_2, hi_2].
  
  //1. Subtract lo_1.
    // [lo_1 - lo_1, hi_1 - lo_1] => [0, hi_1 - lo_1].
  //2. Divide by (hi_1 - lo_1).
    // [0 / (hi_1 - lo_1), (hi_1 - lo_1) / (hi_1 - lo_1)] => [0, 1].
  //3. Multiply by (hi_2 - lo_2). 
    // [0, 1] * (hi_2 - lo_2] = [0, hi_2 - lo_2].
  //4. Add lo_2.
    // [0, hi_2 - lo_2] + lo_2 => [lo_2, hi_2].
  
  val = (val - lo_1) / (hi_1 - lo_1);
  val = val * (hi_2 - lo_2) + lo_2;
  return val;
}

double rand_given_bound(double lo, double hi)
{
  return (rand() / (double)RAND_MAX) * (hi - lo) + lo;
}

