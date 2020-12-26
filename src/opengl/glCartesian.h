/*
 * reference LICENSE file provided.
 *
 * glCartesian.h
 *
 * Provide a set of functions and data definitions for plotting data within
 * a Cartesian plane coordinate space.
 *
 */

#ifndef GLCARTESIAN_H
#define GLCARTESIAN_H

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <opengl/glSetup.h>
#include <opengl/buildShader.h>

static const u_int32_t VERTS_PER_CARTESIAN_POINT = 3;

static const u_int32_t CARTESIAN_POINT_SIZE =
  sizeof(float) * VERTS_PER_CARTESIAN_POINT * DIM_PER_VERT;

static const u_int32_t CARTESIAN_POINT_CAPACITY = 1024;

typedef struct
{
  float _x;
  float _y;
} cartesianPoint_o;

typedef struct
{
  float* _vertices;
  GLuint _vbo;
  GLuint _vao;
} glCartesianPoint_o;

typedef struct
{
  glCartesianPoint_o* _w;
  glCartesianPoint_o* _r;
  glCartesianPoint_o* _points;
  u_int32_t _len;
  GLuint _shaderProgram;
} glCartesian_o;

glCartesianPoint_o new_glCartesianPoint(cartesianPoint_o*);

void del_glCartesianPoint(glCartesianPoint_o);

glCartesian_o new_glCartesian();

void del_glCartesian(glCartesian_o);

void set_glCartesianPoint(glCartesianPoint_o, float, float);

void set_glCartesian(glCartesian_o, float, float, float, float);

void get_glCartesian(glCartesian_o, float*, float*, float*, float*);

void draw_glCartesian(glCartesian_o);

// todo: more functions to write.
// 

void plotPoints
  ( glCartesian_o arg_plane
  , glCartesianPoint_o* arg_newPoints
  , u_int32_t arg_numNewPoints
  );

#endif // GLCARTESIAN_H

