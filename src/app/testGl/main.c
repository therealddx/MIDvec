/*
 * reference LICENSE file provided.
 *
 * main.c
 * Implements test application for this library using OpenGL.
 *
 */

#include <sys/types.h>

#include <opengl/glSetup.h>
#include <opengl/glTriangle.h>

void main()
{
  // initialize context for drawing a damn triangle.
  //
  GLFWwindow* myWindow = setupGlfwGlew("My Test");

  // goal: make a 3-by-3 grid of differently-scaled triangles.
  //
  u_int32_t NUM_TRIS = 9;
  u_int32_t DIM_TRIS = 3;
  glTriangle_o* mytris = malloc(NUM_TRIS * sizeof(glTriangle_o));
  
  u_int32_t _n = 0;
  for (_n = 0; _n < NUM_TRIS; _n++)
  {
    mytris[_n] = new_glTriangle();

    u_int32_t rowInd = _n / DIM_TRIS;
    u_int32_t colInd = _n % DIM_TRIS;
    float scaleKey = (float)(rowInd + 1) + (float)(colInd + 1);

    float xDelta = -0.5f + (float)colInd * 0.5f;
    float yDelta = 0.5f - (float)rowInd * 0.5f;
    float xScale = scaleKey * 0.02f;
    float yScale = scaleKey * 0.05f;

    configureVertShader(mytris[_n], xScale, yScale, xDelta, yDelta);
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
    for (_n = 0; _n < NUM_TRIS; _n++)
    {
      drawTriangle(mytris[_n]);
    }

    // window management functions.
    //
    glfwSwapBuffers(myWindow);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
  }

  // teardown.
  //
  for (_n = 0; _n < NUM_TRIS; _n++)
  {
    del_triangle(mytris[_n]);
  }
  free(mytris);

  // ret.
  //
  glfwTerminate();
  return;
}

