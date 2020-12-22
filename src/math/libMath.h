/*
 * reference LICENSE file provided.
 * 
 * libMath.h.
 * Functions for math-operations, highly relevant to this library, that
 * are not included in the C standard.
 * 
 */
 
#ifndef LIBMATH_H
#define LIBMATH_H

#include <stdlib.h>
#include <math.h>

double sinc(double x);

double sincpi(double x);

double sigmoid(double x);

double sigmoid_ddx(double x);

double map_val
  ( double lo_1
  , double hi_1
  , double lo_2
  , double hi_2
  , double val
  );

double rand_given_bound(double lo, double hi);

#endif // LIBMATH_H

