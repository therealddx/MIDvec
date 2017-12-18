//MIDvec Library.
	//Machine learning.
	//Image development.
	//Digital signal processing.
	//Vectors.
		//(and more)

//Commercial license for MIDvec: Terms stated at Binpress site.
//Contact is available through the Binpress site.

#ifndef CPX_VEC_H
#define CPX_VEC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Objects.
typedef struct cpx {
	double real;
	double imag;
} cpx_o;

typedef struct bvec {
	unsigned char* arr;
	int len;
} bvec_o;
typedef struct ivec {
	int* arr;
	int len;
} ivec_o;
typedef struct dvec {
	double* arr;
	int len;
} dvec_o;
typedef struct cpx_vec {
	cpx_o* arr;
	int len;
} cpx_vec_o;

typedef struct bvec_list {
	bvec_o* arr;
	int len;
} bvec_list_o;
typedef struct ivec_list {
	ivec_o* arr;
	int len;
} ivec_list_o;
typedef struct dvec_list {
	dvec_o* arr;
	int len;
} dvec_list_o;
typedef struct cpx_vec_list {
	cpx_vec_o* arr;
	int len;
} cpx_vec_list_o;

//Constructors.
cpx_o new_cpx_polar(double mag, double phase) {
	cpx_o z;
	z.real = mag*cos(phase);
	z.imag = mag*sin(phase);
	return z;
}
cpx_o new_cpx_rect(double r, double i) {
	cpx_o z;
	z.real = r;
	z.imag = i;
	return z;
}

bvec_o new_bvec(int length) {
	bvec_o bn;
	bn.len = length;
	bn.arr = (unsigned char*)malloc(sizeof(unsigned char)*bn.len);
	
	return bn;
}
ivec_o new_ivec(int length) {
	ivec_o xn;
	xn.len = length;
	xn.arr = (int*)malloc(sizeof(int)*xn.len);
	
	return xn;
}
dvec_o new_dvec(int length) {
	dvec_o xn;
	xn.len = length;
	xn.arr = (double*)malloc(sizeof(double)*xn.len);
	
	return xn;
}
cpx_vec_o new_cpx_vec(int length) {
	cpx_vec_o xn;
	xn.len = length;
	xn.arr = (cpx_o*)malloc(sizeof(cpx_o)*xn.len);
	
	return xn;
}

bvec_list_o new_bvec_list(int num_vecs, int vec_length) {
	
	bvec_list_o list;
	list.len = num_vecs;
	list.arr = (bvec_o*)malloc(sizeof(bvec_o) * list.len);
	
	int n = 0;
	for (n = 0; n < list.len; n++)
		list.arr[n] = new_bvec(vec_length);
	
	return list;
}
ivec_list_o new_ivec_list(int num_vecs, int vec_length) {
	ivec_list_o list;
	list.len = num_vecs;
	list.arr = (ivec_o*)malloc(sizeof(ivec_o) * list.len);
	
	int n = 0;
	for (n = 0; n < list.len; n++)
		list.arr[n] = new_ivec(vec_length);
	
	return list;
}
dvec_list_o new_dvec_list(int num_vecs, int vec_length) {
	
	//number of vectors == num_vecs == list.len
	//length of vectors == vec_length == list.arr[0].len
	
	dvec_list_o list;
	list.len = num_vecs;
	list.arr = (dvec_o*)malloc(sizeof(dvec_o) * list.len);
	
	int n = 0;
	for (n = 0; n < list.len; n++)
		list.arr[n] = new_dvec(vec_length);
	
	return list;
}
cpx_vec_list_o new_cpx_vec_list(int num_vecs, int vec_length) {
	
	cpx_vec_list_o list;
	list.len = num_vecs;
	list.arr = (cpx_vec_o*)malloc(sizeof(cpx_vec_o) * list.len);
	
	int n = 0;
	for (n = 0; n < list.len; n++)
		list.arr[n] = new_cpx_vec(vec_length);
	
	return list;
}

dvec_o cpx_to_vec(cpx_o z) {
	dvec_o v = new_dvec(2);
	v.arr[0] = z.real;
	v.arr[1] = z.imag;
	return v;
}
cpx_o vec_to_cpx(dvec_o v) {
	cpx_o z;
	z.real = v.arr[0];
	z.imag = v.arr[1];
	return z;
}

