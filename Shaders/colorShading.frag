#version 460

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

uniform float time;

void main() {
	color = vec4(fragmentColor.r * (cos(fragmentPosition.x * 4 + time) + 1) * .5,
			     fragmentColor.g * (cos(fragmentPosition.y * 8 + time) + 1) * .5,
				 fragmentColor.b * (cos(fragmentPosition.x * 2 + time) + 1) * .5,
				 fragmentColor.a);
}