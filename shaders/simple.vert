#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

uniform mat4 mvp;
uniform float scale;

out vec3 vNormal;
out vec3 vPosition;

void main() {
	vNormal = normal;
	vPosition = scale*vertex;
	gl_Position = mvp * vec4(scale*vertex, 1.0);
}
