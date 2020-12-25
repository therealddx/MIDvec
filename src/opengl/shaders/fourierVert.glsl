// 
// reference LICENSE file provided.
// 
// fourierVert.glsl
// 
// given an argument position, report that vertex position.
// 

#version 330 core
layout (location = 0) in vec3 arg_pos;

// axis bounds for application space.
// valid for all vertices that get put into a Fourier plot.
// 
uniform float u_xMin; // x-axis: discrete-frequency sample 'k'.
uniform float u_xMax;
uniform float u_yMin; // y-axis: cmag for Fourier value to plot.
uniform float u_yMax;

void main()
{
  // mapping routine from application space coordinates to NDC.
  // 
  // (arg_pos.x - u_xMin) / (u_xMax - u_xMin) : [0, 1]
  // * (2.0) - 1.0 : [-1.0, 1.0]
  // 

  // vec3 rtn_pos = vec3
  //   ( (arg_pos.x - u_xMin) / (u_xMax - u_xMin) * 2.0 - 1.0
  //   , (arg_pos.y - u_yMin) / (u_yMax - u_yMin) * 2.0 - 1.0
  //   , 0.0f
  //   );
  // 
  // gl_Position = vec4(rtn_pos, 1.0f);

  gl_Position = vec4(arg_pos, 1.0f);
}

