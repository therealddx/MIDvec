// 
// reference LICENSE file provided.
// 
// neutVert.glsl
// super-simple ('neutral') vertex shader file.
// 

#version 330 core

layout (location = 0) in vec3 arg_pos;

void main()
{
  gl_Position = vec4(arg_pos, 1.0f);
}

