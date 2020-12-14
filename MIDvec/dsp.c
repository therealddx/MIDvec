/*
 * reference LICENSE file provided.
 * 
 * dsp.c.
 * Implements dsp.h.
 * 
 */

dvec_o conv(dvec_o xn, dvec_o hn) 
{
	int n = 0;
	int m = 0;
	
	dvec_o rn = new_dvec(xn.len);
	
	for (n = 0; n < rn.len; n++) {
		for (m = 0; m < hn.len; m++) {
			if (n - m < 0)
				continue;
			rn.arr[n] += xn.arr[n - m] * hn.arr[m];
		}
	}
	return rn;
}

dvec_o corr(dvec_o xn, dvec_o hn)
{
	int n = 0;
	int m = 0;
	
	dvec_o rn = new_dvec(xn.len);
	
	for (n = 0; n < rn.len; n++) {
		for (m = 0; m < hn.len; m++) {
			if ((n + m) >= rn.len)
				break;
			rn.arr[n] += xn.arr[n + m] * hn.arr[m];
		}
	}
	
	return rn;
}

cpx_vec_o fft(cpx_vec_o xn)
{
	
	int new_length = 2;
	while (1) {
		if (new_length < xn.len)
			new_length <<= 1;
		else if (new_length == xn.len)
			break;
		else if (new_length > xn.len) {
			xn = zeropad_cpx(xn, new_length);
			break;
		}
	}
	
	int n = 0;
	cpx_vec_o Xk;
	Xk.len = xn.len;
	Xk.arr = (cpx_o*)malloc(sizeof(cpx_o)*Xk.len);
	
	cpx_vec_o Xkbuf;
	Xkbuf.len = xn.len;
	Xkbuf.arr = (cpx_o*)malloc(sizeof(cpx_o)*Xk.len);
	
	for (n = 0; n < Xk.len; n++) {
		Xk.arr[n].real = 0;
		Xk.arr[n].imag = 0;
		Xkbuf.arr[n].real = 0;
		Xkbuf.arr[n].imag = 0;
	}
	
	for (n = 0; n < (Xk.len >> 1); n++) {
		Xk.arr[n] = cpx_add(xn.arr[n], xn.arr[n + (xn.len >> 1)]);
		Xk.arr[n + (Xk.len >> 1)] = cpx_sub(xn.arr[n], xn.arr[n + (xn.len >> 1)]);
	}
	
	int depth = (int)(log10((double)Xk.len) / log10(2.0)) - 1;
	int y_int = 0;
	int gap = Xk.len >> 1;
	int inst_N = 4;
	int m = 0;
	cpx_o tw; tw.real = 0; tw.imag = 0;
		int tw_m = 0;
		double tw_p = 0;
	
	//printf("%d, %d, %d, %d, %d\r\n", depth, y_int, gap, inst_N, m);
	
	while (depth > 0) {
		//printf("Depth is %d\r\n", depth);
		for (y_int = 0; y_int < (gap >> 1); y_int++) {
			for (m = 0; m < (inst_N >> 1); m++) {
				tw_p = -2.0 * M_PI * (double)(tw_m) / (double)(inst_N);
				tw.real = cos(tw_p);
				tw.imag = sin(tw_p);
				
				Xkbuf.arr[(gap >> 1) * m + y_int] = cpx_add(Xk.arr[gap * m + y_int], cpx_mul(tw, Xk.arr[gap * m + (gap >> 1) + y_int]));
				Xkbuf.arr[(gap >> 1) * m + y_int + (Xk.len >> 1)] = cpx_sub(Xk.arr[gap * m + y_int], cpx_mul(tw, Xk.arr[gap * m + (gap >> 1) + y_int]));
				
				//printf("Storing to: %d, %d\r\n", (gap>>1)*m+y_int, (gap>>1)*m+y_int+(Xk.len>>1));
				//printf("Reading from: %d, %d\r\n", gap*m+y_int, gap*m+(gap>>1)+y_int);
				//printf("%d / %d\r\n", tw_m, inst_N);
				
				tw_m++;
			} //printf("m loop done\r\n");
			tw_m = 0;
		} //printf("y_int loop done\r\n");
		
		copy_cpx_vec(Xkbuf, Xk);
		
		depth--;
		inst_N <<= 1;
		gap >>= 1;
	}
	
	cpx_vec_scale(Xk, 1.0 / (double)(Xk.len));
	return Xk;
}