//Complex number operations.
double mag_cpx(cpx_o a) { //Find the magnitude of a complex number.
	return sqrt(a.real*a.real + a.imag*a.imag);
}
double mag2_cpx(cpx_o a) { //Find magnitude^2 of a complex number.
	return a.real*a.real + a.imag*a.imag;
}
double phase_cpx(cpx_o a) { //Find the phase of a complex number.
	
	//Handle zero exceptions.
	if ((a.real == 0) && (a.imag == 0))
		return 0;
	else if (a.real == 0)
		if (a.imag > 0)
			return M_PI / 2.0;
		else if (a.imag < 0)
			return 3.0 * M_PI / 2.0;
	else if (a.imag == 0)
		if (a.real > 0)
			return 0;
		else if (a.real < 0)
			return M_PI;
	
	//Handles normal numbers for any quadrant.
	if ((a.real > 0) && (a.imag > 0)) { //quadrant 1
		return atan(a.imag / a.real);
	}
	if ((a.real < 0) && (a.imag > 0)) { //quadrant 2
		return atan(a.imag / a.real) + M_PI;
	}
	if ((a.real < 0) && (a.imag < 0)) { //quadrant 3
		return atan(a.imag / a.real) + M_PI;
	}
	if ((a.real > 0) && (a.imag < 0)) { //quadrant 4
		return atan(a.imag / a.real) + 2.0 * M_PI;
	}
}

cpx_o cpx_scale(cpx_o a, double c) { //Multiply a complex number by a scalor.
	a.real *= c;
	a.imag *= c;
	return a;
}
cpx_vec_o cpx_vec_scale(cpx_vec_o xn, double c) { //Multiply a vector of complex numbers by a scalor.
	int n = 0;
	for (n = 0; n < xn.len; n++)
		xn.arr[n] = cpx_scale(xn.arr[n], c);
	return xn;
}

cpx_vec_o upcast_dvec(dvec_o xn) { //Store a vector of double values as the real components to a vector of complex numbers.
	cpx_vec_o xn_cpx;
	xn_cpx.len = xn.len;
	xn_cpx.arr = (cpx_o*)malloc(sizeof(cpx_o)*xn_cpx.len);
	
	int n = 0;
	for (n = 0; n < xn_cpx.len; n++) {
		xn_cpx.arr[n].real = xn.arr[n];
		xn_cpx.arr[n].imag = 0;
	}
	return xn_cpx;
}
dvec_o downcast_cpx(cpx_vec_o zn, char* select) {
	int n = 0;
	dvec_o vn = new_dvec(zn.len);
	
	if (!strcmp(select, "real")) {
		for (n = 0; n < zn.len; n++)
			vn.arr[n] = zn.arr[n].real;
	}
	else if (!strcmp(select, "imag")) {
		for (n = 0; n < zn.len; n++)
			vn.arr[n] = zn.arr[n].imag;
	}
	else if (!strcmp(select, "mag")) {
		for (n = 0; n < zn.len; n++)
			vn.arr[n] = mag_cpx(zn.arr[n]);
	}
	else if (!strcmp(select, "mag2")) {
		for (n = 0; n < zn.len; n++)
			vn.arr[n] = mag2_cpx(zn.arr[n]);
	}
	else if (!strcmp(select, "phase")) {
		for (n = 0; n < zn.len; n++)
			vn.arr[n] = phase_cpx(zn.arr[n]);
	}
	return vn;
}

cpx_o cpx_add(cpx_o a, cpx_o b) { //Returns a + b.
	cpx_o z;
	z.real = a.real + b.real;
	z.imag = a.imag + b.imag;
	return z;
}
cpx_o cpx_sub(cpx_o a, cpx_o b) { //Returns a - b.
	cpx_o z;
	z.real = a.real - b.real;
	z.imag = a.imag - b.imag;
	return z;
}
cpx_o cpx_mul(cpx_o a, cpx_o b) { //Returns a * b.
	cpx_o z;
	z.real = a.real*b.real - a.imag*b.imag;
	z.imag = b.imag*a.real + a.imag*b.real;
	return z;
}
cpx_o cpx_div(cpx_o a, cpx_o b) { //Returns a / b.
	cpx_o z;
	z.real = a.real * b.real + a.imag * b.imag;
	z.imag = -b.imag * a.real + a.imag * b.real;
	z = cpx_scale(z, 1.0 / (b.real * b.real + b.imag * b.imag));
	return z;
}
cpx_o cpx_recip(cpx_o a) { //Returns 1 / a.
	cpx_o z;
	z.real = a.real / (a.real * a.real + a.imag * a.imag);
	z.imag = -a.imag / (a.real * a.real + a.imag * a.imag);
	return z;
}
cpx_o cpx_conj(cpx_o a) {
	cpx_o z;
	z.real = a.real;
	z.imag = -a.imag;
	return z;
}

