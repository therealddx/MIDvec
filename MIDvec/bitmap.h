//MIDvec Library.
	//Machine learning.
	//Image development.
	//Digital signal processing.
	//Vectors.
		//(and more)

#ifndef BITMAP_H
#define BITMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cpx_vec.h"
#include "dsp.h"
#include "misc_math.h"
#include "ann.h"
#include "sort.h"

//Objects.
typedef struct bmp {
	bvec_o header;
	bvec_o img;
	bvec_o full;
	
	int x_pix;
	int y_pix;
	int bytespp;
	
	int total_num_bytes;
	int num_padded_bytes_per_row;
} bmp_o;
typedef struct graph {
	double x_min;
	double x_max;
	double y_min;
	double y_max;
} graph_o;
typedef struct color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} color_o;
typedef struct line_segment {
	double x1;
	double y1;
	double x2;
	double y2;
} line_segment_o;
typedef struct circle {
	double xc;
	double yc;
	double r;
} circle_o;

//Constructors for above.
void randomize_color(color_o* cc) {
	(*cc).r = (unsigned char)int_rand_given_bound(0, 255);
	(*cc).g = (unsigned char)int_rand_given_bound(0, 255);
	(*cc).b = (unsigned char)int_rand_given_bound(0, 255);
}
color_o new_color_string(char* s, double scalor) {
	color_o cc;
	
	if (!strcmp(s, "red")) {
		cc.r = (unsigned char)(255.0 * scalor);
		cc.g = 0;
		cc.b = 0;
	}
	else if (!strcmp(s, "green")) {
		cc.r = 0;
		cc.g = (unsigned char)(255.0 * scalor);
		cc.b = 0;
	}
	else if (!strcmp(s, "blue")) {
		cc.r = 0;
		cc.g = 0;
		cc.b = (unsigned char)(255.0 * scalor);
	}
	else if (!strcmp(s, "magenta")) {
		cc.r = (unsigned char)(255.0 * scalor);
		cc.g = 0;
		cc.b = (unsigned char)(255.0 * scalor);
	}
	else if (!strcmp(s, "yellow")) {
		cc.r = (unsigned char)(255.0 * scalor);
		cc.g = (unsigned char)(255.0 * scalor);
		cc.b = 0;
	}
	else if (!strcmp(s, "cyan")) {
		cc.r = 0;
		cc.g = (unsigned char)(255.0 * scalor);
		cc.b = (unsigned char)(255.0 * scalor);
	}
	else if (!strcmp(s, "gray")) {
		cc.r = 125;
		cc.g = 125;
		cc.b = 125;
	}
	else {
		cc.r = 0;
		cc.g = 0;
		cc.b = 0;
	}
	
	return cc;
}
color_o new_color_rgb(unsigned char r_d, unsigned char g_d, unsigned char b_d) {
	color_o cc;
	cc.r = r_d;
	cc.g = g_d;
	cc.b = b_d;
	return cc;
}

line_segment_o new_line_segment(double x1d, double y1d, double x2d, double y2d) {
	line_segment_o lin;
	lin.x1 = x1d;
	lin.y1 = y1d;
	lin.x2 = x2d;
	lin.y2 = y2d;
	return lin;
}

//Indexing helper functions.
void parse_int(bvec_o bb, int x, int start_ind, int num_bytes) {
	int i = 0;
	for (i = start_ind; i < start_ind + num_bytes; i++)
		bb.arr[i] = (unsigned char)(   (x >> (8*(i - start_ind))) & 0xFF  );
}

