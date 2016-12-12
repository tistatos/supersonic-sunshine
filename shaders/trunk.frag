#version 330 core

in vec2 vTexCoords;
uniform sampler2D texUnit;
out vec4 finalColor;

void main() {
	vec4 c = texture(texUnit, vTexCoords);
	finalColor.r = max(c.r - 1.0, 0);
	finalColor.g = max(c.g - 1.0, 0);
	finalColor.b = max(c.b - 1.0, 0);
	finalColor.a = 1.0;
}
