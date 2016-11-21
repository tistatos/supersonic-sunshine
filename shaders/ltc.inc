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

	vec3 L[4];
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

	return vec3(sum);
}