//Key .bmp generation functions.
bmp_o new_bmp(int num_cols, int num_rows) {
	bmp_o bmp;
	
	bmp.x_pix = num_cols;
	bmp.y_pix = num_rows;
	bmp.bytespp = 3;
	bmp.img = new_bvec(bmp.x_pix * bmp.y_pix * bmp.bytespp);
	
	return bmp;
}
void generate_header(bmp_o* bmp) {
	(*bmp).header = new_bvec(54);
	
	//Given x_pix, find how many bytes you need to pad each row.
	(*bmp).num_padded_bytes_per_row = round_up_int((*bmp).x_pix * 3, 4) - (*bmp).x_pix * 3;
	(*bmp).total_num_bytes = (*bmp).header.len + (*bmp).img.len + (*bmp).num_padded_bytes_per_row * (*bmp).y_pix;
	
	(*bmp).header.arr[0] = 'B';
	(*bmp).header.arr[1] = 'M';
	
	parse_int((*bmp).header, (*bmp).total_num_bytes, 2, 4);
	parse_int((*bmp).header, 54, 10, 4); //Bitmap image data found at this address.
	parse_int((*bmp).header, 40, 14, 4);
	
	parse_int((*bmp).header, (*bmp).x_pix, 18, 4); //#pixels wide
	parse_int((*bmp).header, (*bmp).y_pix, 22, 4); //#pixels height
	parse_int((*bmp).header, 1, 26, 2); //Just set this to 1
	
	parse_int((*bmp).header, 24, 28, 2); //#bits/pixel
	parse_int((*bmp).header, 0, 30, 4); //Compression method
	parse_int((*bmp).header, (*bmp).img.len + (*bmp).num_padded_bytes_per_row * (*bmp).y_pix, 34, 4); //Image size
	
	parse_int((*bmp).header, 0, 38, 4); //pixels per m (horizontal)
	parse_int((*bmp).header, 0, 42, 4); //pixels per m (vertical)
	parse_int((*bmp).header, 0, 46, 4); //# colors
	parse_int((*bmp).header, 0, 50, 4); //#"significant" colors
}
void generate_full(bmp_o* bmp) {
	(*bmp).full = new_bvec((*bmp).total_num_bytes);
	
	vecoverwrite_b((*bmp).full, (*bmp).header, 0);
	
	//What I need now is to just insert 000 into every X amount of bytes.
	//So each row naturally holds x_pix * 3 bytes.
	//So I jump x_pix * 3 bytes. Then insert 000.
	//Then jump forward to account for 000, then jump x_pix * 3....
	
	//So this'd be even better.
	//I insert 000 into (*bmp).full.len. Then I jump back x_pix * 3 bytes.
	//Then insert 000 there. Then jump back again.
	//Do this for each row that exists.
	
	bvec_o zpad = new_bvec((*bmp).num_padded_bytes_per_row);
	int full_ind = 54;
	int img_ind = 0;
	
	//Padding.
	
	//Paste in a row from the image.
	for (img_ind = 0; img_ind < (*bmp).y_pix * (*bmp).x_pix * 3; img_ind++) {
		
		//Zeropad if you're supposed to at this index.
		if (((img_ind % ((*bmp).x_pix * 3)) == 0) && (img_ind != 0)) {
			vecoverwrite_b((*bmp).full, zpad, full_ind);
			full_ind += zpad.len;
		}
		
		//Paste in this row of image data.
		(*bmp).full.arr[full_ind] = (*bmp).img.arr[img_ind];
		full_ind++;
		
	}
}
void package_bitmap(bmp_o* bmp) {
	generate_header(bmp);
	generate_full(bmp);
}
void save_bmp(bmp_o bmp, char* name) {
	package_bitmap(&bmp);
	
	FILE* fout = fopen(name, "w");
	fwrite(bmp.full.arr, sizeof(bmp.full.arr[0]), bmp.full.len, fout);
	fclose(fout);
}

//Functions exclusively for image data of bmp.img.
void set_white(bmp_o bb) {
	int i = 0;
	for (i = 0; i < bb.img.len; i++)
		bb.img.arr[i] = 255;
}
void set_black(bmp_o bb) {
	int i = 0;
	for (i = 0; i < bb.img.len; i++)
		bb.img.arr[i] = 0;
}

//Graph.
//0. Index access.
int get_bmp_img_ind(bmp_o bmp, int x, int y, int which_color) {
	return (y * bmp.x_pix * bmp.bytespp + x * bmp.bytespp + which_color);
}

