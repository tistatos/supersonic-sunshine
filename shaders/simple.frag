#version 330 core
out vec4 color;

in vec3 vNormal;
void main() {
	color = vec4(vNormal, 1.0); 
}
