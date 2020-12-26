/*
 * reference LICENSE file provided.
 *
 * glCartesian.c
 *
 * Implements glCartesian.h.
 *
 */

#include "glCartesian.h"

glCartesianPoint_o new_glCartesianPoint(cartesianPoint_o* arg_origPos)
{
  /*
   * typedef struct
   * {
   *   float* _vertices;
   *   GLuint _vbo;
   *   GLuint _vao;
   * } glCartesianPoint_o;
   *
   */

  // vars.
  //
  glCartesianPoint_o toReturn;

  // _vertices.
  //
  toReturn._vertices = (float*)malloc(CARTESIAN_POINT_SIZE);

  u_int32_t _n = 0;
  for (_n = 0; _n < (VERTS_PER_CARTESIAN_POINT * DIM_PER_VERT); _n++);
  {
    toReturn._vertices[_n] = 0.0f;
  }

  if (arg_origPos != NULL)
  {
    set_glCartesianPoint(toReturn, arg_origPos->_x, arg_origPos->_y);
  }
  else
  {
    set_glCartesianPoint(toReturn, 0.0f, 0.0f);
  }

  // vbo.
  //
  glGenBuffers(1, &toReturn._vbo);
  glBindBuffer(GL_ARRAY_BUFFER, toReturn._vbo);
  glBufferData(
    GL_ARRAY_BUFFER, CARTESIAN_POINT_SIZE,
    toReturn._vertices, GL_DYNAMIC_DRAW);
 
  // vao.
  //
  glGenVertexArrays(1, &toReturn._vao);
  glBindVertexArray(toReturn._vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, DIM_PER_VERT * sizeof(float), (void*)0);

  // ret.
  //
  return toReturn;
}

void del_glCartesianPoint(glCartesianPoint_o arg_point)
{
  // release _vertices / _vbo / _vao.
  //
  free(arg_point._vertices);
  glDeleteBuffers(1, &arg_point._vbo);
  glDeleteVertexArrays(1, &arg_point._vao);
}

glCartesian_o new_glCartesian()
{
  /*
   * typedef struct
   * {
   *   glCartesianPoint_o* _w;
   *   glCartesianPoint_o* _r;
   *   glCartesianPoint_o* _points;
   *   GLuint _shaderProgram;
   * } glCartesian_o;
   *
   */

  // vars.
  //
  glCartesian_o toReturn;

  // points + ringbuffer pointers.
  //
  toReturn._points = (glCartesianPoint_o*)malloc(
    CARTESIAN_POINT_CAPACITY * CARTESIAN_POINT_SIZE);
  u_int32_t _n = 0;
  for (_n = 0; _n < CARTESIAN_POINT_CAPACITY; _n++)
  {
    toReturn._points[_n] = new_glCartesianPoint(NULL);
  }
  toReturn._w = toReturn._points;
  toReturn._r = toReturn._points;
  toReturn._len = CARTESIAN_POINT_CAPACITY;

  // shaders.
  //
  bpShader_o bpVertShader = 
    new_bpShader("shaders/2dtrans.glsl", GL_VERTEX_SHADER);
  bpShader_o bpFragShader =
    new_bpShader("shaders/neutFrag.glsl", GL_FRAGMENT_SHADER);
  bpShader_o shaderBlueprints[] = { bpVertShader, bpFragShader };
  toReturn._shaderProgram = buildShader(shaderBlueprints, 2);

  set_glCartesian(toReturn, -1.0f, 1.0f, -1.0f, 1.0f);

  // ret.
  //
  return toReturn;
}

void del_glCartesian(glCartesian_o arg_carte)
{
  // release all glCartesianPoints, then release this memory.
  //
  u_int32_t _n = 0;
  for (_n = 0; _n < CARTESIAN_POINT_CAPACITY; _n++)
  {
    del_glCartesianPoint(arg_carte._points[_n]);
  }
  free(arg_carte._points);
  arg_carte._w = NULL;
  arg_carte._r = NULL;
  arg_carte._len = 0;

  // release shadprog.
  // 
  glDeleteProgram(arg_carte._shaderProgram);
}

void set_glCartesianPoint(
  glCartesianPoint_o arg_glcp, float arg_x, float arg_y)
{
  // push to PK-vertex of triangle.
  //
  arg_glcp._vertices[0] = arg_x;
  arg_glcp._vertices[1] = arg_y;
  arg_glcp._vertices[2] = 0.0f;

  // push to non-primary vertices.
  //
  arg_glcp._vertices[3] = arg_x - 0.01f;
  arg_glcp._vertices[4] = arg_y - 0.01f;
  arg_glcp._vertices[5] = 0.0f;

  arg_glcp._vertices[6] = arg_x + 0.01f;
  arg_glcp._vertices[7] = arg_y - 0.01f;
  arg_glcp._vertices[8] = 0.0f;

  // push changes to opengl; VBO data.
  //
  glBindBuffer(GL_ARRAY_BUFFER, arg_glcp._vbo);
  glBufferData(
    GL_ARRAY_BUFFER, CARTESIAN_POINT_SIZE,
    arg_glcp._vertices, GL_DYNAMIC_DRAW);
}