//1. Pixel level graphing functions.
//1.1 : Points.
int draw_point_onecolor(bmp_o bmp, int x, int y, int which_color, unsigned char val) {
	
	if (x >= bmp.x_pix)
		return 0;//x = bmp.x_pix - 1;
	if (x < 0)
		return 0;//x = 0;
	if (y >= bmp.y_pix)
		return 0;//y = bmp.y_pix -1;
	if (y < 0)
		return 0;//y = 0;
	
	int ind = get_bmp_img_ind(bmp, x, y, which_color);
	bmp.img.arr[ind] = val;
	return 1;
}
void draw_point_rgb(bmp_o bmp, int x, int y, color_o cc) {
	draw_point_onecolor(bmp, x, y, 0, cc.b);
	draw_point_onecolor(bmp, x, y, 1, cc.g);
	draw_point_onecolor(bmp, x, y, 2, cc.r);
}
void draw_thickpoint_rgb(bmp_o bmp, int x, int y, int halfnumpix, color_o cc) {
	int cur_x; int cur_y;
	for (cur_x = -halfnumpix; cur_x < halfnumpix; cur_x++) {
		for (cur_y = halfnumpix; cur_y < halfnumpix; cur_y++) {
			draw_point_rgb(bmp, x + cur_x, y + cur_y, cc);
		}
	}
}

//1.2 : Lines.
void draw_line_seg_vert(bmp_o bmp, int x, int yL, int yH, color_o cc) {
	int y;
	for (y = yL; y < yH; y++)
		draw_point_rgb(bmp, x, y, cc);
}
void draw_line_seg_horiz(bmp_o bmp, int y, int xL, int xH, color_o cc) {
	int x;
	for (x = xL; x < xH; x++)
		draw_point_rgb(bmp, x, y, cc);
}
void draw_ruleV(bmp_o bmp, int x, color_o cc) {
	int cur_y = 0;
	for (cur_y = 0; cur_y < bmp.y_pix; cur_y++)
		draw_point_rgb(bmp, x, cur_y, cc);
}
void draw_ruleH(bmp_o bmp, int y, color_o cc) {
	int cur_x = 0;
	for (cur_x = 0; cur_x < bmp.x_pix; cur_x++)
		draw_point_rgb(bmp, cur_x, y, cc);
}

int draw_line(bmp_o bmp, int x1, int y1, int x2, int y2, color_o cc) {
	
	if (x2 == x1) {
		draw_line_seg_vert(bmp, x1, less(y1, y2), greater(y1, y2), cc);
		return 1;
	}
	
	double m = (double)(y2 - y1) / (double)(x2 - x1);
	double m_inv = 1.0/m;
	
	if (x1 < x2) {
		int cur_x;
		double cur_yd = (double)(y1);;
		for (cur_x = x1; cur_x <= x2; cur_x++) {
			draw_point_rgb(bmp, cur_x, (int)round(cur_yd), cc);
			cur_yd += m;
		}
	}
	else if (x1 > x2) {
		int cur_x;
		double cur_yd = (double)(y1);
		for (cur_x = x1; cur_x >= x2; cur_x--) {
			draw_point_rgb(bmp, cur_x, (int)round(cur_yd), cc);
			cur_yd -= m;
		}
	}
	if (y1 < y2) {
		int cur_y;
		double cur_xd = (double)(x1);
		for (cur_y = y1; cur_y <= y2; cur_y++) {
			draw_point_rgb(bmp, (int)round(cur_xd), cur_y, cc);
			cur_xd += m_inv;
		}
	}
	else if (y1 > y2) {
		int cur_y;
		double cur_xd = (double)(x1);
		for (cur_y = y1; cur_y >= y2; cur_y--) {
			draw_point_rgb(bmp, (int)round(cur_xd), cur_y, cc);
			cur_xd -= m_inv;
		}
	}
}

//1.3 : Complex shapes.
int draw_circle(bmp_o bmp, int xc, int yc, int radius, color_o cc) {
	double theta;
	double r = (double)(radius);
	
	while (theta < 2.0*M_PI) {
		draw_point_rgb(bmp, xc + (int)round(r*cos(theta)), yc + (int)round(r*sin(theta)), cc);
		theta += 0.01;
	}
}
void draw_cross(bmp_o bmp, int xc, int yc, int radius, color_o cc) {
	draw_line(bmp, xc, yc - radius, xc, yc + radius, cc);
	draw_line(bmp, xc - radius, yc, xc + radius, yc, cc);
}

