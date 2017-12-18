//MIDvec Library.
	//Machine learning.
	//Image development.
	//Digital signal processing.
	//Vectors.
		//(and more)

//Commercial license for MIDvec: Terms stated at Binpress site.
//Contact is available through the Binpress site.

#ifndef ANN_H
#define ANN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cpx_vec.h"
#include "misc_math.h"
#include "dsp.h"
#include "bitmap.h"
#include "sort.h"

double rand_given_bound(double lo, double hi);
double sigmoid(double x);

//ANN.
typedef struct node {
	double output;
	
	dvec_o weight;
	dvec_o prev_dw;
	double bias;
	double prev_db;
	
	double error;
} node_o;
typedef struct layer {
	node_o* arr_nodes;
	int num_nodes;
} layer_o;
typedef struct ann {
	layer_o* arr_layers;
	int num_layers;
	
	double learning_rate;
	double momentum_factor;
} ann_o;

//Initialize.
void init_weights_and_biases_ann(ann_o ann) {
	int l = 0;
	int n = 0;
	int w = 0;
	
	for (l = 1; l < ann.num_layers; l++) { //For each layer.
		for (n = 0; n < ann.arr_layers[l].num_nodes; n++) { //For each node.
			for (w = 0; w < ann.arr_layers[l].arr_nodes[n].weight.len; w++) { //For each weight and bias.
				ann.arr_layers[l].arr_nodes[n].weight.arr[w] = rand_given_bound(-0.5, 0.5);
				ann.arr_layers[l].arr_nodes[n].prev_dw.arr[w] = 0;
			}
			ann.arr_layers[l].arr_nodes[n].bias = rand_given_bound(-0.5, 0.5);
			ann.arr_layers[l].arr_nodes[n].prev_db = 0;
		}
	}
}
ann_o new_ann_o(ivec_o nodes_per_layer) {
	ann_o ann;
	
	//Make ANN out of layers.
	ann.num_layers = nodes_per_layer.len;
	ann.arr_layers = (layer_o*)malloc(sizeof(layer_o) * ann.num_layers);
	
	int l = 0;
	int n = 0;
	
	//Make layers out of nodes.
	for (l = 0; l < ann.num_layers; l++) {
		ann.arr_layers[l].num_nodes = nodes_per_layer.arr[l];
		ann.arr_layers[l].arr_nodes = (node_o*)malloc(sizeof(node_o) * ann.arr_layers[l].num_nodes);
	}
	
	//Make nodes out of weight vectors. Ignore input layer, which is passive.
	for (l = 1; l < ann.num_layers; l++) {
		for (n = 0; n < ann.arr_layers[l].num_nodes; n++) {
			ann.arr_layers[l].arr_nodes[n].weight = new_dvec(nodes_per_layer.arr[l - 1]);
			ann.arr_layers[l].arr_nodes[n].prev_dw = new_dvec(nodes_per_layer.arr[l - 1]);
		}
	}
	
	init_weights_and_biases_ann(ann);
	ann.learning_rate = 0.05;
	ann.momentum_factor = 0.05;
	
	return ann;
}

