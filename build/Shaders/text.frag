#version 450

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

// uniform float time;
uniform sampler2D imageTexture;

void main() {	
	color = texture(imageTexture, fragmentUV) * fragmentColor;
  color.a = color.r;
  color.r = 1;
  color.g = 1;
  color.b = 1;
}