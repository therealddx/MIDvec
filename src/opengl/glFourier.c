/*
 * reference LICENSE file provided.
 *
 * glFourier.c
 * 
 * Implements glFourier.h.
 *
 */

#include <stdlib.h>
#include <unistd.h>

#include <dsp/dspFourier.h>
#include <opengl/buildShader.h>

#include "glFourier.h"

glFourierContext_o initializeFourierContext(GLuint arg_numBins)
{
  // vars.
  glFourierContext_o rtn_context;

  // setup glew / glfw.
  //
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  rtn_context._window = glfwCreateWindow(800, 600, "Fourier", NULL, NULL);
  glfwMakeContextCurrent(rtn_context._window);

  glewInit();

  // arg check: arg_numBins should be a power of two for FFT.
  // 
  GLuint sel_numBins = 2;
  while (sel_numBins < arg_numBins) { sel_numBins <<= 1; }
  rtn_context._numBins = sel_numBins;

  // allocate VBO IDs (malloc) and VBOs (OpenGL space).
  //
  rtn_context._binsVbo =
    (GLuint*)malloc(rtn_context._numBins * sizeof(GLuint));
  glGenBuffers(rtn_context._numBins, rtn_context._binsVbo);

  // allocate VBO backing storage (malloc).
  //
  // 1. allocate '_numBins' amount of float* objects.
  //  each 'float*' you allocate is one array of vertices.
  //
  // 2. allocate vertex components for each float*.
  //  (x,y,z);(x,y,z);(x,y,z) == 3 vertices * 3 components.
  //  
  // rtn_context._binsVert[_n] = a float* of vertices
  // rtn_context._binsVert[_n][_k] = a (x, y, z) component.
  // 
  rtn_context._binsVert =
    (float**)malloc(rtn_context._numBins * sizeof(float*));

  GLuint _n = 0;
  for (_n = 0; _n < rtn_context._numBins; _n++)
  {
    rtn_context._binsVert[_n] = (float*)malloc(3 * 3 * sizeof(float));
  }

  // allocate VAO ID.
  // 
  glGenVertexArrays(1, &rtn_context._vao);
  glBindVertexArray(rtn_context._vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // shader:
  // 1. assign basic vert / frag shaders.
  //
  bpShader_o bpVertShader =
    new_bpShader("shaders/fourierVert.glsl", GL_VERTEX_SHADER);
  bpShader_o bpFragShader =
    new_bpShader("shaders/fourierFrag.glsl", GL_FRAGMENT_SHADER);
  bpShader_o shaderBlueprints[] = { bpVertShader, bpFragShader };
  rtn_context._shaderProgram = buildShader(shaderBlueprints, 2);

  // 2. prepare the shader uniforms.
  //  a. x min gets 0, first bin.
  //  b. x max gets number of bins.
  //  c. y min gets 0, minimum-magnitude.
  //  d. y max gets 1.0, maximum-magnitude.
  // 

  // a.
  GLint uloc_xMin =
    glGetUniformLocation(rtn_context._shaderProgram, "u_xMin");
  glUniform1f(uloc_xMin, 0.0f);

  // b.
  GLint uloc_xMax =
    glGetUniformLocation(rtn_context._shaderProgram, "u_xMax");
  glUniform1f(uloc_xMax, (GLfloat)rtn_context._numBins);

  // c.
  GLint uloc_yMin =
    glGetUniformLocation(rtn_context._shaderProgram, "u_yMin");
  glUniform1f(uloc_yMin, 0.0f);

  // d.
  GLint uloc_yMax =
    glGetUniformLocation(rtn_context._shaderProgram, "u_yMax");
  glUniform1f(uloc_yMax, 1.0f);

  // 3. use the shader.
  //
  glUseProgram(rtn_context._shaderProgram);

  // ret.
  return rtn_context;
}

void updateGraph(cvec_o arg_fft, glFourierContext_o arg_context)
{
  // arg check: 'arg_fft.len' must equal num bins in 'arg_context'.
  //
  if (arg_fft.len != arg_context._numBins)
  {
    fprintf(stderr, "updateGraph: input fft len is inequal to graphable bins\n");
    assert(0);
  }

  // vars.
  u_int64_t _n = 0;
  
  // for each complex value.
  //
  for (_n = 0; _n < arg_fft.len; _n++)
  {
    // copy that data into that VBO id.
    //
    glBufferData
      ( GL_ARRAY_BUFFER
      , 3 * 3 * sizeof(float) // sizeof(lineVertices)
      , arg_context._binsVert[_n] // lineVertices
      , GL_DYNAMIC_DRAW
      );

    // sizeof(lineVertices) => 3 * 3 * sizeof(float)
    //   3 vertices * 3 components per vertex * bytes per float
    //
    // lineVertices => arg_context._binsVert[_n]
    //   arg_context._binsVert[_n] is a float* of x-y-z stored vertices.
    //
  }
}

double cmag(double complex arg)
{
  return sqrt(creal(arg) * creal(arg) + cimag(arg) * cimag(arg));
}

