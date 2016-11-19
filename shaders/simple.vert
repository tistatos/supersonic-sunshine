#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 m;

layout (std140) uniform Matrices{
    mat4 p;
    mat4 v;
};

out vec4 vNormal;
out vec4 vPosition;
out vec2 vTexCoords;

void main() {
	vNormal = transpose(inverse(m)) * vec4(normal, 1.0);
	vTexCoords = texCoords;
	vPosition = (m * vec4(vertex,1.0));
	gl_Position = p * v * m * vec4(vertex, 1.0);
}
