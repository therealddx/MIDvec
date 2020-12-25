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
  glTriangle_o myTriangle = new_glTriangle();

  // mess with coordinates.
  //
  setTriangleVertex(myTriangle, Vert1, 0.3f, 0.3f);
  setTriangleVertex(myTriangle, Vert2, 0.8f,-0.2f);
  setTriangleVertex(myTriangle, Vert3,-0.2f,-0.2f);

  // drawing cycle:
  //  clear graph.
  //  plot for each bin.
  //  move back-buffer to front.
  //
  while (!glfwWindowShouldClose(myWindow))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(myWindow);
    glfwPollEvents();
  }

  // ret.
  glfwTerminate();
  return;
}