//Operate.
void feed_forward_ann(ann_o ann) {
	int l = 0;
	int n = 0;
	int w = 0;
	
	for (l = 1; l < ann.num_layers; l++) { //For each layer.
		for (n = 0; n < ann.arr_layers[l].num_nodes; n++) { //For each node in that layer.
			for (w = 0; w < ann.arr_layers[l].arr_nodes[n].weight.len; w++) { //For each weight in that node.
				ann.arr_layers[l].arr_nodes[n].output += ann.arr_layers[l].arr_nodes[n].weight.arr[w] * ann.arr_layers[l - 1].arr_nodes[w].output;
				//printf("Layer %d, Node %d += Weight %d * Output from Layer %d, Node %d.\r\n", l, n, w, l - 1, w);
				//printf("Intermediate output: %lf\r\n\r\n", ann.arr_layers[l].arr_nodes[n].output);
			}
			ann.arr_layers[l].arr_nodes[n].output += ann.arr_layers[l].arr_nodes[n].bias;
			//printf("Add on %lf bias for final output for Layer %d, Node %d: %lf\r\n", ann.arr_layers[l].arr_nodes[n].bias, l, n, ann.arr_layers[l].arr_nodes[n].output);
			//printf("\r\n****Done with Node %d****\r\n\r\n", n);
			ann.arr_layers[l].arr_nodes[n].output = sigmoid(ann.arr_layers[l].arr_nodes[n].output);
		}
		//printf("\r\n\r\n****Done with Layer %d****\r\n\r\n", l);
	}
}
int backpropagate_adjust(ann_o ann, dvec_o targets) {
	//Output layer.
	int l = 0; //Iterating over layers of a net.
	
	int n = 0; //Iterating over nodes of a layer
	int w = 0; //Iterating over weights of a node.
	int k = 0; //Iterating over nodes to find a backpropagated error.
	
	int l_output = ann.num_layers - 1; //index of output layer.
	
	double delta = 0; //Final adjustment amount.
	
	//Adjust output nodes.
	if (targets.len != ann.arr_layers[l_output].num_nodes) //# targets == # output nodes.
		return 0;
	for (n = 0; n < ann.arr_layers[l_output].num_nodes; n++) { //For each output node.
	
		ann.arr_layers[l_output].arr_nodes[n].error = targets.arr[n] - ann.arr_layers[l_output].arr_nodes[n].output; //Error is target - output.
		
		for (w = 0; w < ann.arr_layers[l_output].arr_nodes[n].weight.len; w++) { //For each weight of that output node.
			
			delta = ann.learning_rate * ann.arr_layers[l_output].arr_nodes[n].error * ann.arr_layers[l_output - 1].arr_nodes[w].output; //dC/dw is error*activation on this weight.
			delta += ann.momentum_factor * ann.arr_layers[l_output].arr_nodes[n].prev_dw.arr[w]; //Add the momentum factor
			
			ann.arr_layers[l_output].arr_nodes[n].weight.arr[w] += delta; //Adjust weight.
			ann.arr_layers[l_output].arr_nodes[n].prev_dw.arr[w] = delta; //Store weight change.
		}
		
		delta = ann.learning_rate * ann.arr_layers[l_output].arr_nodes[n].error; //dC/db is error.
		delta += ann.momentum_factor * ann.arr_layers[l_output].arr_nodes[n].prev_db; //Add the momentum factor.
		ann.arr_layers[l_output].arr_nodes[n].bias += delta; //Adjust bias.
		ann.arr_layers[l_output].arr_nodes[n].prev_db = delta; //Store bias change.
	}
	
	//Adjust hidden nodes.
	for (l = l_output - 1; l > 0; l--) {
		for (n = 0; n < ann.arr_layers[l].num_nodes; n++) { //For each node in this layer.
			
			//Find the error for that node.
			for (k = 0; k < ann.arr_layers[l + 1].num_nodes; k++) //Take every node in the layer l + 1. Error times weight plus error times weight plus...
				ann.arr_layers[l].arr_nodes[n].error += ann.arr_layers[l + 1].arr_nodes[k].error * ann.arr_layers[l + 1].arr_nodes[k].weight.arr[n];
			ann.arr_layers[l].arr_nodes[n].error *= ann.arr_layers[l].arr_nodes[n].output * (1.0 - ann.arr_layers[l].arr_nodes[n].output);
			
			//Adjust that node's weights.
			for (w = 0; w < ann.arr_layers[l].arr_nodes[n].weight.len; w++) {
				delta = ann.learning_rate * ann.arr_layers[l].arr_nodes[n].error * ann.arr_layers[l - 1].arr_nodes[w].output;
				delta += ann.momentum_factor * ann.arr_layers[l].arr_nodes[n].prev_dw.arr[w];
				ann.arr_layers[l].arr_nodes[n].weight.arr[w] += delta;
				ann.arr_layers[l].arr_nodes[n].prev_dw.arr[w] = delta;
			}
			delta = ann.learning_rate * ann.arr_layers[l].arr_nodes[n].error;
			delta += ann.momentum_factor * ann.arr_layers[l].arr_nodes[n].prev_db;
			ann.arr_layers[l].arr_nodes[n].bias += delta;
			ann.arr_layers[l].arr_nodes[n].prev_db = delta;
		}
	}
	return 1;
}

