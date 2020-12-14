/*
 * reference LICENSE file provided.
 * 
 * dspFourier.h.
 * Header file for frequency-domain (Fourier analysis) functions.
 * 
 */

#ifndef DSPFOURIER_H
#define DSPFOURIER_H

#include <sys/types.h>

// fft.
// Calculate FFT of xn.
// 
cvec_o fft(cvec_o xn);

// ifft.
// Calculate inverse FFT of frequency-domain representation, Xk.
// 
cvec_o ifft(cvec_o Xk);

#endif // DSPFOURIER_H

