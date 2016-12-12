#version 330 core

in vec2 vTexCoords;
uniform sampler2D texUnit;
uniform sampler2D secondTexUnit;
out vec4 finalColor;

void main() {
	vec4 a = texture(texUnit, vTexCoords);
	vec4 b = texture(secondTexUnit, vTexCoords);

	finalColor = a+b;
	/*finalColor = a;*/
}