//2. Coordinate level graphing functions.
//2.0: Mapping.
//2.0.1: Mapping graph coordinates to pixel coordinates.
int map_graph2pix_X(graph_o g, bmp_o bmp, double xd) {
	return (int)((xd - g.x_min) / (g.x_max - g.x_min) * bmp.x_pix);
}
int map_graph2pix_Y(graph_o g, bmp_o bmp, double yd) {
	return (int)((yd - g.y_min) / (g.y_max - g.y_min) * bmp.y_pix);
}
void map_graph2pix_2D(graph_o g, bmp_o bmp, double xd, double yd, int* xp, int* yp) {
	*xp = map_graph2pix_X(g, bmp, xd);
	*yp = map_graph2pix_Y(g, bmp, yd);
}

double map_segment(graph_o g, bmp_o bmp, double len, double theta) {
	
	//The horizontal component needs to be multiplied by:
		//bmp.x_pix / (graph.x_max - graph.x_min)
	//The vertical component needs to be multiplied by:
		//bmp.y_pix / (graph.y_max - graph.y_min)
	
	//The horizontal component is given by:
		//r*cos(theta).
	//The vertical component is given by:
		//r*sin(theta).
	//Final desired length is (r*cos(theta))^2 + (r*sin(theta))^2.
	
	//So what DO I do if, say, I wanna plot a circle in a non-square plot.
		//That just comes back to the draw_circle function.
		//I tell the draw_circle function to scale the radius each time.
		//mm.
		
	double new_x = len*cos(theta) * (double)bmp.x_pix / (g.x_max - g.x_min);
	double new_y = len*sin(theta) * (double)bmp.y_pix / (g.y_max - g.y_min);
	return sqrt((new_x * new_x + new_y * new_y));
}

//2.1: Graphing.
void draw_point_rgb_g(graph_o g, bmp_o bmp, double xd, double yd, color_o cc) {
	int xp = 0; int yp = 0;
	map_graph2pix_2D(g, bmp, xd, yd, &xp, &yp);
	draw_point_rgb(bmp, xp, yp, cc);
}
void draw_thickpoint_rgb_g(graph_o g, bmp_o bmp, double xd, double yd, int halfnumpix, color_o cc) {
	int xp = 0; int yp = 0; int cur;
	map_graph2pix_2D(g, bmp, xd, yd, &xp, &yp);
	for (cur = -halfnumpix; cur < halfnumpix; cur++)
		draw_point_rgb(bmp, xp, yp + cur, cc);
}

//2.1.1: Shapes / lines.
void draw_ruleV_g(graph_o g, bmp_o bmp, double x, color_o cc) {
	int xp;
	map_graph2pix_X(g, bmp, x);
	draw_ruleV(bmp, xp, cc);
}
void draw_ruleH_g(graph_o g, bmp_o bmp, double y, color_o cc) {
	int yp;
	map_graph2pix_Y(g, bmp, y);
	draw_ruleH(bmp, yp, cc);
}
int draw_line_g(graph_o g, bmp_o bmp, line_segment_o lin, color_o cc) {
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	
	map_graph2pix_2D(g, bmp, lin.x1, lin.y1, &x1, &y1);
	map_graph2pix_2D(g, bmp, lin.x2, lin.y2, &x2, &y2);
	
	draw_line(bmp, x1, y1, x2, y2, cc);
}

void draw_circle_g(graph_o g, bmp_o bmp, circle_o cir, color_o cc) {
	
	//First step is map circle values onto here.
	int xp = 0;
	int yp = 0;
	map_graph2pix_2D(g, bmp, cir.xc, cir.yc, &xp, &yp);
	
	double theta = 0;
	double radius = map_segment(g, bmp, cir.r, theta);
	
	while (theta < 2.0*M_PI) {
		//radius = map_segment(g, bmp, cir.r, theta);
		draw_point_rgb(bmp, xp + (int)round(radius*cos(theta)), yp + (int)round(radius*sin(theta)), cc);
		theta += 0.01;
	}
}
void draw_cross_g(graph_o g, bmp_o bmp, circle_o cir, color_o cc) {
	int x_int = 0;
	int y_int = 0;
	map_graph2pix_2D(g, bmp, cir.xc, cir.yc, &x_int, &y_int);
	int radius_int = (int)round(map_segment(g, bmp, cir.r, 0));
	draw_cross(bmp, x_int, y_int, radius_int, cc);
}