//Aux operation functions: Vector generation.
dvec_list_o create_targets(int num_cases) {
	
	//Simplest setup: Exactly one output node on.
		//In this case, num cases == num output nodes.
		
	int ind_case = 0;
	int ind_node = 0;
	dvec_list_o target_vecs = new_dvec_list(num_cases, num_cases);
	
	for (ind_case = 0; ind_case < num_cases; ind_case++) {
		for (ind_node = 0; ind_node < num_cases; ind_node++) {
			if (ind_case == ind_node)
				target_vecs.arr[ind_case].arr[ind_node] = 1;
			else
				target_vecs.arr[ind_case].arr[ind_node] = 0;
		}
	}
	return target_vecs;
}
dvec_list_o create_training_examples(int num_examples, ivec_o nodes_per_layer, int rn_max, double noisebound) {
	
	dvec_list_o input_examples = new_dvec_list(num_examples, nodes_per_layer.arr[0]);
	
	//Constants.
	int num_cases = nodes_per_layer.arr[nodes_per_layer.len - 1];
	//rn_max - is argument
	
	//Dummys.
	int cur_case = 0;
	int j = 0;
	int ind_vec = 0; 
	int r = 0;
	int r_n = 0;
	
	for (ind_vec = 0; ind_vec < input_examples.len; ind_vec++) {
		
		cur_case = ind_vec % num_cases;
		
		//Clear.
		clear_dvec(input_examples.arr[ind_vec]);
		
		//Rect, up line or down line.
		if (cur_case == 0) {
			for (j = 1; j < input_examples.arr[ind_vec].len - 1; j++)
				input_examples.arr[ind_vec].arr[j] = 1.0;
		}
		if (cur_case == 1) {
			for (j = 0; j < input_examples.arr[ind_vec].len; j++)
				input_examples.arr[ind_vec].arr[j] = (double)j / (double)(input_examples.arr[ind_vec].len);
		}
		if (cur_case == 2) {
			for (j = 0; j < input_examples.arr[ind_vec].len; j++)
				input_examples.arr[ind_vec].arr[j] = -(double)j / (double)(input_examples.arr[ind_vec].len);
		}
		
		//Randomize.
		for (r_n = 0; r_n < rn_max; r_n++) {
			r = int_rand_given_bound(0, input_examples.arr[ind_vec].len - 1);
			input_examples.arr[ind_vec].arr[r] += rand_given_bound(-noisebound, noisebound);
		}
		
	} cur_case = 0;
		
	return input_examples;
}
dvec_list_o create_test_items(int num_test_items, ivec_o nodes_per_layer, ivec_o* answer_key, int rn_max, double noisebound) {
	
	dvec_list_o test_list = new_dvec_list(num_test_items, nodes_per_layer.arr[0]);
	(*answer_key) = new_ivec(num_test_items);
	
	//Constants.
	int num_cases = nodes_per_layer.arr[nodes_per_layer.len - 1];
	//rn_max - is argument
	
	//Dummys.
	int j = 0;
	int ind_vec = 0;
	int r = 0;
	int r_n = 0;
	
	for (ind_vec = 0; ind_vec < test_list.len; ind_vec++) {
	
		//Clear.
		clear_dvec(test_list.arr[ind_vec]);
		
		//Rect, up line, or down line.
		r = int_rand_given_bound(0,2);
		if (r == 0) {
			for (j = 1; j < test_list.arr[ind_vec].len - 1; j++)
				test_list.arr[ind_vec].arr[j] = 1.0;
		}
		if (r == 1) {
			for (j = 0; j < test_list.arr[ind_vec].len; j++)
				test_list.arr[ind_vec].arr[j] = (double)j / (double)(test_list.arr[ind_vec].len);
		}
		if (r == 2) {
			for (j = 0; j < test_list.arr[ind_vec].len; j++)
				test_list.arr[ind_vec].arr[j] = -(double)j / (double)(test_list.arr[ind_vec].len);
		}
		(*answer_key).arr[ind_vec] = r;
		
		//Randomize.
		for (r_n = 0; r_n < rn_max; r_n++) {
			r = int_rand_given_bound(0, test_list.arr[ind_vec].len - 1);
			test_list.arr[ind_vec].arr[r] += rand_given_bound(-noisebound, noisebound);
		}
	}
	
	return test_list;
}

