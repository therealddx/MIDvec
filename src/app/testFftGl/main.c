/*
 * reference LICENSE file provided.
 *
 * main.c
 * Implements test application for this library using OpenGL.
 *
 */

#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

#include <math/libMath.h>
#include <math/vec.h>
#include <dsp/dspFourier.h>
#include <opengl/glSetup.h>
#include <opengl/glTriangle.h>
#include <opengl/glCartesian.h>

// goal: make some sine-wave and plot its FFT on continuous-time
// basis in a graph driven by OpenGL.
// 
void main()
{
  // initialize context for drawing a damn triangle.
  //
  GLFWwindow* myWindow = setupGlfwGlew("FFT OpenGL Test");

  // vars.
  //
  int32_t FFT_LEN = 128;
  int32_t _j = 0;
  cvec_o testSignal = new_cvec(FFT_LEN);

  // make cartesian plot. set window for FFT.
  //
  glCartesian_o mycart = new_glCartesian();
  set_glCartesian(mycart, 0.0f, (float)FFT_LEN, 0.0f, 1.0f);

  // drawing cycle.
  //
  while (!glfwWindowShouldClose(myWindow))
  {
    // 'obtain' a 'noisy' test signal and find its fft.
    //
    for (_j = 0; _j < testSignal.len; _j++)
    {
      testSignal.arr[_j] = sin(0.5 * (double)_j) + 
        (double)rand() / (double)RAND_MAX * 0.2 ;
    }
    cvec_o testSignal_fft = fft(testSignal);
  
    // plot the FFT on the cartesian graph.
    //
    for (_j = 0; _j < mycart._len; _j++)
    {
      set_glCartesianPoint(
        mycart._points[_j], (float)_j, cmag(testSignal_fft.arr[_j]));
    }

    // draw delegate.
    //
    draw_glCartesian(mycart);

    // window management functions.
    //
    glfwSwapBuffers(myWindow);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);

    // garbage management functions.
    //
    del_cvec(testSignal_fft);
  }

  // teardown.
  // 
  del_cvec(testSignal);
  del_glCartesian(mycart);

  // ret.
  //
  glfwTerminate();
  return;
}

