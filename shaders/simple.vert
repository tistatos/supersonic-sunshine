#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 bitangent;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec2 texCoords;

uniform mat4 m;

#include "cameramatrix.inc"

out vec4 vNormal;
out vec4 vPosition;
out vec2 vTexCoords;
out vec3 vTangent;
out vec3 vBitangent;

void main() {
	vNormal = transpose(inverse(m)) * vec4(normal, 1.0);
	vTangent = (transpose(inverse(m)) * vec4(tangent, 1.0)).xyz;
	vBitangent = (transpose(inverse(m)) * vec4(bitangent, 1.0)).xyz;

	vTexCoords = texCoords;
	vPosition = (m * vec4(vertex,1.0));
	gl_Position = p * v * m * vec4(vertex, 1.0);
}
