#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

uniform mat4 mvp;

out vec3 vNormal;

void main() {
	vNormal = normal;
	gl_Position = mvp * vec4(vertex/6.0, 1.0);
}