//Aux operation functions: Memory.
void preload_input_cpx(ann_o ann, cpx_o src) {
	ann.arr_layers[0].arr_nodes[0].output = src.real;
	ann.arr_layers[0].arr_nodes[1].output = src.imag;
}
void take_output_cpx(ann_o ann, cpx_o* dst) {
	(*dst).real = ann.arr_layers[ann.num_layers - 1].arr_nodes[0].output;
	(*dst).imag = ann.arr_layers[ann.num_layers - 1].arr_nodes[1].output;
}
int preload_layer_outputs(ann_o ann, dvec_o src, int l_dst) {
	if (src.len != ann.arr_layers[l_dst].num_nodes) {
		printf("Load input fail\r\n");
		return 0;
	}
	
	int n = 0;
	for (n = 0; n < ann.arr_layers[l_dst].num_nodes; n++)
		ann.arr_layers[l_dst].arr_nodes[n].output = src.arr[n];
	return 1;
}
int load_weight_to_node(ann_o ann, int l_dst, int n_dst, dvec_o weight_src, double bias_src) {
	if (weight_src.len != ann.arr_layers[l_dst].arr_nodes[n_dst].weight.len) {
		printf("Load weight fail\r\n");
		return 0;
	}
	int w = 0;
	for (w = 0; w < weight_src.len; w++)
		ann.arr_layers[l_dst].arr_nodes[n_dst].weight.arr[w] = weight_src.arr[w];
	ann.arr_layers[l_dst].arr_nodes[n_dst].bias = bias_src;
	
	return 1;
}
void save_ann(ann_o ann, char* filename) {
	
	//Only weights and biases. Does not hold learning rate or momentum factor.
	
	FILE* fout = fopen(filename, "w");
	int l; int n; int w;
	
	fprintf(fout, "%d, ", ann.num_layers); //Print #layers.
	for (l = 0; l < ann.num_layers; l++) //Print npl.
		fprintf(fout, "%d, ", ann.arr_layers[l].num_nodes);
	
	for (l = 1; l < ann.num_layers; l++) { //For each layer.
		for (n = 0; n < ann.arr_layers[l].num_nodes; n++) { //For each node.
			for (w = 0; w < ann.arr_layers[l].arr_nodes[n].weight.len; w++) //For each weight.
				fprintf(fout, "%lf, ", ann.arr_layers[l].arr_nodes[n].weight.arr[w]);
			fprintf(fout, "%lf, ", ann.arr_layers[l].arr_nodes[n].bias);
		}
	}
	fclose(fout);
}
ann_o load_ann(char* filename) {
	
	//Only weights and biases. Does not hold learning rate or momentum factor.
	
	FILE* fin = fopen(filename, "r");
	int l; int n; int w;
	ann_o ann;
	
	ivec_o npl;
	
	//Load in npl.
	fscanf(fin, "%d, ", &(npl.len));
		npl.arr = (int*)malloc(sizeof(int) * npl.len);
	for (l = 0; l < npl.len; l++)
		fscanf(fin, "%d, ", &(npl.arr[l]));
	
	//Initialize ANN.
	ann = new_ann_o(npl);
	for (l = 1; l < ann.num_layers; l++) {
		for (n = 0; n < ann.arr_layers[l].num_nodes; n++) {
			for (w = 0; w < ann.arr_layers[l].arr_nodes[n].weight.len; w++)
				fscanf(fin, "%lf, ", &(ann.arr_layers[l].arr_nodes[n].weight.arr[w]));
			fscanf(fin, "%lf, ", &(ann.arr_layers[l].arr_nodes[n].bias));
		}
	}
	fclose(fin);
	
	return ann;
}

//Evaluate performance.
int max_output_node_ind(ann_o ann) {
	//Finds the node index, (in the output layer) that holds the max value.
	
	int n = 0;
	int l_output = ann.num_layers - 1;
	double max_output = -1.0;
	int n_max = -1;
	for (n = 0; n < ann.arr_layers[l_output].num_nodes; n++) {
		if (ann.arr_layers[l_output].arr_nodes[n].output > max_output) {
			max_output = ann.arr_layers[l_output].arr_nodes[n].output;
			n_max = n;
		}
	}
	return n_max;
}
int test_and_score(ann_o ann, dvec_list_o test_list, ivec_o answer_key, ivec_o* answer_ann) {
	
	*answer_ann = new_ivec(test_list.len);
	int ind_vec = 0;
	
	for (ind_vec = 0; ind_vec < test_list.len; ind_vec++) {
		preload_layer_outputs(ann, test_list.arr[ind_vec], 0);
		feed_forward_ann(ann);
		(*answer_ann).arr[ind_vec] = max_output_node_ind(ann);
	}
	
	return numequiv_i(answer_key, *answer_ann);
}

//Misc.
void print_ann(ann_o ann) { //Print every weight and bias, of every node, of every layer, of the net.
	int l = 0;
	int n = 0;
	int w = 0;
	
	for (n = 0; n < ann.arr_layers[l].num_nodes; n++) {
		printf("NODE %d.\r\n", n);
		printf("Output %lf\r\n", ann.arr_layers[l].arr_nodes[n].output);
	} printf("\r\n\r\n");
	
	for (l = 1; l < ann.num_layers; l++) {
		printf("LAYER %d.\r\n", l);
		for (n = 0; n < ann.arr_layers[l].num_nodes; n++) {
			printf("NODE %d.\r\n", n);
			for (w = 0; w < ann.arr_layers[l].arr_nodes[n].weight.len; w++)
				printf("%lf\r\n", ann.arr_layers[l].arr_nodes[n].weight.arr[w]);
			printf("Bias: %lf\r\n\r\n", ann.arr_layers[l].arr_nodes[n].bias);
		}
		printf("\r\n\r\n");
	}
}
#endif
