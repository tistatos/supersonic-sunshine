#version 330 core

#include "cameramatrix.inc"
#include "arealight.inc"
#include "ltc.inc"

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoords;
in vec3 vTangent;
in vec3 vBitangent;

out vec4 finalColor;
uniform mat4 m;

uniform sampler2D ltcTexture;
uniform sampler2D ltcAmplitude;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;

uniform AreaLight arealights[5];

uniform float roughness;

#define PI 3.14159265359

void main() {
	vec3 eyePos = (inverse(v) * vec4(vec3(0), 1.0)).xyz;
	eyePos = inverse(v)[3].xyz;
	eyePos = cameraposition;
	vec3 pos = vPosition.xyz;
	vec3 points[4];


	vec3 V = normalize(eyePos - pos);
	vec3 N = normalize(vNormal.xyz);

	float uvRepeat = 1.0;

	mat3 NMat = (mat3(vTangent,vBitangent, N));
	vec3 normalTex = texture(normalMap, vTexCoords).rgb;
	normalTex.y = 1- normalTex.y;

	N = NMat*(2*normalTex-1);
	N = vNormal.xyz*0.0 + N*1.0;

	float theta = acos(dot(V,N));
	float roughnessTex = (1-texture(roughnessMap, vTexCoords*uvRepeat).r);
	roughnessTex = clamp(roughness*roughnessTex, 0.0, 1.0);
	vec2 uv = vec2(roughnessTex, theta/(0.5*PI));

	const float LUT_SIZE  = 64.0;
	const float LUT_SCALE = (LUT_SIZE - 1.0)/LUT_SIZE;
	const float LUT_BIAS  = 0.5/LUT_SIZE;

	uv = uv*LUT_SCALE + LUT_BIAS;

	vec4 ltc = texture(ltcTexture, uv);

	mat3 mInv = mat3(
			vec3(1.0, 0.0, ltc.y),
			vec3(0.0, ltc.z, 0.0),
			vec3(ltc.w, 0.0, ltc.x) );

	int nLights = 3;
	vec3 color = vec3(0.0);
	for(int i = 0; i < nLights; i++){
		AreaLight arealight = arealights[i];
		//area light points
		points[1] =	(arealight.M * vec4(arealight.points[1],1.0)).xyz;
		points[0] = (arealight.M * vec4(arealight.points[2],1.0)).xyz;
		points[2] =	(arealight.M * vec4(arealight.points[0],1.0)).xyz;
		points[3] =	(arealight.M * vec4(arealight.points[3],1.0)).xyz;

		vec3 spec = arealightDiffuse(N,V,pos, mInv, points);
		float specAmplitude = texture(ltcAmplitude, uv).r;
		spec *= specAmplitude;

		vec3 diffuse = arealightDiffuse(N,V,pos, mat3(1.0), points);

		vec3 albedo = texture(diffuseMap, vTexCoords).rgb;
		color += arealight.intensity *
			(	arealight.color * spec +
				albedo * diffuse * arealight.color );
	}
	color /=(2.0*PI);

	finalColor = vec4(color, 1.0);
}