dvec_o magfft(cpx_vec_o xn, int lengthchoose)
{
	
	if (lengthchoose != 0)
		xn = zeropad_cpx(xn, lengthchoose);
	
	int new_length = 2;
	while (1) {
		if (new_length < xn.len)
			new_length <<= 1;
		else if (new_length == xn.len)
			break;
		else if (new_length > xn.len) {
			xn = zeropad_cpx(xn, new_length);
			break;
		}
	}
	
	int n = 0;
	cpx_vec_o Xk;
	Xk.len = xn.len;
	Xk.arr = (cpx_o*)malloc(sizeof(cpx_o)*Xk.len);
	
	cpx_vec_o Xkbuf;
	Xkbuf.len = xn.len;
	Xkbuf.arr = (cpx_o*)malloc(sizeof(cpx_o)*Xk.len);
	
	for (n = 0; n < Xk.len; n++) {
		Xk.arr[n].real = 0;
		Xk.arr[n].imag = 0;
		Xkbuf.arr[n].real = 0;
		Xkbuf.arr[n].imag = 0;
	}
	
	for (n = 0; n < (Xk.len >> 1); n++) {
		Xk.arr[n] = cpx_add(xn.arr[n], xn.arr[n + (xn.len >> 1)]);
		Xk.arr[n + (Xk.len >> 1)] = cpx_sub(xn.arr[n], xn.arr[n + (xn.len >> 1)]);
	}
	
	int depth = (int)(log10((double)Xk.len) / log10(2.0)) - 1;
	int y_int = 0;
	int gap = Xk.len >> 1;
	int inst_N = 4;
	int m = 0;
	cpx_o tw; tw.real = 0; tw.imag = 0;
		int tw_m = 0;
		double tw_p = 0;
	
	//printf("%d, %d, %d, %d, %d\r\n", depth, y_int, gap, inst_N, m);
	
	while (depth > 0) {
		//printf("Depth is %d\r\n", depth);
		for (y_int = 0; y_int < (gap >> 1); y_int++) {
			for (m = 0; m < (inst_N >> 1); m++) {
				tw_p = -2.0 * M_PI * (double)(tw_m) / (double)(inst_N);
				tw.real = cos(tw_p);
				tw.imag = sin(tw_p);
				
				Xkbuf.arr[(gap >> 1) * m + y_int] = cpx_add(Xk.arr[gap * m + y_int], cpx_mul(tw, Xk.arr[gap * m + (gap >> 1) + y_int]));
				Xkbuf.arr[(gap >> 1) * m + y_int + (Xk.len >> 1)] = cpx_sub(Xk.arr[gap * m + y_int], cpx_mul(tw, Xk.arr[gap * m + (gap >> 1) + y_int]));
				
				//printf("Storing to: %d, %d\r\n", (gap>>1)*m+y_int, (gap>>1)*m+y_int+(Xk.len>>1));
				//printf("Reading from: %d, %d\r\n", gap*m+y_int, gap*m+(gap>>1)+y_int);
				//printf("%d / %d\r\n", tw_m, inst_N);
				
				tw_m++;
			} //printf("m loop done\r\n");
			tw_m = 0;
		} //printf("y_int loop done\r\n");
		
		copy_cpx_vec(Xkbuf, Xk);
		
		depth--;
		inst_N <<= 1;
		gap >>= 1;
	}
	
	cpx_vec_scale(Xk, 1.0 / (double)(Xk.len));
	return mag_cpx_vec(Xk);
}