void set_glCartesian
  ( glCartesian_o arg_glc
  , float arg_xMin
  , float arg_xMax
  , float arg_yMin
  , float arg_yMax
  )
{
  // arg check: bounds.
  //
  if ( (arg_xMax <= arg_xMin)
    || (arg_yMax <= arg_yMin)
    )
  {
    fprintf(stderr, "set_glCartesian: bad bounds given");
    assert(0);
  }

  // look at this shader program.
  //
  glUseProgram(arg_glc._shaderProgram);

  // develop new scale/shift values given desired view-window.
  // mapping is assumed to NDC [-1.0, 1.0] bounds.
  //
  float new_Sx = (1.0f - -1.0f) / (arg_xMax - arg_xMin);
  float new_Sy = (1.0f - -1.0f) / (arg_yMax - arg_yMin);
  float new_Tx = -1.0f - (arg_xMin * new_Sx);
  float new_Ty = -1.0f - (arg_yMin * new_Sy);

  // push new scale/shift values to opengl.
  //
  GLint uloc_Sx = glGetUniformLocation(arg_glc._shaderProgram, "u_Sx");
  GLint uloc_Sy = glGetUniformLocation(arg_glc._shaderProgram, "u_Sy");
  GLint uloc_Tx = glGetUniformLocation(arg_glc._shaderProgram, "u_Tx");
  GLint uloc_Ty = glGetUniformLocation(arg_glc._shaderProgram, "u_Ty");

  glUniform1f(uloc_Sx, new_Sx);
  glUniform1f(uloc_Sy, new_Sy);
  glUniform1f(uloc_Tx, new_Tx);
  glUniform1f(uloc_Ty, new_Ty);
}

void get_glCartesian
  ( glCartesian_o arg_glc
  , float* rtn_xMin
  , float* rtn_xMax
  , float* rtn_yMin
  , float* rtn_yMax
  )
{
  // arg check: null pointers
  //
  if ( rtn_xMin == NULL
    || rtn_xMax == NULL
    || rtn_yMin == NULL
    || rtn_yMax == NULL)
  {
    fprintf(stderr, "get_glCartesian: null reference");
    assert(0);
  }

  // vars.
  // 
  float rtn_Sx;
  float rtn_Sy;
  float rtn_Tx;
  float rtn_Ty;

  // retrieve uniform locations.
  //
  GLint uloc_Sx = glGetUniformLocation(arg_glc._shaderProgram, "u_Sx");
  GLint uloc_Sy = glGetUniformLocation(arg_glc._shaderProgram, "u_Sy");
  GLint uloc_Tx = glGetUniformLocation(arg_glc._shaderProgram, "u_Tx");
  GLint uloc_Ty = glGetUniformLocation(arg_glc._shaderProgram, "u_Ty");

  // retrieve uniform values.
  // 
  glGetUniformfv(arg_glc._shaderProgram, uloc_Sx, &rtn_Sx);
  glGetUniformfv(arg_glc._shaderProgram, uloc_Sy, &rtn_Sy);
  glGetUniformfv(arg_glc._shaderProgram, uloc_Tx, &rtn_Tx);
  glGetUniformfv(arg_glc._shaderProgram, uloc_Ty, &rtn_Ty);

  // map uniform values to window bounds.
  //
  *rtn_xMin = -1.0f * (rtn_Tx + 1.0f) / rtn_Sx;
  *rtn_yMin = -1.0f * (rtn_Ty + 1.0f) / rtn_Sy;
  
  *rtn_xMax = 2.0f / rtn_Sx + *rtn_xMin;
  *rtn_yMax = 2.0f / rtn_Sy + *rtn_yMin;
}

void draw_glCartesian(glCartesian_o arg_glc)
{
  // get the shadprog.
  //
  glUseProgram(arg_glc._shaderProgram);

  // for each point you have, lock-on and draw.
  // 
  u_int32_t _n = 0;
  for (_n = 0; _n < CARTESIAN_POINT_CAPACITY; _n++)
  {
    glBindBuffer(GL_ARRAY_BUFFER, arg_glc._points[_n]._vbo);
    glBindVertexArray(arg_glc._points[_n]._vao);
    glDrawArrays(GL_TRIANGLES, 0, VERTS_PER_CARTESIAN_POINT);
  }
}

