#version 330 core
out vec4 color;

in vec3 vNormal;
void main() {
	vec3 fixedNormal = vNormal+1.0;
	fixedNormal /= 2.0;
	color = vec4(fixedNormal, 1.0);
}
