/*
 * reference LICENSE file provided.
 * 
 * dspFilter.c.
 * Implements dspFilter.c.
 * 
 */

WindowSelection pick_window(double arg_stopbandGain)
{
  // vars.
  int32_t rtn_window = -1;
  double stopband_gain_dB = 20.0*log10(arg_stopbandGain);
  
  // map desired stopband gain to necessary window.
  // 
  if (stopband_gain_dB > -21.0) { rtn_window = RECTANGULAR; }
  else if (stopband_gain_dB > -44.0) { rtn_window = HANNING; }
  else if (stopband_gain_dB > -53.0) { rtn_window = HAMMING; }
  else if (stopband_gain_dB > -74.0) { rtn_window = BLACKMAN; }
  else
  {
    printf("pick_window: bad input\n");
    assert(0);
  }
  
  // ret.
  return rtn_window;
}

dvec_o generate_window(
  WindowSelection arg_whichWindow, int32_t arg_windowLen)
{
  // vars.
  int32_t n = 0;
  dvec_o Wn = new_dvec(arg_windowLen);
 
  // given window selection, generate window function.
  //
  if (arg_whichWindow == RECTANGULAR)
  {
    for (n = 0; n < Wn.len; n++)
    {
      Wn.arr[n] = 1.0;
    }
  }
  else if (arg_whichWindow == HANNING)
  {
    for (n = 0; n < Wn.len; n++)
    {
      Wn.arr[n] =
        0.5 * (1.0 - cos((2.0 * M_PI * (double)(n)) / (Wn.len - 1)));
    }
  }
  else if (arg_whichWindow == HAMMING)
  {
    for (n = 0; n < Wn.len; n++)
    {
      Wn.arr[n] =
        0.54 - 0.46 * cos((2.0 * M_PI * (double)(n)) / (Wn.len - 1));
    }
  }
  else if (arg_whichWindow == BLACKMAN)
  {
    for (n = 0; n < Wn.len; n++)
    {
      Wn.arr[n] =
        0.42 - 0.5 *
	  cos((2.0 * M_PI * (double)(n)) / (Wn.len - 1)) +
	  0.08 * cos((4.0 * M_PI * (double)(n)) / (Wn.len - 1));
    }
  }
  else
  {
    printf("generate_window: bad input\n");
    assert(0);
  }

  // ret.
  return Wn;
}

int32_t get_filter_length(
  double arg_w1, double arg_w2, double arg_stopbandGain)
{
  // vars.
  int32_t M = 0;
  double dw = fabs(arg_w1 - arg_w2);
  WindowSelection sel_window = pick_window(arg_stopbandGain);
 
  // determine 'M'; filter length, based on window and delta w.
  //
  if (sel_window == RECTANGULAR)   { M = (int32_t)(4.0*M_PI / dw); } 
  else if (sel_window == HANNING)  { M = (int32_t)(8.0*M_PI / dw); } 
  else if (sel_window == HAMMING)  { M = (int32_t)(8.0*M_PI / dw); } 
  else if (sel_window == BLACKMAN) { M = (int32_t)(12.0*M_PI / dw); } 
  else
  {
    printf("get_filter_length: bad input\n");
    assert(0);
  }

  // ensure odd filter-length.
  //
  if ((M % 2) == 0) { M++; }

  // ret. 
  return M;
}

dvec_o generate_lowpass_impulse_response(
  int32_t arg_filterLen, double arg_wc)
{
  // arg check: odd filter length.
  if ((arg_filterLen & 0x1) == 0) { arg_filterLen++; }

  // vars. 
  int32_t _n = 0;
  double wc_div_pi = arg_wc / M_PI;
  dvec_o hn = new_dvec(arg_filterLen);
 
  // copy in sinc data.
  //
  for (_n = 0; _n < hn.len; _n++)
  {
    hn.arr[_n] =
      wc_div_pi * sinc(arg_wc * (double)(_n - ((hn.len - 1) / 2)));
  }
 
  // ret. 
  return hn;
}

