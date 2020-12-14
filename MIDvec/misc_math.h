/*
 * reference LICENSE file provided.
 * 
 * misc_math.h.
 * miscellaneous methods for general math operations.
 * 
 */
 
#ifndef MISC_MATH_H
#define MISC_MATH_H

#include <stdlib.h>
#include <math.h>

double hard_limit(double x);

double rand_given_bound(double lo, double hi);

int int_rand_given_bound(int lo, int hi);

double sinc(double x);

double sincpi(double x);

double sigmoid(double x);

double sigmoid_ddx(double x);

int revolve_mod(int cur, int lim);

int round_up_int(int x, int base);

int less(int a, int b);

int greater(int a, int b);

double map_val
  ( double lo_1
  , double hi_1
  , double lo_2
  , double hi_2
  , double val
  );

#endif // MISC_MATH_H

