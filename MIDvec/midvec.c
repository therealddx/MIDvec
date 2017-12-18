//MIDvec Library.
	//Machine learning.
	//Image development.
	//Digital signal processing.
	//Vectors.
		//(and more)

//Commercial license for MIDvec: Terms stated at Binpress site.
//Contact is available through the Binpress site.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cpx_vec.h"
#include "dsp.h"
#include "misc_math.h"
#include "ann.h"
#include "bitmap.h"
#include "sort.h"
#include "blackjack.h"

//1. Specific operations for Particle in a Circle problem.
cpx_o calc_target_vN(double xp, double yp, double des_circle_radius) {
	//Based on location, put out desired normal velocity vector.
	
	//(xp, yp) to polar.
	cpx_o rp;
		rp.real = xp;
		rp.imag = yp;
	double rp_mag = mag_cpx(rp);
	double rp_phase = phase_cpx(rp);
	
	//New magnitude and original phase.
	double new_mag = des_circle_radius - rp_mag;
	cpx_o vN;
		vN.real = new_mag * cos(rp_phase);
		vN.imag = new_mag * sin(rp_phase);
	
	return vN;
}
cpx_o calc_target_vT(double xp, double yp) {
	//Based on location, put out desired tangential velocity vector.
	
	//(xp, yp) to polar.
		//New phase: Add pi/2.
	cpx_o rp;
		rp.real = xp;
		rp.imag = yp;
	double rp_mag = mag_cpx(rp);
	double rp_phase = phase_cpx(rp) + (M_PI / 2.0);
	
	//Original magnitude, new phase.
	cpx_o vT;
		vT.real = rp_mag * cos(rp_phase);
		vT.imag = rp_mag * sin(rp_phase);
	return vT;
}
cpx_o calc_target_vD(double xp, double yp, double des_circle_radius) {
	cpx_o vN = calc_target_vN(xp, yp, des_circle_radius);
	cpx_o vT = calc_target_vT(xp, yp);
	return cpx_add(vN, vT);
}
void draw_target_v(graph_o g, bmp_o bmp, cpx_o v, double xp, double yp, color_o cc) {
	//Draws v at (x,y).
	//Comes down to calling the draw_line_g() function.
		//What defines your line segment.
		//You have vX and vY. That's x1, y1.
		//You have 0 and 0. That's x2, y2.
		
		//Problem is you need to move the vector to the right by xp and up by yp.
		//So you need to actually say the points for that line segment you wanna draw are:
		//vX + xp and vY + yp -> (x1, y1).
		//xp and yp -> (x2, y2).
		
		//Of course, vX is v.real, and vY is v.imag.
		//And of course, this all happens in the environment characterized by graph object g
		//and bitmap object bmp, and desired color cc.
	
	line_segment_o lin;
		lin.x1 = v.real + xp;
		lin.y1 = v.imag + yp;
		lin.x2 = xp;
		lin.y2 = yp;
	draw_line_g(g, bmp, lin, cc);
	
	circle_o cir;
		cir.xc = lin.x1;
		cir.yc = lin.y1;
		cir.r = 0.01;
	draw_circle_g(g, bmp, cir, cc);
}

//Debug.
void init_vec(dvec_o vec, int option) { //Dummy function for filling out test vectors.
	int n = 0;
	int f = 0;
		double ff = 0;
		
	if (option == 0) {
		vec.arr[0] = -1;
		vec.arr[1] = -1;
		vec.arr[2] = 1;
		vec.arr[3] = 1;
		vec.arr[4] = 1;
		vec.arr[5] = -1;
		vec.arr[6] = -1;
		vec.arr[7] = 1;
		vec.arr[8] = -1;
		vec.arr[9] = -1;
	}
	else if (option == 1) {
		vec.arr[0] = 1;
		vec.arr[1] = 1;
		vec.arr[2] = 1;
		vec.arr[3] = -1;
		vec.arr[4] = -1;
		vec.arr[5] = 1;
		vec.arr[6] = -1;
	}
	else if (option == 2) {
		vec.arr[0] = 5;
		vec.arr[1] = 4;
		vec.arr[2] = 9;
		vec.arr[3] = 3;
		vec.arr[4] = 7;
		vec.arr[5] = 6;
		vec.arr[6] = 1;
		vec.arr[7] = 0;
		vec.arr[8] = 2;
		vec.arr[9] = 8;
	}
	else if (option == 3) {
		for (n = 0; n < vec.len; n++) {
			for (f = 0; f < 512; f++) {
				ff = (double)(f) * (8000.0 / 512.0);
				vec.arr[n] += cos(2.0 * M_PI * ff * (double)(n) / 8000.0);
			}
		}
	}
}

