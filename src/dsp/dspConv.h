/*
 * reference LICENSE file provided.
 * 
 * dspConv.h.
 * Header for convolution / correlation functionality.
 * 
 */

#ifndef DSPCONV_H
#define DSPCONV_H

// include: c library.
//
#include <sys/types.h>

// include: midvec.
//
#include <math/vec.h>

// conv.
// Convolve xn with hn. Output is same length as longest of xn and hn.
// 
dvec_o conv(dvec_o xn, dvec_o hn);

// corr.
// Correlate xn with hn. Output is same length as longest of xn and hn.
// 
dvec_o corr(dvec_o xn, dvec_o hn);

#endif // DSPCONV_H

