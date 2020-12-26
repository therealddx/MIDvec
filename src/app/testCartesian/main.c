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
  float xMin = -1.0f;
  float xMax =  1.0f;
  float yMin = -1.0f;
  float yMax =  1.0f;

  u_int32_t _n = 0;
  for (_n = 0; _n < mycart._len; _n++)
  {
    float x = (float)_n / ((float)mycart._len - 1.0f) * 2.0f - 1.0f;
    set_glCartesianPoint(mycart._points[_n], x, sin(M_PI * x));
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

    // prep for next draw. (zoom into upper-right quadrant).
    //
    xMin += 0.005f;
    yMin += 0.005f;
    usleep(100000);
    set_glCartesian(mycart, xMin, xMax, yMin, yMax);
  }

  // teardown.
  //
  del_glCartesian(mycart);

  // ret.
  //
  glfwTerminate();
  return;
}

