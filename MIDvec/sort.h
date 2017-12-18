//MIDvec Library.
	//Machine learning.
	//Image development.
	//Digital signal processing.
	//Vectors.
		//(and more)

//Commercial license for MIDvec: Terms stated at Binpress site.
//Contact is available through the Binpress site.

#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cpx_vec.h"
#include "dsp.h"
#include "misc_math.h"
#include "ann.h"
#include "bitmap.h"

//Sort.
void insertion_sort(dvec_o xn) { //Insertion sort algorithm for arranging vector of double values in ascending order.
	int n = 0;
	int nc = 0;
	
	for (n = 1; n < xn.len; n++) {
		nc = n - 1;
		while (xn.arr[n] < xn.arr[nc])
			nc--;
		insert_intra_vec(xn, n, nc + 1);
	}
}
dvec_o selection_sort(dvec_o xn) { //Selection sort algorithm for arranging vector of double values in ascending order.
	int n = 0;
	int sub_min_ind = 0; //min of unsorted subarray
	
	for (n = 0; n < xn.len; n++) {
		sub_min_ind = min(xn, n, xn.len - 1);
		swap_intra_vec(xn, n, sub_min_ind);
	}
	return xn;
}

#endif