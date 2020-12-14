/*
 * reference LICENSE file provided.
 * 
 * dsp.h.
 * Header file for core digital-signal-processing functionality.
 * Declares convolution, correlation, frequency-domain analysis, and 
 * filtering functions.
 * 
 */

#ifndef DSP_H
#define DSP_H

// include: c-standard library.
// 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

// include: self library.
// 
#include "misc_math.h"

// conv.
// Convolve xn with hn. Output is same length as longest of xn and hn.
// 
dvec_o conv(dvec_o xn, dvec_o hn);

// corr.
// Correlate xn with hn. Output is same length as longest of xn and hn.
// 
dvec_o corr(dvec_o xn, dvec_o hn);

// fft.
// Calculate FFT of xn.
// 
cpx_vec_o fft(cpx_vec_o xn);

// ifft.
// Calculate inverse FFT of frequency-domain representation, Xk.
// 
cpx_vec_o ifft(cpx_vec_o Xk);

enum WindowSelection
{
  RECTANGULAR = 0, // -21.0 dB stopband attenuation.
  HANNING     = 1, // -44.0 dB stopband attenuation.
  HAMMING     = 2, // -53.0 dB stopband attenuation.
  BLACKMAN    = 3  // -74.0 dB stopband attenuation.
};

// pick_window.
// Given desired stopband gain, select necessary window.
// 
WindowSelection pick_window(double stopband_gain);
dvec_o generate_window(WindowSelection which_window, int window_length);
int get_filter_length(double w_1, double w_2, double stopband_gain);

// generate_lowpass_impulse_response.
// Given FIR filter parameters, calculate a lowpass-filter impulse response with sinc().
// 
dvec_o generate_lowpass_impulse_response(int filter_length, double wc);

// generate_highpass_impulse_response.
// Given FIR filter parameters, calculate a highpass-filter impulse response with sinc().
// 
dvec_o generate_highpass_impulse_response(int filter_length, double wc);

// generate_lowpass_filter.
// Return a function such that, on convolution with an input signal,
// that input signal is lowpass filtered, IAW the input filter arguments.
// 
dvec_o generate_lowpass_filter(double wL_1, double wL_2, double stopband_gain);

// generate_highpass_filter.
// Return a function such that, on convolution with an input signal,
// that input signal is highpass filtered, IAW the input filter arguments.
// 
dvec_o generate_highpass_filter(double wH_1, double wH_2, double stopband_gain);

// generate_bandstop_filter.
// Return a function such that, on convolution with an input signal,
// that input signal is bandstop filtered, IAW the input filter arguments.
// 
dvec_o generate_bandstop_filter(double wL_1, double wL_2, double wH_1, double wH_2, double stopband_gain);

// generate_highpass_filter.
// Return a function such that, on convolution with an input signal,
// that input signal is bandpass filtered, IAW the input filter arguments.
// 
dvec_o generate_bandpass_filter(double wL_1, double wL_2, double wH_1, double wH_2, double stopband_gain);

#endif