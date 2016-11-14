#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 m;

layout (std140) uniform Matrices{
    mat4 p;
    mat4 v;
};

out vec3 vNormal;
out vec3 vPosition;
out vec2 vTexCoords;

void main() {
	vNormal = inverse(transpose(mat3(v*m))) * normal;
	vTexCoords = texCoords;
	vPosition = (v * m * vec4(vertex,1.0)).xyz;
	gl_Position = p * v * m * vec4(vertex, 1.0);
}
