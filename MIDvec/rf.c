#include "cpx_vec.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define SPEEDOFLIGHT 299792458

enum matrix_state {
	sPARAM,
	zPARAM,
	yPARAM,
	hPARAM,
}

typedef struct twoportnetwork {
	cpx_o v11;
	cpx_o v12;
	cpx_o v21;
	cpx_o v22;
	matrix_state ms;
} twoportnetwork_o;

//We make a two-port network. That's ok.
	//We do, we have the values, ok.

void designbpf_ccssr(double w0, double delta, int N, dvec_o g, double Zo) {
	//Design BPF Capacitively Coupled Shunt Shorted Resonators.
	
	double lambda = SPEEDOFLIGHT/(w0/(2.0*M_PI));
	
	//Calulate J and C values
	dvec_o J = new_dvec(N + 1); //J.arr[n] is J_(n,n+1).
	dvec_o C = new_dvec(N + 1); //C.arr[n] is C_(n,n+1).
	dvec_o dC = new_dvec(N); //n == 0 represents first change in cap value, n == N-1 represents last.
	dvec_o ln = new_dvec(N); //n == 0 represents first t-line, n == N-1 represents last.
	
	J.arr[0] = (1.0/Zo) * sqrt((M_PI * delta) / (4.0*g.arr[1]));
	int n = 0;
	for (n = 1; n < N; n++)
		J.arr[n] = (1.0/Zo) * (M_PI * delta) / (4.0*sqrt(g.arr[n]*g.arr[n+1]));
	J.arr[N] = (1.0 / Zo) * sqrt((M_PI*delta) / (4.0 * g.arr[N] * g.arr[N+1]));
	
	C.arr[0] = J.arr[0] / (w0 * sqrt( 1.0 - (Zo*J.arr[0])*(Zo*J.arr[0]) ) );
	for (n = 1; n < N; n++)
		C.arr[n] = J.arr[n] / w0;
	C.arr[N] = J.arr[N] / (w0 * sqrt( 1.0 - (Zo*J.arr[N])*(Zo*J.arr[N]) ) );
	
	printf("J is:\r\n");
	print_dvec(J);
	
	printf("\r\nC is: \r\n");
	print_dvec(C);
	
	for (n = 1; n < N+1; n++) //dC sees indices [0,N-1]. 
		dC.arr[n-1] = -C.arr[n-1] + -C.arr[n];
	for (n = 0; n < N; n++)
		ln.arr[n] = 0.25*lambda + (Zo * w0 * dC.arr[n]) / (2.0*M_PI) * lambda;
	
	printf("\r\ndC is:\r\n");
	print_dvec(dC);
	
	printf("\r\nln is:\r\n");
	print_dvec(ln);
	
	//Example 3rd order ckt:
	//ln[x] is a shunt shorted line
	//s--C[01]--ln[1]--C[12]--ln[2]--C[23]--ln[3]--C[34]--l
}

void main() {
	int N = 4;
	dvec_o g = new_dvec(N + 1);
		g.arr[0] = 1;
		g.arr[1] = 1.5963;
		g.arr[2] = 1.0967;
		g.arr[3] = 1.5962;
		g.arr[4] = 1;
	
	designbpf_ccsr(2*M_PI*2.5*pow(10,9),0.10,3,g,50);
}


//0.004436267029905 / (3*10^9 * sqrt( 1.0 - (50*0.004436267029905)*(50*0.004436267029905) ) );





































