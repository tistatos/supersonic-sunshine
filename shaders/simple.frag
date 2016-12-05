#version 330 core

#include "cameramatrix.inc"
#include "arealight.inc"
#include "ltc.inc"

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoords;

out vec4 finalColor;
uniform mat4 m;

uniform sampler2D tex;
uniform sampler2D ampTex;
uniform sampler2D bumpMap;
uniform sampler2D roughnessMap;

uniform AreaLight arealight;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float roughness;

#define PI 3.14159265359


void main() {
	vec3 eyePos = (inverse(v) * vec4(vec3(0), 1.0)).xyz;
	vec3 pos = vPosition.xyz;
	vec3 points[4];

	points[1] =	(arealight.M * vec4(arealight.points[1],1.0)).xyz;
	points[2] = (arealight.M * vec4(arealight.points[0],1.0)).xyz;
	points[0] =	(arealight.M * vec4(arealight.points[2],1.0)).xyz;
	points[3] =	(arealight.M * vec4(arealight.points[3],1.0)).xyz;


	vec3 V = normalize(eyePos - pos	);
	vec3 N = normalize(vNormal.xyz);


	vec3 T1, T2;
	T1 = vec3(1.0,0.0,0.0);
	T2 = vec3(0.0,0.0,1.0);

	float uvRepeat = 0.4;

	mat3 NMat = transpose(mat3(T1, T2, N));
	N = NMat*(2*texture(bumpMap, vTexCoords*uvRepeat).rgb-1);
	N = vNormal.xyz*0.8 + N*0.2;

	float theta = acos(dot(V,N));
	float roughnessTex = roughness * texture(roughnessMap, vTexCoords*uvRepeat).r;
	roughnessTex = clamp(roughnessTex, 0.0, 1.0);
	vec2 uv = vec2(roughnessTex, theta/(0.5*PI));

	const float LUT_SIZE  = 64.0;
	const float LUT_SCALE = (LUT_SIZE - 1.0)/LUT_SIZE;
	const float LUT_BIAS  = 0.5/LUT_SIZE;

	uv = uv*LUT_SCALE + LUT_BIAS;

	vec4 ltc = texture(tex, uv);

	mat3 mInv = mat3(
			vec3(1.0, 0.0, ltc.y),
			vec3(0.0, ltc.z, 0.0),
			vec3(ltc.w, 0.0, ltc.x) );

	vec3 spec = arealightDiffuse(N,V,pos, mInv, points);
	float specAmplitude = texture(ampTex, uv).r;
	spec *= specAmplitude;

	vec3 diffuse = arealightDiffuse(N,V,pos, mat3(1.0), points);

	vec3 color = vec3(arealight.intensity/2)*(specularColor * arealight.color * spec + diffuseColor *
			arealight.color  * diffuse);

	color /=(2.0*PI);

	finalColor = vec4(color,1.0);
}