dvec_o generate_highpass_impulse_response(
  int32_t arg_filterLen, double arg_wc)
{
  // arg check: odd filter length.
  if ((arg_filterLen & 0x1) == 0) { arg_filterLen++; }
 
  // vars. 
  int32_t _n = 0;
  double wc_div_pi = arg_wc / M_PI;
  dvec_o hn = new_dvec(arg_filterLen);
 
  // copy in the sinc data.
  //
  for (_n = 0; _n < hn.len; _n++)
  {
    hn.arr[_n] =
      -wc_div_pi * sinc(arg_wc * (double)(_n - ((hn.len - 1) / 2)));
  }
  
  hn.arr[(hn.len - 1) / 2] += 1.0;
 
  // ret.  
  return hn;
}

dvec_o generate_lowpass_filter
  ( double arg_wL1
  , double arg_wL2
  , double arg_stopbandGain
  )
{
  // determine cutoff freq.
  double wc = (arg_wL1 + arg_wL2) / 2.0;
 
  // determine impulse response. 
  dvec_o hn =
    generate_lowpass_impulse_response(
      get_filter_length(arg_wL1, arg_wL2, arg_stopbandGain), wc);

  // determine window function.
  dvec_o Wn =
    generate_window(pick_window(arg_stopbandGain), hn.len);
 
  // apply window function. 
  dvec_o rtn_hn = mul_dvec(hn, Wn);
  del_dvec(hn);
  del_dvec(Wn);
 
  // ret. 
  return rtn_hn;
}

dvec_o generate_highpass_filter
  ( double arg_wH1
  , double arg_wH2
  , double arg_stopbandGain
  )
{
  // determine cutoff freq.
  double wc = (arg_wH1 + arg_wH2) / 2.0;
 
  // determine impulse response. 
  dvec_o hn =
    generate_highpass_impulse_response(
      get_filter_length(arg_wH1, arg_wH2, arg_stopbandGain), wc);

  // determine window function.
  dvec_o Wn =
    generate_window(pick_window(arg_stopbandGain), hn.len);
 
  // apply window function.
  dvec_o rtn_hn = mul_dvec(hn, Wn);
  del_dvec(hn);
  del_dvec(Wn); 
 
  // ret. 
  return rtn_hn;
}

dvec_o generate_bandstop_filter
  ( double arg_wL1
  , double arg_wL2
  , double arg_wH1
  , double arg_wH2
  , double arg_stopbandGain
  )
{
  // lowpass / highpass / final impulse responses.
  //
  dvec_o hnL = generate_lowpass_filter(arg_wL1, arg_wL2, arg_stopbandGain);
  dvec_o hnH = generate_highpass_filter(arg_wH1, arg_wH2, arg_stopbandGain);
  dvec_o hn = add_cvec(hnL, hnH);

  // destroy lowpass / highpass; bandpass is now built.
  //
  del_dvec(hnL);
  del_dvec(hnH);
 
  // ret. 
  return hn;
}

dvec_o generate_bandpass_filter
  ( double arg_wL1
  , double arg_wL2
  , double arg_wH1
  , double arg_wH2
  , double arg_stopbandGain
  )
{
  // lowpass / highpass / final impulse responses.
  //
  dvec_o hnL = generate_lowpass_filter(arg_wL1, arg_wL2, arg_stopbandGain);
  dvec_o hnH = generate_highpass_filter(arg_wH1, arg_wH2, arg_stopbandGain);
  dvec_o hn = add_cvec(hnL, hnH);

  // destroy lowpass / highpass; bandpass is now built.
  //
  del_dvec(hnL);
  del_dvec(hnH);

  // final processing for bandpass filter.
  //
  int32_t _n = 0;
  for (_n = 0; _n < hn.len; _n++) { _hn[n] *= -1.0; } 
  hn.arr[(hn.len - 1) / 2] += 1.0;
 
  // ret. 
  return hn;
}

