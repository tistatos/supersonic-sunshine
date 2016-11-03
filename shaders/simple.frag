#version 330 core
out vec4 color;

in vec3 vNormal;
in vec3 vPosition;

uniform float roughness;

#define PI 3.14159265359
// An "improved" Oren-Nayar - see http://mimosa-pudica.net/improved-oren-nayar.html
float diffuseReflection(float rough, float albedo, vec3 L, vec3 N, vec3 V){
	float NdotL = dot(N,L);
	float NdotV = dot(N,V);

	float s = dot(L,V) - (NdotL*NdotV);
	float t;
	if (s > 0.00) {
		t = max(NdotL, NdotV);
	} else {
		t = 1;
	}
	float roughsq = rough*rough;

	float A = 0.3183*(1.0 - (0.5*roughsq/(roughsq+0.33) ) + (0.17*albedo*roughsq/(roughsq+0.13)));
	float B = 0.3183*(0.45*(roughsq/(roughsq+0.09)));
	//float A = 1.0/(PI + (PI/2.0 - 2.0/3.0)*(rough+0.000001));
	//float B = (rough+0.000001)/(PI + (PI/2.0 - 2.0/3.0)*(rough+0.000001));

	return albedo * NdotL * (A+(B*(s/t)));
}

void main() {
	//hårdkodade positioner
	vec3 lightPos = vec3(6.0,10.0,0.0);
	vec3 eyePos = vec3(0.0, 1.5, 6.0);

	vec3 L = normalize(lightPos-vPosition);
	vec3 N = normalize(vNormal);
	vec3 V = normalize(eyePos - vPosition);

	float diffuse = diffuseReflection(roughness, 0.98, L, N, V);

	vec3 fixedNormal = vNormal+1.0;
	fixedNormal /= 2.0;


	color = vec4(vec3(diffuse), 1.0);
}
