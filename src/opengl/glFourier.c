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

  // allocate VBO IDs (calloc) and VBOs (OpenGL space).
  //
  rtn_context._binsVbo = (GLuint*)calloc(rtn_context._numBins, sizeof(GLuint));
  glGenBuffers(rtn_context._numBins, rtn_context._binsVbo);

  // shader: assign basic vert / frag shaders.
  //
  bpShader_o vertShader = new_bpShader("fourierVert.glsl", GL_VERTEX_SHADER);
  bpShader_o fragShader = new_bpShader("fourierFrag.glsl", GL_FRAGMENT_SHADER);
  bpShader_o shaderBlueprints[] = { bpVertShader, bpFragShader };
  rtn_context._shaderProgram = buildShader(shaderBlueprints, 2);

  // ret.
  return sel_numBins;
}

void updateGraph(cvec_o arg_fft, glFourierContext_o arg_context)
{
  // vars.
  u_int64_t _n = 0;
  
  // prepare the shader uniforms.
  //
  // <todo> 
  GLint uloc_xMin = glGetUniformLocation(shaderProgram, 

  // for each complex value.
  //
  for (_n = 0; _n < arg_fft.len; _n++)
  {
  }
    // get its corresponding VBO id.
    //
    glBindBuffer(GL_ARRAY_BUFFER, arg_vboBins[_n]);

    float lineVertices[] = 
    { (float)_n,              0.0, 0.0
    , (float)_n, cmag(arg_fft[_n], 0.0);
    };

    // buffer data.
    //
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(lineVertices), 2, GL_DYNAMIC_DRAW);

}

