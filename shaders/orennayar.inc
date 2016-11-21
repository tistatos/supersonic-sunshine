// An "improved" Oren-Nayar - see http://mimosa-pudica.net/improved-oren-nayar.html
float diffuseReflection(float rough, float albedo, vec3 L, vec3 N, vec3 V) {
	float NdotL = dot(N,L);
	float NdotV = dot(N,V);

	float s = dot(L,V) - (NdotL*NdotV);
	float t;
	if (s > 0.00) {
		t = max(NdotL, NdotV);
	}
	else {
		t = 1;
	}
	float roughsq = rough*rough;

	float A = 0.3183*(1.0 - (0.5*roughsq/(roughsq+0.33) ) + (0.17*albedo*roughsq/(roughsq+0.13)));
	float B = 0.3183*(0.45*(roughsq/(roughsq+0.09)));
	//float A = 1.0/(PI + (PI/2.0 - 2.0/3.0)*(rough+0.000001));
	//float B = (rough+0.000001)/(PI + (PI/2.0 - 2.0/3.0)*(rough+0.000001));

	return albedo * clamp(NdotL, 0.0, 1.0) * (A+(B*(s/t)));
}
