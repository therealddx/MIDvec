/*
 * reference LICENSE file provided.
 *
 * glFourier.h.
 *
 * Provide a context for displaying frequency-domain information.
 *
 */

#include <sys/types.h>
#include <complex.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct glFourierContext
{
  GLFWwindow* _window;
  GLuint _numBins;
  GLuint* _binsVbo; // one bin => one drawn line => one VBO.
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
void updateGraph(cvec_o arg_fft);

