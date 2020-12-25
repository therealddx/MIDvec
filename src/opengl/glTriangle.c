/*
 * reference LICENSE file provided.
 *
 * glTriangle.c
 *
 * Implements glTriangle.h.
 *
 */

#include <stdio.h>

#include "glTriangle.h"

static const GLuint VERTS_PER_TRIANGLE = 3;

static const GLuint TRIANGLE_SIZE =
  VERTS_PER_TRIANGLE * DIM_PER_VERT * sizeof(float);

glTriangle_o new_glTriangle()
{
  glTriangle_o toReturn;

  toReturn._vertices = (float*)malloc(TRIANGLE_SIZE);

  toReturn._vertices[0] = 0.0f;
  toReturn._vertices[1] = 0.0f;
  toReturn._vertices[2] = 0.0f;

  toReturn._vertices[3] = -0.5f;
  toReturn._vertices[4] = -0.5f;
  toReturn._vertices[5] = 0.0f;

  toReturn._vertices[6] = 0.5f;
  toReturn._vertices[7] = -0.5f;
  toReturn._vertices[8] = 0.0f;

  // vbo.
  //
  glGenBuffers(1, &toReturn._vbo);
  glBindBuffer(GL_ARRAY_BUFFER, toReturn._vbo);
  glBufferData(
    GL_ARRAY_BUFFER, TRIANGLE_SIZE, toReturn._vertices, GL_DYNAMIC_DRAW);

  // vao.
  //
  glGenVertexArrays(1, &toReturn._vao);
  glBindVertexArray(toReturn._vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, DIM_PER_VERT * sizeof(float), (void*)0);

  // shaders.
  //
  bpShader_o bpVertShader =
    new_bpShader("shaders/neutVert.glsl", GL_VERTEX_SHADER);
  bpShader_o bpFragShader =
    new_bpShader("shaders/neutFrag.glsl", GL_FRAGMENT_SHADER);
  bpShader_o shaderBlueprints[] = { bpVertShader, bpFragShader };
  toReturn._shaderProgram = buildShader(shaderBlueprints, 2);

  glUseProgram(toReturn._shaderProgram);

  // ret.
  //
  return toReturn;
}

void setTriangleVertex
  ( glTriangle_o arg_tri
  , TriangleVertex arg_vert
  , float arg_x
  , float arg_y
  )
{
  // arg check.
  //
  if (arg_tri._vertices == NULL)
  {
    fprintf(stderr, "setTriangleVertex: uninitialized input triangle");
  }

  // store new values.
  //
  int vertOffset = (int)arg_vert * (int)DIM_PER_VERT;
  arg_tri._vertices[vertOffset + 0] = arg_x;
  arg_tri._vertices[vertOffset + 1] = arg_y;

  // push those changes.
  // 
  // "hey, opengl. pay attention to this VBO."
  //
  glBindBuffer(GL_ARRAY_BUFFER, arg_tri._vbo);

  // "dump this application data in it."
  //
  glBufferData(
    GL_ARRAY_BUFFER, TRIANGLE_SIZE, arg_tri._vertices, GL_DYNAMIC_DRAW);
}

