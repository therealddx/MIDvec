/*
 * reference LICENSE file provided.
 * 
 * dspFourier.h.
 * Header file for frequency-domain (Fourier analysis) functions.
 * 
 */

#ifndef DSPFOURIER_H
#define DSPFOURIER_H

// include: c standard library.
#include <sys/types.h>
#include <math.h>
#include <stdio.h>

// include: midvec library.
#include <math/vec.h>

// fft.
// Calculate FFT of xn.
// 
cvec_o fft(cvec_o arg_xn);

#endif // DSPFOURIER_H

