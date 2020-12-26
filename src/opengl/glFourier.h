/*
 * reference LICENSE file provided.
 *
 * glFourier.h.
 *
 * Provide a context for displaying frequency-domain information.
 *
 */

#ifndef GLFOURIER_H
#define GLFOURIER_H

#include <sys/types.h>
#include <complex.h>
#include <math.h>

#include <math/vec.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct glFourierContext
{
  GLFWwindow* _window;
  GLuint _numBins;
  GLuint* _binsVbo; // one bin => one drawn line => one VBO.
  float** _binsVert; // backing-data storage, associated with _binsVbo.
  GLuint _vao; // VAO that binds to VBOs.
  GLuint _shaderProgram;
} glFourierContext_o;

// 
// initializeFourierContext.
//
// Initialize OpenGL so that it's ready to draw graphs specific to Fourier
// analysis.
//
glFourierContext_o initializeFourierContext(GLuint arg_numBins);

// 
// updateGraph.
//
// Given an array of complex values that represent the output of an FFT,
// display those complex values.
//
void updateGraph(cvec_o arg_fft, glFourierContext_o arg_context);

#endif // GLFOURIER_H

