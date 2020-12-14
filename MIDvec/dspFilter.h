/*
 * reference LICENSE file provided.
 * 
 * dspFilter.h.
 * Header file for DSP filter operations:
 *  Selecting and generating window functions for stopband gain.
 *  Generating impulse response for a filter.
 *  Generating filter functions for a comprehensive convolution filter.
 * 
 */

#ifndef DSPFILTER_H
#define DSPFILTER_H

#include <assert.h>

#include <vec.h>
#include <opvec.h>

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
WindowSelection pick_window(double arg_stopbandGain);

// generate_window.
// Given desired WindowSelection, generate window function signal.
//
dvec_o generate_window(
  WindowSelection arg_whichWindow, int32_t arg_windowLen);

// get_filter_length.
//
// Given desired digital frequency cutoffs 'w_1' and 'w_2', and desired
// stopband gain, calculate necessary filter length.
//
int32_t get_filter_length(double w_1, double w_2, double stopband_gain);

// generate_lowpass_impulse_response.
// generate_highpass_impulse_response.
//
// Given filter length and digital frequency cutoffs,
//   generate low-pass / high-pass impulse response.
// 
dvec_o generate_lowpass_impulse_response(
  int32_t arg_filterLen, double arg_wc);

dvec_o generate_highpass_impulse_response(
  int32_t arg_filterLen, double arg_wc);

// generate_lowpass_filter.
// generate_highpass_filter.
// generate_bandstop_filter.
// generate_bandpass_filter.
//
// Given digital frequency cutoffs and stopband gain values,
//   generate LPF / HPF / BPS / BPF impulse response.
//
dvec_o generate_lowpass_filter
  ( double wL_1
  , double wL_2
  , double stopband_gain
  );

dvec_o generate_highpass_filter
  ( double wH_1
  , double wH_2
  , double stopband_gain
  );

dvec_o generate_bandstop_filter
  ( double wL_1
  , double wL_2
  , double wH_1
  , double wH_2
  , double stopband_gain
  );

dvec_o generate_bandpass_filter
  ( double wL_1
  , double wL_2
  , double wH_1
  , double wH_2
  , double stopband_gain
  );

#endif // DSPFILTER_H

