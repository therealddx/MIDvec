/*
 * reference LICENSE file provided.
 *
 * glTriangle.h
 *
 * Functions and data for dynamically plotting triangles into a 2D 
 * OpenGL scene.
 *
 */

#ifndef GLTRIANGLE_H
#define GLTRIANGLE_H

#include <opengl/glSetup.h>
#include <opengl/buildShader.h>

typedef enum TriangleVertex_e
{
  Vert1 = 0,
  Vert2 = 1,
  Vert3 = 2
} TriangleVertex;

typedef struct
{
  float* _vertices;
  GLuint _vbo;
  GLuint _vao;
  GLuint _shaderProgram;
} glTriangle_o;

glTriangle_o new_glTriangle();

void del_glTriangle(glTriangle_o);

void setTriangleVertex(glTriangle_o, TriangleVertex, float, float);

void drawTriangle(glTriangle_o);

void configureVertShader
  ( glTriangle_o arg_tri
  , float arg_Sx
  , float arg_Sy
  , float arg_Tx
  , float arg_Ty
  );

#endif // GLTRIANGLE_H

