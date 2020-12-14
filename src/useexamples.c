//MIDvec Library.
	//Machine learning.
	//Image development.
	//Digital signal processing.
	//Vectors.
		//(and more)

//Commercial license for MIDvec: Terms stated at Binpress site.
//Contact is available through the Binpress site.

//To use any of these examples, just copy the chunk
//of code between the /* and */ and paste it into
//main(). Between the below examples (which easily
//cover MIDvec's functionality) and the MIDvec_doc.pdf,
//using MIDvec C library will become an intuitive
//way to solve problems quickly.
	
	//Correlation testing.
	/*
	//Initialize.
	dvec_o xn = new_dvec(10);
	dvec_o hn = new_dvec(7);
	
	dvec_o rn;
	
	init_vec(xn, 0);
	init_vec(hn, 1);
	
	bmp_o bmp_xn = new_bmp(800,800);
	bmp_o bmp_hn = new_bmp(800,800);
	bmp_o bmp_rn = new_bmp(800,800);
		set_white(bmp_xn);
		set_white(bmp_hn);
		set_white(bmp_rn);
	
	graph_o g;
		g.y_min = -10.0;
		g.y_max = 10.0;
		g.x_min = -1.0;
		g.x_max = 15.0;
	color_o red = new_color_rgb(255, 0, 0);
	color_o black = new_color_rgb(0, 0, 0);
	
	draw_axes(g, bmp_xn, black);
	draw_axes(g, bmp_hn, black);
	draw_axes(g, bmp_rn, black);
	
	//Execute.
	rn = corr(xn, hn);
	
	//Display.
	draw_stemplot_g(g, bmp_xn, 0, xn, red);
	draw_stemplot_g(g, bmp_hn, 0, hn, red);
	draw_stemplot_g(g, bmp_rn, 0, rn, red);
	save_bmp(bmp_xn, "bmp_xn.bmp");
	save_bmp(bmp_hn, "bmp_hn.bmp");
	save_bmp(bmp_rn, "bmp_rn.bmp");
	
	printf("\r\nInput signal, xn:\r\n");
	print_dvec(xn);
	printf("\r\nImpulse response, hn:\r\n");
	print_dvec(hn);
	printf("\r\nOutput vector, rn:\r\n");
	print_dvec(rn);
	*/
	
	//Convolution testing.
	/*
	//Initialize.
	dvec_o xn = new_dvec(5);
	dvec_o hn = new_dvec(3);
	
	dvec_o yn1;
	dvec_o yn2;
	
	int n = 0;
	for (n = 0; n < xn.len; n++)
		xn.arr[n] = 1;
	for (n = 0; n < hn.len; n++)
		hn.arr[n] = 1.0 / (double)(hn.len);
	
	graph_o g;
		g.y_min = -5.0;
		g.y_max = 5.0;
		g.x_min = -1.0;
		g.x_max = 15.0;
	color_o red = new_color_rgb(255, 0, 0);
	color_o black = new_color_rgb(0, 0, 0);
	
	bmp_o bmp_xn = new_bmp(800,800);
	bmp_o bmp_hn = new_bmp(800,800);
	bmp_o bmp_yn1 = new_bmp(800,800);
	bmp_o bmp_yn2 = new_bmp(800,800);
		set_white(bmp_xn);
		set_white(bmp_hn);
		set_white(bmp_yn1);
		set_white(bmp_yn2);
		
	draw_axes(g, bmp_xn, black);
	draw_axes(g, bmp_hn, black);
	draw_axes(g, bmp_yn1, black);
	draw_axes(g, bmp_yn2, black);
	
	//Test.
	yn1 = conv(xn, hn);
	yn2 = conv_full_length(xn, hn);
	
	//Show.
	draw_stemplot_g(g, bmp_xn, 0, xn, red);
	draw_stemplot_g(g, bmp_hn, 0, hn, red);
	draw_stemplot_g(g, bmp_yn1, 0, yn1, red);
	draw_stemplot_g(g, bmp_yn2, 0, yn2, red);
	save_bmp(bmp_xn, "bmp_xn.bmp");
	save_bmp(bmp_hn, "bmp_hn.bmp");
	save_bmp(bmp_yn1, "bmp_yn1.bmp");
	save_bmp(bmp_yn2, "bmp_yn2.bmp");
	
	printf("\r\nInput signal, xn:\r\n");
	print_dvec(xn);
	printf("\r\nImpulse response, hn:\r\n");
	print_dvec(hn);
	printf("\r\nOutput vector 1, yn1:\r\n");
	print_dvec(yn1);
	printf("\r\nOutput vector 2, yn2:\r\n");
	print_dvec(yn2);
	*/
	
	//FFT testing.
	/*
	//Initialize.
	cpx_vec_o xn;
	xn.len = 16;
	int n = 0;
	for (n = 0; n < xn.len; n++) {
		xn.arr[n].real = 4.7 + 3.2*cos(0.5*M_PI*(double)(n)) + 10.0*cos(0.375*M_PI*(double)(n));
		xn.arr[n].imag = 0;
	}
	dvec_o xn_real = downcast_cpx(xn, "real");
	
	printf("Start\r\n");
	print_cpx_vec(xn);
	cpx_vec_o Xk;
	
	//Execute.
	Xk = fft(xn);
	xn = ifft(Xk);
	
	//Output.
	signal_to_stemplot_bmp(xn_real, "bmp_xn.bmp");
	fft_mags_to_bmp(Xk, "fft.bmp");
	
	printf("\r\nFFT output:\r\n");
	print_cpx_vec(Xk);
	
	printf("\r\nIFFT output:\r\n");
	print_cpx_vec(xn);
	*/
	
	//FFT testing 2.
	/*
	dvec_o xn_real = datatovec_d("speechsample.txt", 32768);
	cpx_vec_o xn = upcast_dvec(xn_real);
	
	cpx_vec_o Xk = fft(xn);
	fft_mags_to_bmp(Xk, "fft.bmp");
	*/
	
	//Insert intra vec testing.
	/*
	int n = 0;
	dvec_o xn = new_dvec(10);
	for (n = 0; n < xn.len; n++)
		xn.arr[n] = (double)(n);
	
	printf("Original xn:\r\n");
	print_dvec(xn);
	
	insert_intra_vec(xn, 2, 8);
	
	printf("\r\nxn after insert_intra_vec() call:\r\n");
	print_dvec(xn);
	*/
	
	//Swap intra vec testing.
	/*
	int n = 0;
	dvec_o xn;
	xn.len = 10;
	for (n = 0; n < xn.len; n++)
		xn.arr[n] = (double)(n);
	
	printf("Original xn:\r\n");
	print_dvec(xn);
	
	swap_intra_vec(xn, 2, 8);
	
	printf("\r\nxn after insert_intra_vec() call:\r\n");
	print_dvec(xn);
	*/

	//Insertion sort testing.
	/*
	int n = 0;
	
	dvec_o xn;
	xn.len = 10;
	init_vec(xn, 2);
	
	print_dvec(xn);
	printf("\r\n");
	
	insertion_sort(xn);
	
	printf("\r\n");
	print_dvec(xn);
	*/
	
	//Selection sort testing.
	/*
	int n = 0;
	
	dvec_o xn;
	xn.len = 10;
	init_vec(xn, 2);
	
	print_dvec(xn);
	printf("\r\n");
	
	selection_sort(xn);
	
	printf("\r\n");
	print_dvec(xn);
	*/
	
	//Generate lowpass filter helper functions testing.
		//Pick window testing.
		/*
		printf("%d\r\n\r\n", pick_window(0.1));
		printf("%d\r\n\r\n", pick_window(0.01));
		printf("%d\r\n\r\n", pick_window(0.005));
		printf("%d\r\n\r\n", pick_window(0.001));
		printf("%d\r\n\r\n", pick_window(0.0001));
		*/
		
		//Generate window testing.
		/*
		int n = 0;
		dvec_o xn;
		for (n = 0; n < 5; n++) {
			xn = generate_window(n, 100);
			printf("%d:\r\n", n);
			print_dvec(xn);
			printf("\r\n\r\n");
		}
		*/
		
		//Get filter length testing.
		/*
		int M = 0;
		M = get_filter_length(0.2*M_PI, 0.3*M_PI, 0.1);
		printf("Filter length is %d\r\n\r\n", M);
		
		M = get_filter_length(0.2*M_PI, 0.3*M_PI, 0.01);
		printf("Filter length is %d\r\n\r\n", M);
		
		M = get_filter_length(0.2*M_PI, 0.3*M_PI, 0.005);
		printf("Filter length is %d\r\n\r\n", M);
		
		M = get_filter_length(0.2*M_PI, 0.3*M_PI, 0.001);
		printf("Filter length is %d\r\n\r\n", M);
		
		M = get_filter_length(0.2*M_PI, 0.3*M_PI, 0.0001);
		printf("Filter length is %d\r\n\r\n", M);
		*/
		
		//Generate lowpass impulse response testing.
		/*
		dvec_o hn = generate_lowpass_impulse_response(100, 0.5*M_PI);
		print_dvec(hn);
		*/
		
	//Generate lowpass filter testing.
	/*
	//Declare.
	dvec_o xn = new_dvec(512);
	cpx_vec_o Xk = new_cpx_vec(xn.len);
	
	dvec_o hn;
	dvec_o yn;
	
	cpx_vec_o xn_cpx;
	cpx_vec_o yn_cpx;
	
	cpx_vec_o Yk = new_cpx_vec(xn.len);
	
	//Initialize.
	init_vec(xn, 3);
		xn_cpx = upcast_dvec(xn);
	
	Xk = fft(xn_cpx);
	
	//Generate and operate lowpass filter.
	hn = generate_lowpass_filter(0.3*M_PI, 0.2*M_PI, 0.1); 
	yn = conv(xn, hn);
		yn_cpx = upcast_dvec(yn);
	Yk = fft(yn_cpx);
	
	//Analyze results.
	printf("Used hn with length %d\r\n", hn.len);
	
	printf("Spectrum before lowpass filter:\r\n");
	print_cpx_vec(Xk);
	
	printf("\r\nSpectrum after lowpass filter:\r\n");
	print_cpx_vec(Yk);
	*/
	
	//Generate highpass filter testing.
	/*
	//Declare.
	dvec_o xn;
	xn.len = 512;
	xn.arr = (double*)malloc(sizeof(double)*xn.len);
	
	cpx_vec_o Xk;
	Xk.len = xn.len;
	Xk.arr = (cpx_o*)malloc(sizeof(cpx_o)*Xk.len);
	
	dvec_o hn;
	dvec_o yn;
	
	cpx_vec_o xn_cpx;
	cpx_vec_o yn_cpx;
	
	cpx_vec_o Yk;
	Yk.len = xn.len;
	Yk.arr = (cpx_o*)malloc(sizeof(cpx_o)*Yk.len);
	
	//Initialize.
	init_vec(xn, 3);
		xn_cpx = upcast_dvec(xn);
	
	Xk = fft(xn_cpx);
	
	//Generate and operate filter.
	hn = generate_highpass_filter(0.3*M_PI, 0.2*M_PI, 0.1);
	yn = conv(xn, hn);
		yn_cpx = upcast_dvec(yn);
	Yk = fft(yn_cpx);
	
	//Analyze results.
	printf("Used hn with length %d\r\n", hn.len);
	
	printf("Spectrum before filter:\r\n");
	print_cpx_vec(Xk);
	
	printf("\r\nSpectrum after filter:\r\n");
	print_cpx_vec(Yk);
	*/
	
	//Generate bandstop filter testing.
	/*
	//Declare.
	dvec_o xn;
	xn.len = 512;
	xn.arr = (double*)malloc(sizeof(double)*xn.len);
	
	cpx_vec_o Xk;
	Xk.len = xn.len;
	Xk.arr = (cpx_o*)malloc(sizeof(cpx_o)*Xk.len);
	
	dvec_o hn;
	dvec_o yn;
	
	cpx_vec_o xn_cpx;
	cpx_vec_o yn_cpx;
	
	cpx_vec_o Yk;
	Yk.len = xn.len;
	Yk.arr = (cpx_o*)malloc(sizeof(cpx_o)*Yk.len);
	
	//Initialize.
	init_vec(xn, 3);
		xn_cpx = upcast_dvec(xn);
	
	Xk = fft(xn_cpx);
	
	//Generate and operate filter.
	hn = generate_bandstop_filter(0.2*M_PI, 0.3*M_PI, 0.7*M_PI, 0.8*M_PI, 0.1); 
	yn = conv(xn, hn);
		yn_cpx = upcast_dvec(yn);
	Yk = fft(yn_cpx);
	
	//Analyze results.
	printf("Used hn with length %d\r\n", hn.len);
	
	printf("Spectrum before filter:\r\n");
	print_cpx_vec(Xk);
	
	printf("\r\nSpectrum after filter:\r\n");
	print_cpx_vec(Yk);
	*/
	
	//Generate bandpass filter testing.
	/*
	//Declare.
	dvec_o xn;
	xn.len = 512;
	xn.arr = (double*)malloc(sizeof(double)*xn.len);
	
	cpx_vec_o Xk;
	Xk.len = xn.len;
	Xk.arr = (cpx_o*)malloc(sizeof(cpx_o)*Xk.len);
	
	dvec_o hn;
	dvec_o yn;
	
	cpx_vec_o xn_cpx;
	cpx_vec_o yn_cpx;
	
	cpx_vec_o Yk;
	Yk.len = xn.len;
	Yk.arr = (cpx_o*)malloc(sizeof(cpx_o)*Yk.len);
	
	//Initialize.
	init_vec(xn, 3);
		xn_cpx = upcast_dvec(xn);
	
	Xk = fft(xn_cpx);
	
	//Generate and operate filter.
	hn = generate_bandpass_filter(0.2*M_PI, 0.3*M_PI, 0.7*M_PI, 0.8*M_PI, 0.1); 
	yn = conv(xn, hn);
		yn_cpx = upcast_dvec(yn);
	Yk = fft(yn_cpx);
	
	//Analyze results.
	printf("Used hn with length %d\r\n", hn.len);
	
	printf("Spectrum before filter:\r\n");
	print_cpx_vec(Xk);
	
	printf("\r\nSpectrum after filter:\r\n");
	print_cpx_vec(Yk);
	*/
	
	//ANN functionality testing.
		/*
		//Create ANN. new_ann_o() test.
		ivec_o nodes_per_layer = new_ivec(5);
		nodes_per_layer.arr[0] = 20;
		nodes_per_layer.arr[1] = 4;
		nodes_per_layer.arr[2] = 6;
		nodes_per_layer.arr[3] = 12;
		nodes_per_layer.arr[4] = 15;
		
		ann_o ann = new_ann_o(nodes_per_layer);
		
		printf("ANN has %d layers.\r\n", ann.num_layers);
		
		printf("Their amounts of nodes are:\r\n");
		int l = 0;
		int n = 0;
		for (l = 0; l < ann.num_layers; l++) {
			printf("Layer %d has %d nodes.\r\n", l, ann.arr_layers[l].num_nodes);
			printf("This layer's nodes have the following weight vector lengths: \r\n");
			for (n = 0; n < ann.arr_layers[l].num_nodes; n++)
				printf("Node %d: %d\r\n", n, ann.arr_layers[l].arr_nodes[n].weight.len);
			printf("\r\n\r\n\r\n");
		}
		*/
		
		//Demo that weights and biases are randomly initialized.
		/*
		ivec_o nodes_per_layer = new_ivec(5);
		nodes_per_layer.arr[0] = 20;
		nodes_per_layer.arr[1] = 4;
		nodes_per_layer.arr[2] = 6;
		nodes_per_layer.arr[3] = 12;
		nodes_per_layer.arr[4] = 15;
		
		ann_o ann = new_ann_o(nodes_per_layer);
		
		int l = 0;
		int n = 0;
		int w = 0;
		for (l = 1; l < ann.num_layers; l++) {
			for (n = 0; n < ann.arr_layers[l].num_nodes; n++) {
				printf("Layer %d, node %d.\r\n", l, n);
				printf("%d weights:\r\n", ann.arr_layers[l].arr_nodes[n].weight.len);
				print_dvec(ann.arr_layers[l].arr_nodes[n].weight);
				printf("Bias: %lf\r\n\r\n\r\n", ann.arr_layers[l].arr_nodes[n].bias);
			}
			printf("\r\n\r\n");
		}
		*/
		
		//Feed forward to ANN output. feed_forward_ann() test.
		/*
		//Declare memory.
		ivec_o nodes_per_layer = new_ivec(4);
		nodes_per_layer.arr[0] = 3;
		nodes_per_layer.arr[1] = 4;
		nodes_per_layer.arr[2] = 1;
		nodes_per_layer.arr[3] = 2;
		
		ann_o ann = new_ann_o(nodes_per_layer);
		
		//Initialize input layer.
		dvec_o input_layer;
			input_layer = new_dvec(3);
			input_layer.arr[0] = 1; input_layer.arr[1] = 2; input_layer.arr[2] = 3;
		preload_layer_outputs(ann, input_layer, 0);
		
		
		//Initialize weights and biases to presets.
		int l = 0;
		int n = 0;
		int i = 0;
		int q = 0;
		dvec_o weight_src;
		for (l = 1; l < ann.num_layers; l++) {
			for (n = 0; n < ann.arr_layers[l].num_nodes; n++) {
				weight_src = new_dvec(ann.arr_layers[l - 1].num_nodes);
				
				for (i = 0; i < weight_src.len; i++)
					weight_src.arr[i] = (double)(q);
				
				load_weight_to_node(ann, l, n, weight_src, weight_src.arr[0]);
				q++;
			}
		}
		
		//print_ann(ann);
		feed_forward_ann(ann);
		*/
		
		//ANN training test.
		/*
			//Differentiate between three different signals.
			//Case 0: Rect. [0, 1, 1, 1, ... 1, 1, 0].
			//Case 1: Upward line. [0, 0.1, 0.2... 1.0].
			//Case 2: Downward line. [0, -0.1, -0.2 ... -1.0].
			
		//Initialize net.
		ivec_o npl = new_ivec(3);
			npl.arr[0] = 10;
			npl.arr[1] = 50;
			npl.arr[2] = 3;
		ann_o ann = new_ann_o(npl);
			ann.learning_rate = 0.1;
			ann.momentum_factor = 0.01;
		
		//Generate training examples.
		int num_cases = npl.arr[npl.len - 1]; int cur_case = 0;
		int ind_vec = 0; int j = 0;
		int r = 0; int r_n = 0; int rn_max = 100;
		dvec_list_o input_examples = create_training_examples(1000, npl, 100, 0.2);
		
		//Train.
		dvec_list_o target_vec_list = create_targets(num_cases);
		for (ind_vec = 0; ind_vec < input_examples.len; ind_vec++) {
			preload_layer_outputs(ann, input_examples.arr[ind_vec], 0);
			feed_forward_ann(ann);
			backpropagate_adjust(ann, target_vec_list.arr[cur_case]);
			cur_case = revolve_mod(cur_case, num_cases);
		}
		
		//Generate test items.
		ivec_o answer_key;
		dvec_list_o test_list = create_test_items(1000, npl, &answer_key, 100, 0.2);
		
		//Test and score.
		ivec_o answer_ann = new_ivec(test_list.len);
		int num_correct = test_and_score(ann, test_list, answer_key, &answer_ann);
		
		//Extra output.
		for (ind_vec = 0; ind_vec < test_list.len; ind_vec++)
			printf("%d, %d\r\n", answer_ann.arr[ind_vec], answer_key.arr[ind_vec]);
		printf("%d correct guesses.\r\n", num_correct);
		*/
	//ANN project 1: Train a particle to move in a circle.
	/*
		//Vector field verification.
		//Initialize.
		graph_o g;
			g.x_min = -1.0;
			g.x_max = 1.0;
			g.y_min = -1.0;
			g.y_max = 1.0;
			
		bmp_o b = new_bmp(1000, 1000);
		
		cpx_o z;
			z.real = -0.1;
			z.imag = -0.1;
			
		color_o ccN;
			ccN.r = 0;
			ccN.g = 0;
			ccN.b = 255;
		
		color_o ccT;
			ccT.r = 255;
			ccT.g = 0;
			ccT.b = 0;
			
		color_o ccD;
			ccD.r = 0;
			ccD.g = 0;
			ccD.b = 0;
		
		//Draw.
		set_white(b);
		
		double r_i = 0.0;
		double theta_i = 0;
			double xp;
			double yp;
		
		cpx_o vN_test;
		cpx_o vT_test;
		cpx_o vD_test;
		
		circle_o desired_circle;
			desired_circle.xc = 0;
			desired_circle.yc = 0;
			desired_circle.r = 0.5;
		
		color_o ccC;
			ccC.r = 100;
			ccC.g = 100;
			ccC.b = 100;
			
		draw_circle_g(g, b, desired_circle, ccC);
		
		while (r_i < 1.0) {
			while (theta_i < 2.0*M_PI) {
				xp = r_i * cos(theta_i);
				yp = r_i * sin(theta_i);
				
				vN_test = calc_target_vN(xp, yp, desired_circle.r);
					vN_test = cpx_scale(vN_test, 0.3);
				vT_test = calc_target_vT(xp, yp);
					vT_test = cpx_scale(vT_test, 0.3);
				vD_test = cpx_add(vN_test, vT_test);
				
				//draw_target_v(g, b, vN_test, xp, yp, ccN);
				//draw_target_v(g, b, vT_test, xp, yp, ccT);
				draw_target_v(g, b, vD_test, xp, yp, ccD);
				
				theta_i += (M_PI / 6.0);
			} theta_i = 0;
			r_i += 0.2;
		}
		
		//Save.
		save_bmp(b, "piac.bmp");
	*/
		//Execute.
		/*
			//Initialize.
			//1. Experimental hyperparameters.
			int num_moves = 0;
			int total_num_moves = 100; //How many moves from each of those original positions.
			int num_tests = 0;
			int total_num_tests = 10000; //How many different original positions.
										  //test2: 10000
			double time_constant = 0.01;
			
			//2. Vectors.
			cpx_o rp;
			cpx_o rp_old;
				
			cpx_o vp;
				cpx_o vp_n;
				cpx_o vp_t;
			dvec_o vD = new_dvec(4);
				cpx_o vD_n;
				cpx_o vD_t;
			
			//3. ANN
			ann_o ann;
				ivec_o npl = new_ivec(3);
					npl.arr[0] = 2;
					npl.arr[1] = 100;
					npl.arr[2] = 4;
				ann = new_ann_o(npl);
				init_weights_and_biases_ann(ann);
				
				ann.learning_rate = 0.1;
				ann.momentum_factor = 0.1;
			
			//4. Bitmap shiz.
			bmp_o bmp = new_bmp(800, 800);
			set_white(bmp);
			
			bmp_o bmp_test = new_bmp(800, 800);
			set_white(bmp_test);
			
			graph_o g;
				g.x_min = -1.0;
				g.x_max = 1.0;
				g.y_min = -1.0;
				g.y_max = 1.0;
				
			color_o cc_axes = new_color_string("gray", 1.0);
			draw_axes(g, bmp, cc_axes);
			
			circle_o cir;
				cir.r = 0.005;
			circle_o cross;
				cross.r = 0.02;
				
			int num_colors = 5;
			color_o* cc_points = (color_o*)malloc(sizeof(color_o) * num_colors);
				cc_points[0] = new_color_string("red", 1.0);
				cc_points[1] = new_color_string("green", 0.8);
				cc_points[2] = new_color_string("yellow", 0.5);
				cc_points[3] = new_color_string("magenta", 1.0);
				cc_points[4] = new_color_string("blue", 1.0);
			
			color_o cc_b = new_color_string("black", 0);
			circle_o des_cir;
				des_cir.xc = 0;
				des_cir.yc = 0;
				des_cir.r = 0.5;
			draw_circle_g(g, bmp, des_cir, cc_b);
		*/	
			//Train.
			/*
			while (num_tests < total_num_tests) {
				
				if (num_tests % 100 == 0)
					printf("Train %d\r\n", num_tests);
				
				rp.real = rand_given_bound(-1.0, 1.0);
				rp.imag = rand_given_bound(-1.0, 1.0);
					cross.xc = rp.real;
					cross.yc = rp.imag;
					//draw_cross_g(g, bmp, cross, cc_points[num_tests % num_colors]);
				
				while (num_moves < total_num_moves) {
					
					//if (num_moves % 100 == 0)
					//	printf("Move %d\r\n", num_moves);
					
					//0. Handle particle out of bounds.
					if ((rp.real > 1.0) || (rp.real < -1.0) || (rp.imag > 1.0) || (rp.imag < -1.0)) {
						rp.real = rand_given_bound(-1.0, 1.0);
						rp.imag = rand_given_bound(-1.0, 1.0);
						//printf("RESET\r\n");
					}
					
					//1. Feed location into ANN.
					preload_input_cpx(ann, rp);
					feed_forward_ann(ann);
					
					//2. Compute new location.
					vp_n.real = (ann.arr_layers[ann.num_layers - 1].arr_nodes[0].output - 0.5) * 2.0;
					vp_n.imag = (ann.arr_layers[ann.num_layers - 1].arr_nodes[1].output - 0.5) * 2.0;
					vp_t.real = (ann.arr_layers[ann.num_layers - 1].arr_nodes[2].output - 0.5) * 2.0;
					vp_t.imag = (ann.arr_layers[ann.num_layers - 1].arr_nodes[3].output - 0.5) * 2.0;
					vp = cpx_add(vp_n, vp_t);
					
					rp_old = rp;
					rp = cpx_add(rp, cpx_scale(vp, time_constant));
					
					//3. Adjust ANN using calculated vD.
					vD_n = calc_target_vN(rp.real, rp.imag, des_cir.r);
					vD_t = calc_target_vT(rp.real, rp.imag);
						vD.arr[0] = (vD_n.real + 1.0) * 0.5;
						vD.arr[1] = (vD_n.imag + 1.0) * 0.5;
						vD.arr[2] = (vD_t.real + 1.0) * 0.5;
						vD.arr[3] = (vD_t.imag + 1.0) * 0.5;
					backpropagate_adjust(ann, vD);
					
					//4. Show me what happened.
					//printf("New position: %lf + j%lf\r\n", rp.real, rp.imag);
					//printf("vD: %lf + j%lf\r\n", cpx_add(vD_n, vD_t).real, cpx_add(vD_n, vD_t).imag);
						cir.xc = rp.real;
						cir.yc = rp.imag;
					//draw_circle_g(g, bmp, cir, cc_points[num_tests % num_colors]);
					//draw_target_v(g, bmp, vD, rp.real, rp.imag, cc_b);
					
					num_moves++;
				}
				num_moves = 0;
				num_tests++;
			} num_tests = 0;
			
			//save_bmp(bmp, "particle_train.bmp");
			save_ann(ann, "particle_ann.txt");
			printf("Done training\r\n");
			*/
			
			//Test.
			/*
			draw_circle_g(g, bmp_test, des_cir, cc_b);
			draw_axes(g, bmp_test, cc_axes);
				draw_ruleV_g(g, bmp_test, 0.5, cc_axes);
				draw_ruleV_g(g, bmp_test, -0.5, cc_axes);
				draw_ruleH_g(g, bmp_test, 0.5, cc_axes);
				draw_ruleH_g(g, bmp_test, -0.5, cc_axes);
			
			ann = load_ann("particle_ann.txt");
				ann.learning_rate = 0.1;
				ann.momentum_factor = 0.1;
			
			total_num_tests = 10;
			total_num_moves = 100;
			while (num_tests < total_num_tests) {
				
				if (num_tests % 10 == 0)
					printf("Test %d\r\n", num_tests);
				
				rp.real = rand_given_bound(-1.0, 1.0);
				rp.imag = rand_given_bound(-1.0, 1.0);
					cross.xc = rp.real;
					cross.yc = rp.imag;
					draw_cross_g(g, bmp_test, cross, cc_points[num_tests % num_colors]);
				
				while (num_moves < total_num_moves) {
					
					//0. Handle particle out of bounds.
					if ((rp.real > 1.0) || (rp.real < -1.0) || (rp.imag > 1.0) || (rp.imag < -1.0)) {
						rp.real = rand_given_bound(-1.0, 1.0);
						rp.imag = rand_given_bound(-1.0, 1.0);
						//printf("RESET\r\n");
					}
					
					//1. Feed location into ANN.
					preload_input_cpx(ann, rp);
					feed_forward_ann(ann);
					
					//2. Compute new location.
					vp_n.real = (ann.arr_layers[ann.num_layers - 1].arr_nodes[0].output - 0.5) * 2.0;
					vp_n.imag = (ann.arr_layers[ann.num_layers - 1].arr_nodes[1].output - 0.5) * 2.0;
					vp_t.real = (ann.arr_layers[ann.num_layers - 1].arr_nodes[2].output - 0.5) * 2.0;
					vp_t.imag = (ann.arr_layers[ann.num_layers - 1].arr_nodes[3].output - 0.5) * 2.0;
					vp = cpx_add(vp_n, vp_t);
					
					rp_old = rp;
					rp = cpx_add(rp, cpx_scale(vp, time_constant));
					
					//3. Show me what happened.
					//printf("New position: %lf + j%lf\r\n", rp.real, rp.imag);
					//printf("vD: %lf + j%lf\r\n", cpx_add(vD_n, vD_t).real, cpx_add(vD_n, vD_t).imag);
						cir.xc = rp.real;
						cir.yc = rp.imag;
					draw_circle_g(g, bmp_test, cir, cc_points[num_tests % num_colors]);
					
					num_moves++;
				}
				num_moves = 0;
				num_tests++;
			}
			
			save_bmp(bmp_test, "particle_test.bmp");
			printf("Done testing\r\n");
			*/
	//Bitmap infrastructure.
		//Experimentation on .bmp from Paint.
		/*
		bvec_o testb_in = new_bvec(374);
		FILE* fin = fopen("bmp_test_256_good.bmp", "r");
		fread(testb_in.arr, 1, 374, fin);
		print_bvec(testb_in);
		*/
		
		//Pixel draw testing.
		/*
		//Initialize.
		bmp_o bmp = new_bmp(500, 500);
		
		//Do something with the image.
		set_white(bmp);
		color_o cc;
			cc.r = 0;
			cc.g = 0;
			cc.b = 255;
		
		draw_line(bmp, 100, 100, 300, 300, cc); //x2 > x1, y2 > y1
		draw_line(bmp, 400, 400, 100, 100, cc); //x2 < x1, y2 < y1
		draw_line(bmp, 100, 400, 400, 100, cc); //x2 > x1, y2 < y1
		draw_line(bmp, 400, 100, 100, 400, cc); //x2 < x1, y2 > y1
		draw_line(bmp, 100, 350, 100, 400, cc);
		draw_circle(bmp, 350, 200, 100, cc);
		
		//Save it.
		save_bmp(bmp, "test.bmp");
		*/
		
		//Graph test 1.
		/*
		//Initialize.
		bmp_o bmp = new_bmp(500, 500);
		
		//Draw something.
		set_white(bmp);
		
		graph_o g;
			g.x_min = -1.0;
			g.x_max = 1.0;
			g.y_min = -1.0;
			g.y_max = 1.0;
		circle_o cir;
			cir.xc = 0.5;
			cir.yc = 0.5;
			cir.r = 0.1;
		circle_o cir2;
			cir2.xc = 0;
			cir2.yc = -0.3;
			cir2.r = 0.2;
		circle_o cir3;
			cir3.xc = -0.5;
			cir3.yc = 0.1;
			cir3.r = 0.05;
			
		line_segment_o lin;
			lin.x1 = 0.1;
			lin.y1 = 0.15;
			lin.x2 = -0.7;
			lin.y2 = -0.3;
		
		color_o cc;
			cc.r = 255;
			cc.g = 0;
			cc.b = 0;
		
		int xx = 0; int yy = 0;
		map_graph2pix_2D(g, bmp, -0.8, 0.1, &xx, &yy);
		printf("%d %d\r\n", xx, yy);
		
		double r = 1.0; double theta = M_PI/2.0;
		int r_int = (int)map_segment(g, bmp, r, theta);
		printf("%lf at %lf goes to %d\r\n", r, theta, r_int);
		
		draw_circle_g(g, bmp, cir, cc);
		//draw_circle_g(g, bmp, cir2, cc);
		//draw_circle_g(g, bmp, cir3, cc);
		
		draw_line_g(g, bmp, lin, cc);
		
		//Save it.
		save_bmp(bmp, "test2.bmp");
		*/
		
		//Graph test 2. draw_xyplot_g().
		/*
		//Initialize.
		bmp_o bmp = new_bmp(2400, 1600);
		set_white(bmp);
		
		graph_o g;
			g.x_min = -0.1;
			g.x_max = 10.0;
			g.y_min = -2.0;
			g.y_max = 2.0;
		
		color_o cc = new_color_rgb(0, 0, 0);
		color_o cc_a = new_color_rgb(0, 0, 255);
		
		int n = 0;
		dvec_o x_vals = new_dvec(100000);
		dvec_o y_vals = new_dvec(x_vals.len);
		
		//Execute.
		x_vals = generate_linear_vec(x_vals.len, g.x_min, g.x_max);
		for (n = 0; n < y_vals.len; n++)
			y_vals.arr[n] = 0.2*sin(2.0*x_vals.arr[n]);// * sin(500.0*x_vals.arr[n]);
		
		draw_axes(g, bmp, cc);
		draw_xyplot_g(g, bmp, x_vals, y_vals, cc_a);
		
		//Save.
		save_bmp(bmp, "graph.bmp");
		*/
		
		//Graph test 2.5. draw_stemplot_g().
		/*
		//Initialize.
		bmp_o bmp = new_bmp(500, 500);
		set_white(bmp);
		
		graph_o g;
			g.x_min = 0;
			g.x_max = 100;
			g.y_min = -1;
			g.y_max = 1;
		
		color_o cc = new_color_rgb(0, 0, 0);
		color_o cc_a = new_color_rgb(255, 0, 0);
		
		int n = 0;
		dvec_o y_vals = new_dvec(50);
		
		//Execute.
		for (n = 0; n < y_vals.len; n++)
			y_vals.arr[n] = ((double)n - ((double)y_vals.len) / 2.0) / (double)y_vals.len;
		
		draw_axes(g, bmp, cc);
		draw_stemplot_g(g, bmp, 25, y_vals, cc_a);
		
		//Save.
		save_bmp(bmp, "graph.bmp");
		*/
		
		//Graph test 3. draw_rthetaplot_g().
		/*
		//Initialize.
		bmp_o bmp = new_bmp(800, 800);
		set_white(bmp);
		
		graph_o g;
			g.x_min = -2*M_PI;
			g.x_max = 2*M_PI;
			g.y_min = -2*M_PI;
			g.y_max = 2*M_PI;
		
		color_o cc = new_color_rgb(0, 0, 0);
		color_o cc1 = new_color_rgb(0, 0, 255);
		color_o cc2 = new_color_rgb(255, 125, 0);
		
		int n = 0;
		dvec_o theta_vals = new_dvec(10000);
		dvec_o r_vals1 = new_dvec(theta_vals.len);
		dvec_o r_vals2 = new_dvec(theta_vals.len);
		
		//Execute.
		theta_vals = generate_linear_vec(theta_vals.len, 0, 2*M_PI);
		for (n = 0; n < theta_vals.len; n++) {
			r_vals1.arr[n] = theta_vals.arr[n];//1.0 * sin(5.0 * theta_vals.arr[n]);
			r_vals2.arr[n] = -theta_vals.arr[n];//-1.0 * sin(5.0 * theta_vals.arr[n]);
		}
		
		draw_axes(g, bmp, cc);
		draw_rthetaplot_g(g, bmp, theta_vals, r_vals1, cc1);
		draw_rthetaplot_g(g, bmp, theta_vals, r_vals2, cc2);
		
		//Save.
		save_bmp(bmp, "graph.bmp");
		*/
		
	//Card testing.
	/*
	printf("start\r\n");
	
	vec_card_o shoe = new_vec_card(4*52);
	shoe = new_deck(shoe);
	print_cardvec(shoe);
	
	printf("\r\n\r\n");
	
	shoe = shuffle(shoe);
	print_cardvec(shoe);
	
	ivec_o cardcounts = new_ivec(10);
		clear_ivec(cardcounts);
	int n = 0;
	
	//Count.
	for (n = 0; n < shoe.len; n++)
		cardcounts.arr[shoe.arr[n].faceval - 2]++;
	
	//Print.
	printf("\r\n\r\n");
	for (n = 0; n < cardcounts.len; n++)
		printf("There are %d %ds\r\n", cardcounts.arr[n], n + 2);
	*/
	
	//Blackjack simulator (in blackjack.h)
	/*
	blackjack_sim();
	*/