dvec_o mag_cpx_vec(cpx_vec_o a) {
	int n = 0;
	dvec_o out = new_dvec(a.len);
	for (n = 0; n < a.len; n++)
		out.arr[n] = mag_cpx(a.arr[n]);
	return out;
}

//Vector operations.
//1: Printing.
//1.1: Printing to console.
void print_bvec(bvec_o bb) {
	int i = 0;
	for (i = 0; i < bb.len; i++)
		printf("%d, %d\r\n", i, (unsigned char)bb.arr[i]);
}
void print_ivec(ivec_o v) {
	int i = 0;
	for (i = 0; i < v.len; i++)
		printf("%d, %d\r\n", i, v.arr[i]);
	//printf("\r\n");
}
void print_dvec(dvec_o v) { //Print all terms of a vector of doubles.
	int i = 0;
	for (i = 0; i < v.len; i++)
		printf("%d, %.25lf\r\n", i, v.arr[i]);
	//printf("\r\n");
}
void print_cpx_vec(cpx_vec_o xn) { //Print all terms of a vector of complex nums.
	int n = 0;
	for (n = 0; n < xn.len; n++)
		printf("%d, %lf, %lf, %lf, %lf\r\n", n, xn.arr[n].real, xn.arr[n].imag, mag_cpx(xn.arr[n]), phase_cpx(xn.arr[n]));
}

//1.2: Printing to file.
void fprint_bvec(bvec_o bb, FILE* fout) {
	int i = 0;
	for (i = 0; i < bb.len; i++)
		fprintf(fout, "%d: %d\r\n", i, (unsigned char)bb.arr[i]);
}
void fprint_ivec(ivec_o v, FILE* fout) {
	int i = 0;
	for (i = 0; i < v.len; i++)
		fprintf(fout, "%d: %d\r\n", i, v.arr[i]);
}
void fprint_dvec(dvec_o v, FILE* fout) {
	int i = 0;
	for (i = 0; i < v.len; i++)
		fprintf(fout, "%d, %lf,\r\n", i, v.arr[i]);
}
void fprint_dvec_list(dvec_list_o vlist, FILE* fout) {
	//For each value in vlist
		//for each vector in vlist
	int val_ind = 0;
	int vec_ind = 0;
	for (val_ind = 0; val_ind < vlist.arr[0].len; val_ind++) {
		fprintf(fout, "%d, ", val_ind);
		for (vec_ind = 0; vec_ind < vlist.len; vec_ind++)
			fprintf(fout, "%lf, ", vlist.arr[vec_ind].arr[val_ind]);
		fprintf(fout, "\r\n");
	}
}
void fprint_cpx_vec(cpx_vec_o xn, FILE* fout) { //Print all terms of a vector of complex nums.
	int n = 0;
	for (n = 0; n < xn.len; n++)
		fprintf(fout, "%d: %lf, %lf, %lf, %lf\r\n", n, xn.arr[n].real, xn.arr[n].imag, mag_cpx(xn.arr[n]), phase_cpx(xn.arr[n]));
}

//2: Trivial memory management.
//2.1: Copying.
void copy_bvec(bvec_o src, bvec_o dst) {
	int n = 0;
	for (n = 0; n < src.len; n++)
		dst.arr[n] = src.arr[n];
}
void copy_ivec(ivec_o src, ivec_o dst) {
	int n = 0;
	for (n = 0; n < src.len; n++)
		dst.arr[n] = src.arr[n];
}
void copy_dvec(dvec_o src, dvec_o dst) {
	int n = 0;
	for (n = 0; n < src.len; n++)
		dst.arr[n] = src.arr[n];
}
void copy_cpx_vec(cpx_vec_o src, cpx_vec_o dst) {
	int n = 0;
	for (n = 0; n < src.len; n++)
		dst.arr[n] = src.arr[n];
}