cpx_vec_o ifft(cpx_vec_o Xk)
{
	int n = 0;
	
	cpx_vec_o xn;
	xn.len = Xk.len;
	xn.arr = (cpx_o*)malloc(sizeof(cpx_o)*xn.len);
	
	cpx_vec_o xnbuf;
	xnbuf.len = Xk.len;
	xnbuf.arr = (cpx_o*)malloc(sizeof(cpx_o)*xn.len);
	
	for (n = 0; n < xn.len; n++) {
		xn.arr[n].real = 0;
		xn.arr[n].imag = 0;
		xnbuf.arr[n].real = 0;
		xnbuf.arr[n].imag = 0;
	}
	
	for (n = 0; n < (xn.len >> 1); n++) {
		xn.arr[n] = cpx_add(Xk.arr[n], Xk.arr[n + (Xk.len >> 1)]);
		xn.arr[n + (xn.len >> 1)] = cpx_sub(Xk.arr[n], Xk.arr[n + (Xk.len >> 1)]);
	}
	
	int depth = (int)(log10((double)xn.len) / log10(2.0)) - 1;
	int y_int = 0;
	int gap = xn.len >> 1;
	int inst_N = 4;
	int m = 0;
	cpx_o tw; tw.real = 0; tw.imag = 0;
		int tw_m = 0;
		double tw_p = 0;
	
	//printf("%d, %d, %d, %d, %d\r\n", depth, y_int, gap, inst_N, m);
	
	while (depth > 0) {
		//printf("Depth is %d\r\n", depth);
		for (y_int = 0; y_int < (gap >> 1); y_int++) {
			for (m = 0; m < (inst_N >> 1); m++) {
				tw_p = -2.0 * M_PI * (double)(tw_m) / (double)(inst_N);
				tw.real = cos(tw_p);
				tw.imag = sin(tw_p);
				
				xnbuf.arr[(gap >> 1) * m + y_int] = cpx_add(xn.arr[gap * m + y_int], cpx_mul(tw, xn.arr[gap * m + (gap >> 1) + y_int]));
				xnbuf.arr[(gap >> 1) * m + y_int + (xn.len >> 1)] = cpx_sub(xn.arr[gap * m + y_int], cpx_mul(tw, xn.arr[gap * m + (gap >> 1) + y_int]));
				
				//printf("Storing to: %d, %d\r\n", (gap>>1)*m+y_int, (gap>>1)*m+y_int+(xn.len>>1));
				//printf("Reading from: %d, %d\r\n", gap*m+y_int, gap*m+(gap>>1)+y_int);
				//printf("%d / %d\r\n", tw_m, inst_N);
				
				tw_m++;
			} //printf("m loop done\r\n");
			tw_m = 0;
		} //printf("y_int loop done\r\n");
		
		copy_cpx_vec(xnbuf, xn);
		
		depth--;
		inst_N <<= 1;
		gap >>= 1;
	}
	return xn;
}

WindowSelection pick_window(double stopband_gain)
{
  // vars.
  int which_window = -1;
	double stopband_gain_dB = 20.0*log10(stopband_gain);
	
  // map desired stopband gain to necessary window.
  // 
	if (stopband_gain_dB > -21.0) //Rectangular.
		which_window = RECTANGULAR;
	else if (stopband_gain_dB > -44.0) //Hanning.
		which_window = HANNING;
	else if (stopband_gain_dB > -53.0) //Hamming.
		which_window = HAMMING;
	else if (stopband_gain_dB > -74.0) //Blackman.
		which_window = BLACKMAN;
	else
  {
		printf("LPF generation failed.\r\nYou requested too much stopband attenuation.\r\n");
		printf("To fix this problem, increase the value you have for stopband_gain.\r\n");
		printf("A usual value is 0.01.\r\n");
	}
  
  // ret.
	return which_window;
}

dvec_o generate_window(WindowSelection which_window, int window_length)
{
	int n = 0;
	
	dvec_o Wn;
	Wn.len = window_length;
	Wn.arr = (double*)malloc(sizeof(double)*Wn.len);
	
	if (which_window == RECTANGULAR)
  {
		for (n = 0; n < Wn.len; n++)
			Wn.arr[n] = 1.0;
	}
	else if (which_window == HANNING)
  {
		for (n = 0; n < Wn.len; n++)
			Wn.arr[n] = 0.5 * (1.0 - cos((2.0 * M_PI * (double)(n)) / (Wn.len - 1)));
	}
	else if (which_window == HAMMING)
  {
		for (n = 0; n < Wn.len; n++)
			Wn.arr[n] = 0.54 - 0.46 * cos((2.0 * M_PI * (double)(n)) / (Wn.len - 1));
	}
	else if (which_window == BLACKMAN)
  {
		for (n = 0; n < Wn.len; n++)
			Wn.arr[n] = 0.42 - 0.5 * cos((2.0 * M_PI * (double)(n)) / (Wn.len - 1)) + 0.08 * cos((4.0 * M_PI * (double)(n)) / (Wn.len - 1));
	}
	else
		clear_dvec(Wn);
	return Wn;
}

