#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

uniform mat4 mv;
uniform mat4 m;
uniform mat4 mvp;
uniform mat3 normalMatrix;

layout (std140) uniform Matrices{
    mat4 p;
    mat4 v;
};

out vec3 vNormal;
out vec3 vPosition;

void main() {
	vNormal = normalMatrix * normal;
	vPosition = (mv*vec4(vertex,1.0)).xyz;
	gl_Position = p * v * m * vec4(vertex, 1.0);
}