//2.2: Clearing.
void clear_bvec(bvec_o vec) {
	int n = 0;
	for (n = 0; n < vec.len; n++)
		vec.arr[n] = 0;
}
void clear_ivec(ivec_o vec) {
	int n = 0;
	for (n = 0; n < vec.len; n++)
		vec.arr[n] = 0;
}
void clear_dvec(dvec_o vec) {
	int n = 0;
	for (n = 0; n < vec.len; n++)
		vec.arr[n] = 0;
}
void clear_cpx_vec(cpx_vec_o vec) {
	int n = 0;
	for (n = 0; n < vec.len; n++) {
		vec.arr[n].real = 0;
		vec.arr[n].imag = 0;
	}
}

//2.3 Zeropadding.
bvec_o zeropad_bvec(bvec_o vec, int new_length) {
	int n = 0;
	
	bvec_o vec_out = new_bvec(new_length);
	
	for (n = 0; n < vec.len; n++)
		vec_out.arr[n] = vec.arr[n];
	for (n = vec.len; n < vec_out.len; n++)
		vec_out.arr[n] = 0;
		
	return vec_out;
}
ivec_o zeropad_ivec(ivec_o vec, int new_length) {
	int n = 0;
	
	ivec_o vec_out = new_ivec(new_length);
	
	for (n = 0; n < vec.len; n++)
		vec_out.arr[n] = vec.arr[n];
	for (n = vec.len; n < vec_out.len; n++)
		vec_out.arr[n] = 0;
		
	return vec_out;
}
dvec_o zeropad_dvec(dvec_o vec, int new_length) {
	int n = 0;
	
	dvec_o vec_out = new_dvec(new_length);
	
	for (n = 0; n < vec.len; n++)
		vec_out.arr[n] = vec.arr[n];
	for (n = vec.len; n < vec_out.len; n++)
		vec_out.arr[n] = 0;
		
	return vec_out;
}
cpx_vec_o zeropad_cpx(cpx_vec_o vec, int new_length) {
	int n = 0;
	
	cpx_vec_o cpx_vec_out = new_cpx_vec(new_length);
	
	for (n = 0; n < vec.len; n++) {
		cpx_vec_out.arr[n].real = vec.arr[n].real;
		cpx_vec_out.arr[n].imag = vec.arr[n].imag;
	}
	for (n = vec.len; n < cpx_vec_out.len; n++) {
		cpx_vec_out.arr[n].real = 0;
		cpx_vec_out.arr[n].imag = 0;
	}
	
	return cpx_vec_out;
}

//3: Intra-vector comparison.
//3.1: Min and max location.
int min(dvec_o xn, int start_ind, int end_ind) { //Find index of min value in vector of doubles.
	//Searches for n == [start_ind, end_ind].
	//Returns index of minimum.
	
	int n = 0;
	double min = 10000000000000000.0;
	int min_ind = -1;
	for (n = start_ind; n <= end_ind; n++) {
		if (xn.arr[n] < min) {
			min = xn.arr[n];
			min_ind = n;
		}
	}
	return min_ind;
}
int max(dvec_o xn, int start_ind, int end_ind) { //Find index of max value in vector of doubles.
	//Searches for n == [start_ind, end_ind].
	//Returns index of maximum.
	
	int n = 0;
	double max = -10000000000000000.0;
	int max_ind = -1;
	for (n = start_ind; n <= end_ind; n++) {
		if (xn.arr[n] > max) {
			max = xn.arr[n];
			max_ind = n;
		}
	}
	return max_ind;
}
int min_abs(dvec_o xn, int start_ind, int end_ind) { //Find index of minimum absolute value in vector of doubles.
	//Searches for n == [start_ind, end_ind].
	//Returns index of term that has the smallest absolute value.
	
	int n = 0;
	double min = 10000000000000000.0;
	int min_ind = -1;
	for (n = start_ind; n <= end_ind; n++) {
		if (fabs(xn.arr[n]) < min) {
			min = fabs(xn.arr[n]);
			min_ind = n;
		}
	}
	return min_ind;
}
int max_abs(dvec_o xn, int start_ind, int end_ind) { //Find index of maximum absolute value in vector of doubles.
	//Searches for n == [start_ind, end_ind].
	//Returns index of term that has maximum absolute value.
	
	int n = 0;
	double max = -10000000000000000.0;
	int max_ind = -1;
	for (n = start_ind; n <= end_ind; n++) {
		if (fabs(xn.arr[n]) > max) {
			max = fabs(xn.arr[n]);
			max_ind = n;
		}
	}
	return max_ind;
}

