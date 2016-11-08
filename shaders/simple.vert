#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

uniform mat4 mv;
uniform mat4 mvp;
uniform mat3 normalMatrix;


out vec3 vNormal;
out vec3 vPosition;

void main() {
	vNormal = normalMatrix * normal;
	vPosition = (mv*vec4(vertex,1.0)).xyz;
	gl_Position = mvp * vec4(vertex, 1.0);
}
