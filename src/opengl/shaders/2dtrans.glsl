// 
// reference LICENSE file provided.
// 
// shiftScale.glsl
// 
// Vertex shader file for providing arbitrary shift/scale in 2d space.
// 
// Transformation is controlled via uniforms, and this shader generates
// the corresponding matrices / operations on the input vertex position.
// 

#version 330 core

layout (location = 0) in vec3 arg_pos;

uniform float u_Sx = 1.0f;
uniform float u_Sy = 1.0f;
uniform float u_Tx = 0.0f;
uniform float u_Ty = 0.0f;

void main()
{
  // here's your shift-scale matrix.
  // 
  // [ Sx,  0,  0, Tx ] * [   x ] = [ Sx * x + Tx ]
  // [  0, Sy,  0, Ty ]   [   y ]   [ Sy * y + Ty ]
  // [  0,  0, Sz, Tz ]   [   z ]   [ Sz * z + Tz ]
  // [  0,  0,  0,  1 ]   [ 1.0 ]   [           1 ]

  // hard-code the z-axis to zero; 2d graphics here.
  // 
  float Sz = 0.0f;
  float Tz = 0.0f;

  // make matrix.
  // 
  mat4 mat_shiftScale = mat4
    ( vec4(u_Sx, 0.0f, 0.0f, 0.0f)
    , vec4(0.0f, u_Sy, 0.0f, 0.0f)
    , vec4(0.0f, 0.0f,   Sz, 0.0f)
    , vec4(u_Tx, u_Ty,   Tz, 1.0f)
    );

  // execute matrix.
  // 
  gl_Position = mat_shiftScale * vec4(arg_pos, 1.0f);
}