//3.2: Number equivalent.
int numequiv_b(bvec_o x1, bvec_o x2) { //Return the amount of indices where x1[n] equals x2[n].
	if (x1.len != x2.len)
		return 0;
	
	int n = 0;
	int num_equiv = 0;
	for (n = 0; n < x1.len; n++) {
		if (x1.arr[n] == x2.arr[n])
			num_equiv++;
	}
	return num_equiv;
}
int numequiv_i(ivec_o x1, ivec_o x2) { //Return the amount of indices where x1[n] equals x2[n].
	if (x1.len != x2.len)
		return 0;
	
	int n = 0;
	int num_equiv = 0;
	for (n = 0; n < x1.len; n++) {
		if (x1.arr[n] == x2.arr[n])
			num_equiv++;
	}
	return num_equiv;
}

//4. Indexing.
void insert_intra_vec(dvec_o xn, int src_ind, int dst_ind) { //Pull out xn[src_ind] and shift terms around so you can put it in the place of xn[dst_ind].
	int n = 0;
	int temp = xn.arr[src_ind];
	
	//Shift everything right.
	if (src_ind > dst_ind) {
		for (n = src_ind; n >= dst_ind + 1; n--)
			xn.arr[n] = xn.arr[n - 1];
		xn.arr[dst_ind] = temp;
	}
	//Shift everything left.
	else if (src_ind < dst_ind) {
		for (n = src_ind; n < dst_ind; n++)
			xn.arr[n] = xn.arr[n + 1];
		xn.arr[dst_ind] = temp;
	}
	
	//Test to insert xn[a] into xn[b].
		//xn[a] => temp.
		//From k == a, say that xn[k] = xn[k - 1], k--, until k == b + 1.
		//xn[b] = temp.
	
	//Test to insert xn[a] into xn[b].
		//xn[a] => temp.
		//From k == a, say that xn[k] = xn[k + 1], k++, until k == b.
		//xn[b] = temp.
}
void swap_intra_vec(dvec_o xn, int n1, int n2) { //xn[n1] gets the value at xn[n2] and xn[n2] gets the value at xn[n1].
	double temp = xn.arr[n1];
	xn.arr[n1] = xn.arr[n2];
	xn.arr[n2] = temp;
}

bvec_o vecinsert_b(bvec_o b_dst, bvec_o b_src, int offset) {
	bvec_o b_out = new_bvec(b_dst.len + b_src.len);
	
	int n = 0;
	for (n = 0; n < offset; n++)
		b_out.arr[n] = b_dst.arr[n];
	for (n = offset; n < offset + b_src.len; n++)
		b_out.arr[n] = b_src.arr[n - offset];
	for (n = offset + b_src.len; n < b_out.len; n++)
		b_out.arr[n] = b_dst.arr[n - b_src.len];
	
	return b_out;
	
	//Why.
		//Because, b_out.len - b_src.len = b_dst.len.
		//So b_out goes from offset + src.len to b_out.len.
		//And b_dst goes from offset (most recent excluded index) to b_dst.len.
	
	//Make a new vector b_out.
		//It is identical to b_dst for indices n = [0, offset - 1].
		//It is identical to b_src for indices n = [offset, offset + src.len - 1].
		//It is identical to the rest of b_dst for indices n = [offset + src.len - 1, b_out.len - 1].
}
void vecoverwrite_b(bvec_o b_dst, bvec_o b_src, int offset) {
	int n = 0;
	for (n = 0; n < b_src.len; n++)
		b_dst.arr[n + offset] = b_src.arr[n];
}

dvec_o subvecfromvec_d(dvec_o in, int start, int end) {
	int n = 0;
	dvec_o out = new_dvec(end - start + 1);
	for (n = start; n <= end; n++)
		out.arr[n - start] = in.arr[n];
	return out;
}

//5. Menial math operations.
void flipsign_i(ivec_o vec) {
	int n = 0;
	for (n = 0; n < vec.len; n++)
		vec.arr[n] *= -1.0;
}
void flipsign_d(dvec_o vec) {
	int n = 0;
	for (n = 0; n < vec.len; n++)
		vec.arr[n] *= -1.0;
}
void flipsign_cpx(cpx_vec_o vec) {
	int n = 0;
	for (n = 0; n < vec.len; n++) {
		vec.arr[n].real *= -1.0;
		vec.arr[n].imag *= -1.0;
	}
}

