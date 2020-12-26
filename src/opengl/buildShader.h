/*
 * reference LICENSE file provided.
 *
 * buildShader.h
 *
 * Encapsulated functions for creating shader programs within the 
 * OpenGL space.
 *
 */

#ifndef BUILDSHADER_H
#define BUILDSHADER_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include <GL/glew.h>

// 
// compileShader.
//
// Open 'arg_glslFile', and use its GLSL source to compile a shader 
// of type 'arg_shaderType'.
//
// Store its source code in 'rtn_shaderSourceBuffer' and return its 
// ID in the OpenGL space.
//
GLuint compileShader
  ( const char* arg_glslFile
  , GLenum arg_shaderType
  , char** rtn_shaderSourceBuffer
  );

// 
// linkShaders.
//
// Given a pointer to an array of shader IDs 'arg_allShaders', seek 
// its first 'arg_numShaders' shaders and link them into a shader program.
//
// Return the shader program's ID in the OpenGL space. If 'arg_discard' is 
// set to GL_TRUE, discard the individual shader IDs from the OpenGL space.
//
GLuint linkShaders
  ( GLuint* arg_allShaders
  , GLuint arg_numShaders
  , GLboolean arg_discard
  );

// 
// bpShader.
//
// Blueprint for a shader of a given type '_shaderType' whose source is 
// stored at path '_glslFile'.
//
typedef struct bpShader
{
  const char* _glslFile;
  GLenum _shaderType;
} bpShader_o;

bpShader_o new_bpShader(const char*, GLenum);

// 
// buildShader.
//
// Given an array of bpShader_o structs 'arg_bpShaders' with length
// 'arg_numShaders', compile and link their GLSL source files into a
// shader program.
//
// Return the shader program's ID in the OpenGL space. Discard individual
// shader IDs, and dispose of all memory not immediately necessary for
// the shader program.
//
GLuint buildShader(bpShader_o* arg_bpShaders, GLuint arg_numBpShaders);

#endif // BUILDSHADER_H

