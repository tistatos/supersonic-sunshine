/**
* @file ltcMaps.h
* @author Erik Sandrén
* @date 15-11-2016
* @brief Linear transformed cosine texture maps
*/

#ifndef __LTCMAPS_H__
#define __LTCMAPS_H__

#include <GLFW/glfw3.h>
#include "ltc.h"

typedef struct LTC {
	GLuint material;
	GLuint amplitude;

	GLuint materialIndex = -1;
	GLuint amplitudeIndex = -1;
} LTC_t;

LTC_t loadLTCTextures() {
	LTC_t maps;
	glGenTextures(1, &maps.material);
	glGenTextures(1, &maps.amplitude);

	glBindTexture(GL_TEXTURE_2D, maps.material);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size, size, 0, GL_RGBA, GL_FLOAT, (void*)&invM);

	glBindTexture(GL_TEXTURE_2D, maps.amplitude);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size, size, 0, GL_RED, GL_FLOAT, (void*)&tabAmplitude);

	return maps;
}

void bindTextures(LTC_t* maps) {
	//GLint activeTexture;
	//glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
	//maps->materialIndex = activeTexture;
	//glBindTexture(GL_TEXTURE_2D, maps->material);
	//glActiveTexture(++activeTexture);
	//maps->amplitudeIndex = activeTexture;
	//glBindTexture(GL_TEXTURE_2D, maps->amplitude);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, maps->amplitude);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, maps->material);
}
#endif
