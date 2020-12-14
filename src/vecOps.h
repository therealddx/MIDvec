/*
 * reference LICENSE file provided.
 *
 * vecOps.h.
 * Header file defining operations that can be done between vectors.
 *
 */

#ifndef VECOPS_H
#define VECOPS_H

#include <assert.h>
#include <stdio.h>

#include <vec.h>

dvec_o mul_dvec(dvec_o, dvec_o);

cvec_o mul_cvec(cvec_o, cvec_o);

dvec_o add_dvec(dvec_o, dvec_o);

cvec_o add_cvec(cvec_o, cvec_o);

#endif // VECOPS_H

