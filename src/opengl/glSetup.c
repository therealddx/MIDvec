/*
 * reference LICENSE file provided.
 *
 * glSetup.c
 *
 * Implements glSetup.h.
 *
 */

#include "glSetup.h"

GLFWwindow* setupGlfwGlew(const char* arg_title)
{
  // setup GLFW.
  //
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* rtn_window = glfwCreateWindow(
    800, 600, arg_title, NULL, NULL);
  glfwMakeContextCurrent(rtn_window);

  // setup GLEW.
  //
  glewInit();

  return rtn_window;
}

