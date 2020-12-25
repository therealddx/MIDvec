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
    new_bpShader("shaders/2dtrans.glsl", GL_VERTEX_SHADER);
  bpShader_o bpFragShader =
    new_bpShader("shaders/neutFrag.glsl", GL_FRAGMENT_SHADER);
  bpShader_o shaderBlueprints[] = { bpVertShader, bpFragShader };
  toReturn._shaderProgram = buildShader(shaderBlueprints, 2);

  // ret.
  //
  return toReturn;
}

void del_triangle(glTriangle_o arg_tri)
{
  // _vertices: delete unconditionally.
  //   data was malloc'd in; no longer relevant.
  //
  free(arg_tri._vertices);

  // _vbo: delete unconditionally.
  //   VBO is designed for one-to-one association with backing-data.
  // 
  glDeleteBuffers(1, &arg_tri._vbo);

  // _vao: delete unconditionally.
  //   VAO is designed for one-to-one association with VBO.
  //
  glDeleteVertexArrays(1, &arg_tri._vao);

  // _shaderProgram: delete unconditionally.
  //   the shader program is created by 'new', so 'del' should destroy it.
  // 
  glDeleteProgram(arg_tri._shaderProgram);
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

void drawTriangle(glTriangle_o arg_tri)
{
  // "hey, opengl. activate my shader."
  // "then get backing data from my VBO and VAO."
  // 
  glUseProgram(arg_tri._shaderProgram);
  glBindBuffer(GL_ARRAY_BUFFER, arg_tri._vbo);
  glBindVertexArray(arg_tri._vao);

  // "now draw what you see."
  //
  glDrawArrays(GL_TRIANGLES, 0, VERTS_PER_TRIANGLE);
}

void configureVertShader
  ( glTriangle_o arg_tri
  , float arg_Sx
  , float arg_Sy
  , float arg_Tx
  , float arg_Ty
  )
{
  // "hey, opengl. i'm modifying uniforms in -this- shader program.
  //
  glUseProgram(arg_tri._shaderProgram);

  // set its uniforms.
  // 
  GLint uloc_Sx =
    glGetUniformLocation(arg_tri._shaderProgram, "u_Sx");
  glUniform1f(uloc_Sx, arg_Sx);

  GLint uloc_Sy =
    glGetUniformLocation(arg_tri._shaderProgram, "u_Sy");
  glUniform1f(uloc_Sy, arg_Sy);

  GLint uloc_Tx =
    glGetUniformLocation(arg_tri._shaderProgram, "u_Tx");
  glUniform1f(uloc_Tx, arg_Tx);

  GLint uloc_Ty =
    glGetUniformLocation(arg_tri._shaderProgram, "u_Ty");
  glUniform1f(uloc_Ty, arg_Ty);
}


