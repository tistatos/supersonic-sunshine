#version 330 core

out vec4 color;
uniform vec4 lightColor;

void main() {
	color = lightColor;
	color = vec4(1.0);
}
