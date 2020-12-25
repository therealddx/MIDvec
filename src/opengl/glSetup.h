/*
 * reference LICENSE file provided.
 *
 * glSetup.h
 *
 * Functions for dynamic setup / teardown of GLFW / GLEW / OpenGL.
 *
 */

#ifndef GLSETUP_H
#define GLSETUP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const int DIM_PER_VERT = 3; // x, y, z

GLFWwindow* setupGlfwGlew(const char*);

#endif // GLSETUP_H

