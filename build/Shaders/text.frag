#version 450

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

// uniform float time;
uniform sampler2D imageTexture;
uniform vec4 fontColor;

void main() {	
	color = texture(imageTexture, fragmentUV) * fragmentColor;
  color.a = color.r;
  color.r = fontColor[0];
  color.g = fontColor[1];
  color.b = fontColor[2];
  color.a *= fontColor[3];
}