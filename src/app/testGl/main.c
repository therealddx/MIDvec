/*
 * reference LICENSE file provided.
 *
 * main.c
 * Implements test application for this library using OpenGL.
 *
 */

#include <opengl/glSetup.h>
#include <opengl/glTriangle.h>

void main()
{
  // initialize context for drawing a damn triangle.
  //
  GLFWwindow* myWindow = setupGlfwGlew("My Test");
  glTriangle_o mytri1 = new_glTriangle();
  glTriangle_o mytri2 = new_glTriangle();

  // mess with transform / scale.
  //
  configureVertShader(mytri1, 0.5f, 0.5f, 0.5f, 0.5f);
  configureVertShader(mytri2, 0.1f, 0.1f, -0.5f, -0.5f);

  // drawing cycle:
  //  clear graph.
  //  plot for each bin.
  //  move back-buffer to front.
  //
  while (!glfwWindowShouldClose(myWindow))
  {
    // drawdelegate
    //
    drawTriangle(mytri1);
    drawTriangle(mytri2);

    // winman
    //
    glfwSwapBuffers(myWindow);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
  }

  // ret.
  glfwTerminate();
  return;
}

