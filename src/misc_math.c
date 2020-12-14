/*
 * reference LICENSE file provided.
 *
 * misc_math.c.
 * Implements misc_math.h.
 *
 */

double hard_limit(double x)
{
  if (x < 0)
    return -1;
  else
    return 1;
}

double rand_given_bound(double lo, double hi)
{
  return (rand() / (double)RAND_MAX) * (hi - lo) + lo;
}

int int_rand_given_bound(int lo, int hi)
{
  
  double lo_d = (double)lo;
  double hi_d = (double)hi;
  
  // [lo, hi], endpoint-inclusive.
  // (rand() / (double)(RAND_MAX)) * (hi_d + 1.0 - lo_d)
  //  -> returns [0, hi_d + 1.0 - lo_d)
  // (rand() / (double)(RAND_MAX)) * (hi_d + 1.0 - lo_d) + lo_d
  //  -> returns [lo_d, hi_d + 1.0)
  //
  return (int)((rand() / (double)(RAND_MAX)) * (hi_d + 1.0 - lo_d) + lo_d);
  //-> [lo, hi].
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

int revolve_mod(int cur, int lim)
{
  cur++;
  if (cur < lim)
    return cur;
  else
    cur = 0;
  return cur;
}

int round_up_int(int x, int base)
{
  return ((x - 1 + base) / base) * base;
}

int less(int a, int b)
{
  if (a < b)
    return a;
  else
    return b;
}  

int greater(int a, int b)
{
  if (a > b)
    return a;
  else
    return b;
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