int get_filter_length(double w_1, double w_2, double stopband_gain)
{
	double stopband_gain_dB = 20.0*log10(stopband_gain);
	double dw = fabs(w_1 - w_2);
	int M = 0;
	
	if (stopband_gain_dB > -21.0) //Rectangular.
		M = (int)(4.0*M_PI / dw);
	else if (stopband_gain_dB > -44.0) //Hanning.
		M = (int)(8.0*M_PI / dw);
	else if (stopband_gain_dB > -53.0) //Hamming.
		M = (int)(8.0*M_PI / dw);
	else if (stopband_gain_dB > -74.0) //Blackman.
		M = (int)(12.0*M_PI / dw);
	else
  { //Fail.
		printf("LPF generation failed.\r\nYou requested too much stopband attenuation.\r\n");
		printf("To fix this problem, increase the value you have for stopband_gain.\r\n");
		printf("A usual value is 0.01.\r\n");
		return 0;
	}
	if ((M % 2) == 0) //Filter length odd.
		M++;
	
	return M;
}

dvec_o generate_lowpass_impulse_response(int filter_length, double wc)
{
	if ((filter_length % 2) == 0) //Filter length odd.
		filter_length++;
	
	int n = 0;
	double wc_div_pi = wc / M_PI;
	
	dvec_o hn;
	hn.len = filter_length;
	hn.arr = (double*)malloc(sizeof(double)*hn.len);
	
	for (n = 0; n < hn.len; n++)
		hn.arr[n] = wc_div_pi * sinc(wc * (double)(n - ((hn.len - 1) / 2)));
	
	return hn;
}

dvec_o generate_highpass_impulse_response(int filter_length, double wc)
{
	if ((filter_length % 2) == 0)
		filter_length++;
	
	int n = 0;
	double wc_div_pi = wc / M_PI;
	
	dvec_o hn;
	hn.len = filter_length;
	hn.arr = (double*)malloc(sizeof(double)*hn.len);
	
	for (n = 0; n < hn.len; n++)
		hn.arr[n] = -wc_div_pi * sinc(wc * (double)(n - ((hn.len - 1) / 2)));
	
	hn.arr[(hn.len - 1) / 2] += 1.0;
	
	return hn;
}

dvec_o generate_lowpass_filter(double wL_1, double wL_2, double stopband_gain)
{
	double wc = (wL_1 + wL_2) / 2.0;
	
	dvec_o hn = generate_lowpass_impulse_response(get_filter_length(wL_1, wL_2, stopband_gain), wc);
	dvec_o Wn = generate_window(pick_window(stopband_gain), hn.len);
	
	hn = termbyterm_mul(hn, Wn);
	
	return hn;
}

dvec_o generate_highpass_filter(double wH_1, double wH_2, double stopband_gain)
{
	double wc = (wH_1 + wH_2) / 2.0;
	
	dvec_o hn = generate_highpass_impulse_response(get_filter_length(wH_1, wH_2, stopband_gain), wc);
	dvec_o Wn = generate_window(pick_window(stopband_gain), hn.len);
	
	hn = termbyterm_mul(hn, Wn);
	
	return hn;
}

dvec_o generate_bandstop_filter(double wL_1, double wL_2, double wH_1, double wH_2, double stopband_gain)
{
	dvec_o hnL = generate_lowpass_filter(wL_1, wL_2, stopband_gain);
	dvec_o hnH = generate_highpass_filter(wH_1, wH_2, stopband_gain);
	dvec_o hn = termbyterm_add(hnL, hnH);
	
	return hn;
}

dvec_o generate_bandpass_filter(double wL_1, double wL_2, double wH_1, double wH_2, double stopband_gain)
{
	dvec_o hnL = generate_lowpass_filter(wL_1, wL_2, stopband_gain);
	dvec_o hnH = generate_highpass_filter(wH_1, wH_2, stopband_gain);
	dvec_o hn = termbyterm_add(hnL, hnH);
	
	flipsign_d(hn);
	hn.arr[(hn.len - 1) / 2] += 1.0;
	
	return hn;
}

