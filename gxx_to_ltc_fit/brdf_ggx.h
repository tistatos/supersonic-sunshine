#ifndef _BRDF_GGX_
#define _BRDF_GGX_

#include "brdf.h"

class BrdfGGX : public Brdf
{
public:

	virtual float eval(const vec3& V, const vec3& L, const float alpha, float& pdf) const
	{
		if(V.z <= 0)
		{
			pdf = 0;
			return 0;
		}

		// masking
		const float a_V = 1.0f / alpha / tanf(acosf(V.z));
		const float LambdaV = (V.z<1.0f) ? 0.5f * (-1.0f + sqrtf(1.0f + 1.0f/a_V/a_V)) : 0.0f;
		const float G1 = 1.0f / (1.0f + LambdaV);

		// shadowing
		float G2;
		if(L.z <= 0.0f)
			G2 = 0;
		else
		{
			const float a_L = 1.0f / alpha / tanf(acosf(L.z));
			const float LambdaL = (L.z<1.0f) ? 0.5f * (-1.0f + sqrtf(1.0f + 1.0f/a_L/a_L)) : 0.0f;
			G2 = 1.0f / (1.0f + LambdaV + LambdaL);
		}

		// D
		const vec3 H = normalize(V+L);
		const float slopex = H.x/H.z;
		const float slopey = H.y/H.z;
		float D = 1.0f / (1.0f + (slopex*slopex+slopey*slopey)/alpha/alpha);
		D = D*D;
		D = D / (3.14159f * alpha * alpha * H.z*H.z*H.z*H.z);

		pdf = fabsf(D * H.z / 4.0f / dot(V,H));
		float res = D * G2 / 4.0f / V.z;

		return res;
	}

	virtual vec3 sample(const vec3& V, const float alpha, const float U1, const float U2) const
	{
		const float phi = 2.0f*3.14159f * U1;
		const float r = alpha*sqrtf(U2/(1.0f-U2));
		const vec3 N = normalize(vec3(r*cosf(phi), r*sinf(phi), 1.0f));
		const vec3 L = -V + 2.0f * N * dot(N, V);
		return L;
	}

};


#endif
