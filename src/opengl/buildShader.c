/*
 * buildShader.c
 *
 * Implements buildShader.h.
 *
 */

#include "buildShader.h"

// compileShader.
//  arg_glslFile: path to *.glsl file.
//  arg_shaderType: e.g. GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.
//
GLuint compileShader
  ( const char* arg_glslFile
  , GLenum arg_shaderType
  , char** rtn_shaderSourceBuffer
  )
{
  // vars.
  GLuint rtn_shaderGLID;

  // open file handle to 'arg_glslFile'.
  //
  FILE* f_readShaderSource = fopen(arg_glslFile, "rb");
  if (f_readShaderSource == NULL)
  {
    printf("compileShader: bad file\n");
    assert(0);
  }

  // read bytes from 'arg_glslFile' (max 1KB).
  //
  size_t maxSourceBytes = 1024;
  char* shaderSourceBuffer = (char*)malloc(sizeof(char) * maxSourceBytes);
  size_t rBytes = fread
    ( shaderSourceBuffer
    , sizeof(char)
    , maxSourceBytes
    , f_readShaderSource
    );
  fclose(f_readShaderSource);

  if (rBytes < 0)
  {
    printf("compileShader: bad fread\n");
    assert(0);
  }
  shaderSourceBuffer[rBytes] = '\0';

  // initialize shader:
  //  create shader of given type.
  //  attach source code to shader.
  //  compile shader GLSL code.
  //  report compilation success / failure.
  //
  rtn_shaderGLID = glCreateShader(arg_shaderType);
  glShaderSource(
    rtn_shaderGLID, 1, (const char * const *)(&shaderSourceBuffer), NULL);
  glCompileShader(rtn_shaderGLID);

  GLint compileSuccess;
  glGetShaderiv(rtn_shaderGLID, GL_COMPILE_STATUS, &compileSuccess);
  if (!compileSuccess)
  {
    char errorMsg[512];
    glGetShaderInfoLog(rtn_shaderGLID, 512, NULL, errorMsg);
    printf("compileShader: error: GLID %d: %s\n", rtn_shaderGLID, errorMsg);
    printf("compileShader: shader source:\n%s\n", shaderSourceBuffer);
    assert(0);
  }

  // ret.
  //  pass off malloc'd memory to rtn_shaderSourceBuffer; free later.
  //  return shader id.
  //
  *rtn_shaderSourceBuffer = shaderSourceBuffer;
  return rtn_shaderGLID; 
}

GLuint linkShaders
  ( GLuint* arg_allShaders
  , GLuint arg_numShaders
  , GLboolean arg_discard
  )
{
  // vars.
  // 
  GLuint rtn_shaderProgram = glCreateProgram();
  GLuint _n = 0;

  // attach all shaders to this program.
  //
  for (_n = 0; _n < arg_numShaders; _n++)
  {
    glAttachShader(rtn_shaderProgram, arg_allShaders[_n]);
  }

  // link the program together.
  //
  glLinkProgram(rtn_shaderProgram);
 
  // optionally, discard no-longer-relevant shaders. 
  //
  if (arg_discard == GL_TRUE)
  {
    for (_n = 0; _n < arg_numShaders; _n++)
    {
      glDeleteShader(arg_allShaders[_n]);
    }
  }

  // ret.
  return rtn_shaderProgram;
}

bpShader_o new_bpShader(const char* arg_glslFile, GLenum arg_shaderType)
{
  bpShader_o toReturn;

  toReturn._glslFile = arg_glslFile;
  toReturn._shaderType = arg_shaderType;

  return toReturn;
}

GLuint buildShader(bpShader_o* arg_bpShaders, GLuint arg_numBpShaders)
{
  // vars.
  //
  GLuint _n = 0;
  bpShader_o* cur_bpShader = arg_bpShaders;
  char* shaderSources[arg_numBpShaders];
  GLuint shaderIds[arg_numBpShaders];
  GLuint rtn_shaderProgram = 0;

  // loop: compileShader.
  //
  for (_n = 0; _n < arg_numBpShaders; _n++)
  {
    shaderIds[_n] = compileShader
      ( cur_bpShader->_glslFile
      , cur_bpShader->_shaderType
      , &shaderSources[_n]
      );

    cur_bpShader++;
  }

  // link shaders.
  // 
  rtn_shaderProgram = linkShaders
    ( shaderIds
    , arg_numBpShaders
    , GL_TRUE
    );

  // discard sources.
  //
  for (_n = 0; _n < arg_numBpShaders; _n++)
  {
    free(shaderSources[_n]);
  }

  // ret.
  return rtn_shaderProgram;
}

