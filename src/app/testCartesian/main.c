/*
 * reference LICENSE file provided.
 *
 * main.c
 * Implements test application for this library using OpenGL.
 *
 */

#include <sys/types.h>
#include <math.h>

#include <opengl/glSetup.h>
#include <opengl/glTriangle.h>
#include <opengl/glCartesian.h>

void main()
{
  // initialize context for drawing a damn triangle.
  //
  GLFWwindow* myWindow = setupGlfwGlew("My Test");

  // goal: make some sine-wave in a cartesian to show you aren't a 
  // nincompoop
  //
  glCartesian_o mycart = new_glCartesian();

  u_int32_t _n = 0;
  for (_n = 0; _n < mycart._len; _n++)
  {
    float z = (float)_n / (float)mycart._len * 2.0f - 1.0f;
    set_glCartesianPoint(mycart._points[_n], z, sin(0.01 * _n));
  }

  // drawing cycle:
  //  clear graph.
  //  plot for each bin.
  //  move back-buffer to front.
  //
  while (!glfwWindowShouldClose(myWindow))
  {
    // draw delegate.
    //
    draw_glCartesian(mycart);

    // window management functions.
    //
    glfwSwapBuffers(myWindow);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
  }

  // teardown.
  //
  del_glCartesian(mycart);

  // ret.
  //
  glfwTerminate();
  return;
}