double norm_vec(dvec_o vec) {
	int n = 0;
	double norm = 0;
	for (n = 0; n < vec.len; n++)
		norm += vec.arr[n] * vec.arr[n];
	return sqrt(norm);
}
dvec_o generate_linear_vec(int num_samples, double low, double high) {
	dvec_o x = new_dvec(num_samples);
	
	int n = 0;
	for (n = 0; n < x.len; n++)
		x.arr[n] = ((double)n / (double)(num_samples - 1)) * (high - low) + low;
	return x;
}
dvec_o generate_linear_vec2(double stepsize, double low, double high, int include_end) {
	dvec_o x = new_dvec( (int)((high - low) / stepsize) + include_end );
	
	int n = 0;
	for (n = 0; n < x.len; n++) {
		x.arr[n] = low + (double)(n) * stepsize;
	}
	
	return x;
}

double sum_d(dvec_o v) {
	int n = 0;
	double sum = 0;
	for (n = 0; n < v.len; n++)
		sum += v.arr[n];
	return sum;
}
double avg_d(dvec_o v) {
	return sum_d(v) / (double)(v.len);
}

//6. Term by term vector operations.
dvec_o termbyterm_add(dvec_o x1n, dvec_o x2n) {
	if (x1n.len != x2n.len) {
		printf("Error in termbyterm_add. Vectors inequal in length.\r\n");
		return;
	}
	
	int n = 0;
	dvec_o yn;
	yn.len = x1n.len;
	yn.arr = (double*)malloc(sizeof(double)*yn.len);
	
	for (n = 0; n < yn.len; n++)
		yn.arr[n] = x1n.arr[n] + x2n.arr[n];
	
	return yn;
}
dvec_o termbyterm_mul(dvec_o x1n, dvec_o x2n) {
	if (x1n.len != x2n.len) {
		printf("Error in termbyterm_mul. Vectors inequal in length.\r\n");
		return;
	}
	
	int n = 0;
	dvec_o yn;
	yn.len = x1n.len;
	yn.arr = (double*)malloc(sizeof(double)*yn.len);
	
	for (n = 0; n < yn.len; n++)
		yn.arr[n] = x1n.arr[n] * x2n.arr[n];
	
	return yn;
}
dvec_o termbyterm_sub(dvec_o x1n, dvec_o x2n) {
	if (x1n.len != x2n.len) {
		printf("Error in termbyterm_sub. Vectors inequal in length.\r\n");
		return;
	}
	
	int n = 0;
	dvec_o yn;
	yn.len = x1n.len;
	yn.arr = (double*)malloc(sizeof(double)*yn.len);
	
	for (n = 0; n < yn.len; n++)
		yn.arr[n] = x1n.arr[n] - x2n.arr[n];
	
	return yn;
}
dvec_o termbyterm_div(dvec_o x1n, dvec_o x2n) {
	if (x1n.len != x2n.len) {
		printf("Error in termbyterm_div. Vectors inequal in length.\r\n");
		return;
	}
	
	int n = 0;
	dvec_o yn;
	yn.len = x1n.len;
	yn.arr = (double*)malloc(sizeof(double)*yn.len);
	
	for (n = 0; n < yn.len; n++)
		yn.arr[n] = x1n.arr[n] / x2n.arr[n];
	
	return yn;
}

//7. File IO.
bvec_o datatovec_b(char* filename, int length) {
	bvec_o xx = new_bvec(length);
	
	FILE* fin = fopen(filename, "r");
	
	int n = 0;
	for (n = 0; n < length; n++)
		fscanf(fin, "%c\r\n", &(xx.arr[n]));
	
	return xx;
}
ivec_o datatovec_i(char* filename, int length) {
	ivec_o xx = new_ivec(length);
	
	FILE* fin = fopen(filename, "r");
	
	int n = 0;
	for (n = 0; n < length; n++)
		fscanf(fin, "%d\r\n", &(xx.arr[n]));
	
	return xx;
}
dvec_o datatovec_d(char* filename, int length) {
	dvec_o xx = new_dvec(length);
	
	FILE* fin = fopen(filename, "r");
	
	int n = 0;
	for (n = 0; n < length; n++)
		fscanf(fin, "%lf\r\n", &(xx.arr[n]));
	
	return xx;
}

#endif