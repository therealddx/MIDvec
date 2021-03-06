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
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <stdlib.h>
#include <complex.h>

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

dvec_o copy_dvec(dvec_o arg_dvec);

cvec_o copy_cvec(cvec_o arg_cvec);

void rcopy_dvec(dvec_o* arg_src, dvec_o* rtn_dst);

void rcopy_cvec(cvec_o* arg_src, cvec_o* rtn_dst);

dvec_o new_dvec(u_int64_t arg_len);

cvec_o new_cvec(u_int64_t arg_len);

void del_dvec(dvec_o arg_dvec);

void del_cvec(cvec_o arg_cvec);

cvec_o upcast_dvec(dvec_o arg_dvec);

#endif // VEC_H

