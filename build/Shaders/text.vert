#version 450

attribute vec4 coord;
varying vec2 texcoord;

uniform mat4 Projection;

void main(void) {
  gl_Position = Projection * vec4(coord.xy, 0, 1);
  texcoord = coord.zw;
}