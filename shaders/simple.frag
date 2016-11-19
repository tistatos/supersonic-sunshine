#version 330 core
layout (std140) uniform Matrices{
    mat4 p;
    mat4 v;
};

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoords;

out vec4 finalColor;
uniform mat4 m;

uniform sampler2D tex;
uniform sampler2D ampTex;

uniform sampler2D bumpMap;

struct AreaLight{
	vec3 points[4];
	vec3 color;
	mat4 M;
	float intensity;
};

uniform AreaLight arealight;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float roughness;

#define PI 3.14159265359

void ClipQuadToHorizon(inout vec3 L[5], out int n)
{
    // detect clipping config
    int config = 0;
    if (L[0].z > 0.0) config += 1;
    if (L[1].z > 0.0) config += 2;
    if (L[2].z > 0.0) config += 4;
    if (L[3].z > 0.0) config += 8;

    // clip
    n = 0;

    if (config == 0)
    {
        // clip all
    }
    else if (config == 1) // V1 clip V2 V3 V4
    {
        n = 3;
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[3].z * L[0] + L[0].z * L[3];
    }
    else if (config == 2) // V2 clip V1 V3 V4
    {
        n = 3;
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
    }
    else if (config == 3) // V1 V2 clip V3 V4
    {
        n = 4;
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
        L[3] = -L[3].z * L[0] + L[0].z * L[3];
    }
    else if (config == 4) // V3 clip V1 V2 V4
    {
        n = 3;
        L[0] = -L[3].z * L[2] + L[2].z * L[3];
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
    }
    else if (config == 5) // V1 V3 clip V2 V4) impossible
    {
        n = 0;
    }
    else if (config == 6) // V2 V3 clip V1 V4
    {
        n = 4;
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
    }
    else if (config == 7) // V1 V2 V3 clip V4
    {
        n = 5;
        L[4] = -L[3].z * L[0] + L[0].z * L[3];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
    }
    else if (config == 8) // V4 clip V1 V2 V3
    {
        n = 3;
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
        L[1] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] =  L[3];
    }
    else if (config == 9) // V1 V4 clip V2 V3
    {
        n = 4;
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[2].z * L[3] + L[3].z * L[2];
    }
    else if (config == 10) // V2 V4 clip V1 V3) impossible
    {
        n = 0;
    }
    else if (config == 11) // V1 V2 V4 clip V3
    {
        n = 5;
        L[4] = L[3];
        L[3] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
    }
    else if (config == 12) // V3 V4 clip V1 V2
    {
        n = 4;
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
    }
    else if (config == 13) // V1 V3 V4 clip V2
    {
        n = 5;
        L[4] = L[3];
        L[3] = L[2];
        L[2] = -L[1].z * L[2] + L[2].z * L[1];
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
    }
    else if (config == 14) // V2 V3 V4 clip V1
    {
        n = 5;
        L[4] = -L[0].z * L[3] + L[3].z * L[0];
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
    }
    else if (config == 15) // V1 V2 V3 V4
    {
        n = 4;
    }

    if (n == 3)
        L[3] = L[0];
    if (n == 4)
        L[4] = L[0];
}

float IntegrateEdge(vec3 v1, vec3 v2) {
	float cosTheta = dot(v1, v2);
	float theta = acos(cosTheta);
	float res = cross(v1, v2).z * ((theta > 0.001) ? theta/sin(theta) : 1.0);

	return res;
}

vec3 arealightDiffuse(vec3 N, vec3 V, vec3 P, mat3 mInv, vec3 points[4]) {
	// construct orthonormal basis around N
	mat3 Minv = mInv;

	vec3 T1, T2;
	T1 = normalize(V - N*dot(V, N));
	T2 = cross(N, T1);

	Minv = Minv * transpose(mat3(T1, T2, N));

	vec3 L[5];
	L[0] = Minv * (points[0] - P);
	L[1] = Minv * (points[1] - P);
	L[2] = Minv * (points[2] - P);
	L[3] = Minv * (points[3] - P);

	int n;
	ClipQuadToHorizon(L, n);

	if (n == 0)
			return vec3(0, 0, 0);

    // project onto sphere
    L[0] = normalize(L[0]);
    L[1] = normalize(L[1]);
    L[2] = normalize(L[2]);
    L[3] = normalize(L[3]);
    L[4] = normalize(L[4]);

	L[0] = normalize(L[0]);
	L[1] = normalize(L[1]);
	L[2] = normalize(L[2]);
	L[3] = normalize(L[3]);
	 // integrate
	float sum = 0.0;

	sum += IntegrateEdge(L[0], L[1]);
	sum += IntegrateEdge(L[1], L[2]);
	sum += IntegrateEdge(L[2], L[3]);
	if (n >= 4)
        sum += IntegrateEdge(L[3], L[4]);
    if (n == 5)
        sum += IntegrateEdge(L[4], L[0]);
	sum = max(sum, 0.0);

	return vec3(sum);
}

void main() {
	vec3 eyePos = (inverse(v) * vec4(vec3(0), 1.0)).xyz;
	vec3 pos = vPosition.xyz;
	vec3 points[4];

	points[1] =	(arealight.M * vec4(arealight.points[1],1.0)).xyz;
	points[2] = (arealight.M * vec4(arealight.points[0],1.0)).xyz;
	points[0] =	(arealight.M * vec4(arealight.points[2],1.0)).xyz;
	points[3] =	(arealight.M * vec4(arealight.points[3],1.0)).xyz;

	vec3 N = normalize(vNormal.xyz);
	vec3 V = normalize(eyePos - pos	);

	float theta = acos(dot(V,N));
	vec2 uv = vec2(roughness, theta/(0.5*PI));

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

	vec3 color = vec3(arealight.intensity)*(specularColor * arealight.color * spec + diffuseColor *
			arealight.color  * diffuse);

	color /=(2.0*PI);

	finalColor = vec4(color,1.0);
}
