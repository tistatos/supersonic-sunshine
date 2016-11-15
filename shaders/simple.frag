#version 330 core
out vec4 color;

layout (std140) uniform Matrices{
    mat4 p;
    mat4 v;
};

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
in mat4 modelMatrix;

uniform sampler2D tex;

struct AreaLight{
	vec3 points[4];
	mat4 M;
	float intensity;
};

uniform AreaLight arealight;


uniform float roughness;

#define PI 3.14159265359


float IntegrateEdge(vec3 v1, vec3 v2)
{
    float cosTheta = dot(v1, v2);
    float theta = acos(cosTheta);
    float res = cross(v1, v2).z * ((theta > 0.001) ? theta/sin(theta) : 1.0);

    return res;
}

float arealightDiffuse(vec3 N, vec3 V, vec3 P, vec3 points[4]){
		// construct orthonormal basis around N
		mat3 Minv = mat3(1.0);

    vec3 T1, T2;
    T1 = normalize(V - N*dot(V, N));
    T2 = cross(N, T1);

    Minv = Minv * transpose(mat3(T1, T2, N));
		vec3 L[5];
    L[0] = Minv * (points[0] - P);
    L[1] = Minv * (points[1] - P);
    L[2] = Minv * (points[2] - P);
    L[3] = Minv * (points[3] - P);


    L[0] = normalize(L[0]);
    L[1] = normalize(L[1]);
    L[2] = normalize(L[2]);
    L[3] = normalize(L[3]);
		 // integrate
    float sum = 0.0;

    sum += IntegrateEdge(L[0], L[1]);
    sum += IntegrateEdge(L[1], L[2]);
    sum += IntegrateEdge(L[2], L[3]);
		sum += IntegrateEdge(L[3], L[0]);

		sum = max(sum, 0.0);

		return sum;
}

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

	return albedo * clamp(NdotL, 0.0, 1.0) * (A+(B*(s/t)));
}

void main() {
	//hårdkodade positioner
	vec3 lightPos = (v * vec4(0.0,1.5,6.0,1.0)).xyz;
	vec3 eyePos = (v * vec4(0.0,1.5, 6.0,1.0)).xyz;
	vec3 pos = vec4(vPosition,1.0).xyz;
	vec3 points[4];

	points[0] = (v*arealight.M * vec4(arealight.points[0],1.0)).xyz;
	points[1] =	(v*arealight.M * vec4(arealight.points[1],1.0)).xyz;
	points[2] =	(v*arealight.M * vec4(arealight.points[2],1.0)).xyz;
	points[3] =	(v*arealight.M * vec4(arealight.points[3],1.0)).xyz;

/*
	points[0] = (v*vec4(0.5, 3.0, 0.5,1.0)).xyz;
	points[1] =	(v*vec4(0.5, 3.0, -0.5,1.0)).xyz;
	points[2] =	(v*vec4(-0.5, 3.0, -0.5,1.0)).xyz;
	points[3] =	(v*vec4(-0.5, 3.0, 0.5,1.0)).xyz;
*/
	vec3 L = normalize(lightPos-pos);
	vec3 N = normalize(vNormal);
	vec3 V = normalize(eyePos - pos	);

	// float diffuse = diffuseReflection(roughness, 0.95, L, N, V);
	float diffuse = arealight.intensity * arealightDiffuse(N,V, pos, points);
	vec3 fixedNormal = vNormal+1.0;
	fixedNormal /= 2.0;

	vec4 ltc = texture(tex, vTexCoords);

	color = vec4(vec3(diffuse),1.0);
	//color = vec4(vec3(points[3].y), 1.0);

	//color = vec4(vec3(debug(N, V, L)), 1.0);
}
