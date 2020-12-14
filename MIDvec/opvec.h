/*
 * reference LICENSE file provided.
 *
 * opvec.h.
 * Header file defining operations to be done between vectors.
 *
 */

#ifndef OPVEC_H
#define OPVEC_H

#include <assert.h>
#include <stdio.h>

#include <vec.h>

dvec_o mul_dvec(dvec_o, dvec_o);

cvec_o mul_cvec(cvec_o, cvec_o);

dvec_o add_dvec(dvec_o, dvec_o);

cvec_o add_cvec(cvec_o, cvec_o);

#endif // OPVEC_H