//2.1.2: Plots : Grid control.
void draw_axes(graph_o g, bmp_o bmp, color_o cc) {
	
	//If g.y_max < 0 or g.y_min > 0, it means that we are above the x-axis.
		//THe x-axis doesn't exist on our screen.
	if (!((g.y_max < 0) || (g.y_min > 0))) {
		line_segment_o lin_h;
			lin_h.x1 = g.x_max;
			lin_h.y1 = 0;
			lin_h.x2 = g.x_min;
			lin_h.y2 = 0;
		
		draw_line_g(g, bmp, lin_h, cc);
	}
	
	if (!((g.x_max < 0) || (g.x_min > 0))) {
		line_segment_o lin_v;
			lin_v.x1 = 0;
			lin_v.y1 = g.y_max;
			lin_v.x2 = 0;
			lin_v.y2 = g.y_min;
			
		draw_line_g(g, bmp, lin_v, cc);
	}
}
void markaxis_X(graph_o g, bmp_o bmp, double grid_interval) {
	//Starting at x = 0.
		//Go a certain distance.
			//How long do we go?
			//I can tell you an interval. Simple.
		//Draw a line of a certain size.
			//How long?
				//Should be a constant pixel height, every time.
				//Say, 1% of the vertical dimension.
		//You have to do this in each direction.
	
	//Start x == 0 and go right.
		//Compute how many marks you need to make. Round down.
		//For each mark you have to make, make a tick mark of a certain height.
		//Increase your x position by grid_interval.
	//Start at x == 0 and go left.
		//Same as above, just decrease x position by grid_interval at a time.
	
	int cur_mark_ind;
	int num_marks;
	line_segment_o lin;
	
	double cur_xpos;
		int cur_xpos_pix;
		int cur_ypos_pix = map_graph2pix_Y(g, bmp, 0);
	
	int dtick = (int)(0.01 * (double)bmp.y_pix);
	
	color_o cc = new_color_rgb(0, 0, 0);
		
	if (g.x_max > 0) {
		cur_xpos = 0;
		cur_xpos_pix = 0;
		
		num_marks = (int)(g.x_max / grid_interval);
		
		for (cur_mark_ind = 0; cur_mark_ind <= num_marks; cur_mark_ind++) {
			cur_xpos_pix = map_graph2pix_X(g, bmp, cur_xpos);
			draw_line_seg_vert(bmp, cur_xpos_pix, cur_ypos_pix - dtick, cur_ypos_pix + dtick, cc);
			cur_xpos += grid_interval;
		}
	} 
	if (g.x_min < 0) {
		cur_xpos = 0;
		cur_xpos_pix = 0;
		
		num_marks = (int)(-g.x_min / grid_interval);
		
		for (cur_mark_ind = 0; cur_mark_ind <= num_marks; cur_mark_ind++) {
			cur_xpos_pix = map_graph2pix_X(g, bmp, cur_xpos);
			draw_line_seg_vert(bmp, cur_xpos_pix, cur_ypos_pix - dtick, cur_ypos_pix + dtick, cc);
			cur_xpos -= grid_interval;
		}
	}
}
void markaxis_Y(graph_o g, bmp_o bmp, double grid_interval) {
	
	int cur_mark_ind;
	int num_marks;
	line_segment_o lin;
	
	double cur_ypos;
		int cur_ypos_pix;
		int cur_xpos_pix = map_graph2pix_X(g, bmp, 0);
	
	int dtick = (int)(0.01 * (double)bmp.y_pix);
	
	color_o cc = new_color_rgb(0, 0, 0);
		
	if (g.y_max > 0) {
		cur_ypos = 0;
		cur_ypos_pix = 0;
		
		num_marks = (int)(g.y_max / grid_interval);
		
		for (cur_mark_ind = 0; cur_mark_ind <= num_marks; cur_mark_ind++) {
			cur_ypos_pix = map_graph2pix_Y(g, bmp, cur_ypos);
			draw_line_seg_horiz(bmp, cur_ypos_pix, cur_xpos_pix - dtick, cur_xpos_pix + dtick, cc);
			cur_ypos += grid_interval;
		}
	} 
	if (g.y_min < 0) {
		cur_ypos = 0;
		cur_ypos_pix = 0;
		
		num_marks = (int)(-g.y_min / grid_interval);
		
		for (cur_mark_ind = 0; cur_mark_ind <= num_marks; cur_mark_ind++) {
			cur_ypos_pix = map_graph2pix_Y(g, bmp, cur_ypos);
			draw_line_seg_horiz(bmp, cur_ypos_pix, cur_xpos_pix - dtick, cur_xpos_pix + dtick, cc);
			cur_ypos -= grid_interval;
		}
	}
}

