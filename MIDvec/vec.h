/*
 * reference LICENSE file provided.
 *
 * vec.h.
 * definition of vector objects.
 *
 */

#ifndef VEC_H
#define VEC_H

// include: c library.
//
#include <sys/types.h>
#include <stdlib.h>

typedef struct dvec
{
  double* arr;
  u_int64_t len;
} dvec_o;

typedef struct cvec
{
  double complex* arr;
  u_int64_t len;
} cvec_o;

dvec_o new_dvec(u_int64_t arg_len);

dvec_o copy_dvec(dvec_o arg_dvec);

cvec_o new_cvec(u_int64_t arg_len);

cvec_o copy_cvec(cvec_o arg_cvec);

#endif // VEC_H