void main() {
	printf("Done");
	int n = 0;
	
	double fc = 915.0*pow(10,6); //915 MHz
	double Tc = 1.0/(fc);
	double Ts = 0.05*Tc; //5 samples per carrier wave
	
	double Tb = 10000.0*Tc; //1000 waves of carrier -> 1 bit
	double Ts1 = 0.018*Tb; //s1 is 5MHz
	double Ts2 = 0.01*Tb; //s2 is 9MHz
	
	double t = 0;
	
	//Assemble bits
	dvec_o s1 = new_dvec((int)(Tb / Ts));
	dvec_o s2 = new_dvec((int)(Tb / Ts));
	for (n = 0; n < s1.len; n++) {
		t = (double)(n)*Ts;
		s1.arr[n] = hard_limit(cos(2*M_PI*(1/Ts1)*t));
	}
	for (n = 0; n < s2.len; n++) {
		t = (double)(n)*Ts;
		s2.arr[n] = hard_limit(cos(2*M_PI*(1/Ts2)*t));
	}
	
	//Assemble carrier
	double timelength = 4.0*Tb;//pow(10,-7); //make this an even no
	dvec_o carrier = new_dvec((int)(timelength / Ts));
	for (n = 0; n < carrier.len; n++) {
		t = (double)(n) * Ts;
		carrier.arr[n] = cos(2.0*M_PI*fc*t);
	}
	
	//Assemble alternating bitstream
	int cur_bit = 0;
	int i = 0;
	int numbits = (int)(timelength / Tb);
	dvec_o bitstream = new_dvec( (int)(timelength / Ts) );
	
	for (cur_bit = 0; cur_bit < numbits; cur_bit++) {
		if (cur_bit % 2 == 0) {
			for (i = 0; (i < s1.len) && (i + cur_bit*s2.len < bitstream.len); i++)
				bitstream.arr[i + cur_bit*s1.len] = s1.arr[i];					
		}
		else if (cur_bit % 2 == 1) {
			for (i = 0; (i < s2.len) && (i + cur_bit*s2.len < bitstream.len); i++)
				bitstream.arr[i + cur_bit*s2.len] = s2.arr[i];
		}
	}
	
	dvec_o reflected_signal = termbyterm_mul(carrier, bitstream);
	dvec_o reflected_signal_FFT = magfft(upcast_dvec(reflected_signal), 262144*2);
	
	//Ok, so let's see if we can apply filters at 122.5 MHz and 225 MHz
	//to recover the bitstream.
	//double fs = 1.0/Ts; //4.5*10^9
	//double wL1 = 2.0*M_PI*400.0*pow(10,6)/fs;
	//double wL2 = 2.0*M_PI*600.0*pow(10,6)/fs;
	//dvec_o LPF = generate_lowpass_filter(wL1, wL2, 0.01);
	//dvec_o LPF_FFT = magfft(upcast_dvec(LPF));
	
	//dvec_o filtered_data = conv(reflected_signal, LPF);
	//dvec_o filtered_data_FFT = magfft(upcast_dvec(filtered_data));
	
	dvec_o downmixed_data = termbyterm_mul(carrier, reflected_signal);
	dvec_o downmixed_data_FFT = magfft(upcast_dvec(downmixed_data), 262144*2);
	
	//Test bit pattern will be 1-0-1-0...
	FILE* fout = fopen("fileout.csv","w");
	
	dvec_list_o FFTs = new_dvec_list(2, 262144*2);
	FFTs.arr[0] = downmixed_data_FFT;
	FFTs.arr[1] = reflected_signal_FFT;
	fprint_dvec_list(FFTs, fout);
	
	//fprintf(fout, "LPF:\r\n");
	//fprint_dvec(LPF, fout);
	//fprintf(fout, "LPF FFT:\r\n");
	//fprint_dvec(LPF_FFT, fout);
	
	//fprintf(fout, "carrier FFT:\r\n");
	//fprint_dvec(magfft(upcast_dvec(carrier)), fout);
	
	//fprintf(fout, "Subtone1:\r\n");
	//fprint_dvec(s1, fout);
	
	//fprintf(fout, "Subtone2:\r\n");
	//fprint_dvec(s2, fout);
	
	//fprintf(fout, "bitstream:\r\n");
	//fprint_dvec(bitstream, fout);
	
	//fprintf(fout, "reflected signal:\r\n");
	//fprint_dvec(reflected_signal, fout);
	//fprintf(fout, "reflected signal_FFT:\r\n");
	//fprint_dvec(reflected_signal_FFT, fout);
	
	//fprintf(fout, "downmixed_data\r\n");
	//fprint_dvec(downmixed_data, fout);
	//fprintf(fout, "downmixed_data_FFT\r\n");
	//fprint_dvec(downmixed_data_FFT, fout);
	
	//fprintf(fout, "filtered data:\r\n");
	//fprint_dvec(filtered_data, fout);
	//fprintf(fout, "filtered data FFT:\r\n");
	//fprint_dvec(filtered_data_FFT, fout);
	
	fclose(fout);
}