//2.1.3: Plots : Low-level.
int draw_xyplot_g(graph_o g, bmp_o bmp, dvec_o x_vals, dvec_o y_vals, color_o cc) {
	
	if (x_vals.len != y_vals.len)
		return 0;
	
	int n = 0;
	for (n = 0; n < x_vals.len; n++)
		draw_thickpoint_rgb_g(g, bmp, x_vals.arr[n], y_vals.arr[n], 8, cc);
	return 1;
}
int draw_rthetaplot_g(graph_o g, bmp_o bmp, dvec_o theta_vals, dvec_o r_vals, color_o cc) {
	if (theta_vals.len != r_vals.len)
		return 0;
	
	int n = 0;
	for (n = 0; n < theta_vals.len; n++)
		draw_point_rgb_g(g, bmp, r_vals.arr[n] * cos(theta_vals.arr[n]), r_vals.arr[n] * sin(theta_vals.arr[n]), cc);
	return 1;
}
int draw_stemplot_g(graph_o g, bmp_o bmp, int offset, dvec_o y_vals, color_o cc) {
	
	line_segment_o lin;
	int n = offset; int xc; int yc;
	for (n = offset; n < offset + y_vals.len; n++) {
		//Draw a line segment.
			//Point 1 is on the x-axis (y = 0) and is at x = n in global coordinate space.
				//Point 1: (n, 0).
			//Point 2 is at y = y_vals.arr[n] and is at x = n in the global coordinate.
				//Point 2: (n, y_vals.arr[n]);
			
			lin.x1 = (double)n;
			lin.y1 = 0;
			lin.x2 = (double)n;
			lin.y2 = y_vals.arr[n - offset];
			
			draw_line_g(g, bmp, lin, cc);
				map_graph2pix_2D(g, bmp, lin.x1, lin.y2, &xc, &yc);
			draw_circle(bmp, xc, yc, 4, cc);
	}
	
	//draw_xyplot_g(g, bmp, x_vals, y_vals, cc);
	
	return 1;
}

//2.1.4: Plots : High-level.
void signal_to_stemplot_bmp(dvec_o xn, char* name) {
	
	graph_o g;
		g.y_min = xn.arr[min(xn, 0, xn.len - 1)] - 5.0;
		g.y_max = xn.arr[max(xn, 0, xn.len - 1)] + 5.0;
		g.x_min = -1.0;
		g.x_max = (double)xn.len * 1.1;
	
	color_o cc_red = new_color_rgb(255, 0, 0);
	color_o cc_black = new_color_rgb(0, 0, 0);
	
	bmp_o bmp_xn = new_bmp(1000,1000);
		set_white(bmp_xn);
		draw_axes(g, bmp_xn, cc_black);
		
	draw_stemplot_g(g, bmp_xn, 0, xn, cc_red);
	markaxis_X(g, bmp_xn, 1.0);
	markaxis_Y(g, bmp_xn, 1.0);
	save_bmp(bmp_xn, name);
}
void fft_mags_to_bmp(cpx_vec_o Xk, char* name) {
	dvec_o Xk_mags = downcast_cpx(Xk, "mag");
	
	graph_o g;
		g.x_min = -1.0;
		g.x_max = Xk_mags.len;
		g.y_min = 0;
		g.y_max = Xk_mags.arr[max(Xk_mags, 0, Xk_mags.len - 1)] * 1.1;
	
	color_o cc_red = new_color_rgb(255, 0, 0);
	color_o cc_black = new_color_rgb(0, 0, 0);
	
	bmp_o bmp_mags = new_bmp(800, 800);
		set_white(bmp_mags);
		draw_axes(g, bmp_mags, cc_black);
	
	draw_stemplot_g(g, bmp_mags, 0, Xk_mags, cc_red);
	markaxis_X(g, bmp_mags, 1);
	markaxis_Y(g, bmp_mags, 0.5);
	save_bmp(bmp_mags, name);
}

#endif