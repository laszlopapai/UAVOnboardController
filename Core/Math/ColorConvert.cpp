#include <algorithm>
#include "ColorConvert.h"
#include <math.h>

using namespace IoT::Core;

void ColorConvert::hsv2rgb(float H, float S, float V, float *r, float *g, float *b)
{
	H -= ((uint64_t)(H / 360) * 360.0f);

	float R, G, B;
	if (V <= 0)
	{
		R = G = B = 0;
	}
	else if (S <= 0)
	{
		R = G = B = V;
	}
	else
	{
		float hf = fabsf(H) / 60.0f;
		int i = (int)hf;
		float f = hf - i;
		float pv = V * (1 - S);
		float qv = V * (1 - S * f);
		float tv = V * (1 - S * (1 - f));

		// Red is the dominant color
		if (i == 0) {
			R = V;
			G = tv;
			B = pv;
		}
		if (i == 5) {
			R = V;
			G = pv;
			B = qv;
		}

		// Green is the dominant color
		if (i == 1) {
			R = qv;
			G = V;
			B = pv;
		}
		if (i == 2) {
			R = pv;
			G = V;
			B = tv;
		}

		// Blue is the dominant color
		if (i == 3) {
			R = pv;
			G = qv;
			B = V;
		}
		if (i == 4) {
			R = tv;
			G = pv;
			B = V;
		}
	}

	*r = std::max(std::min(R, 1.0f), 0.0f);
	*g = std::max(std::min(G, 1.0f), 0.0f);
	*b = std::max(std::min(B, 1.0f), 0.0f);
}

void ColorConvert::rgb2hsv(float R, float G, float B, float *h, float *s, float *v) {

	float cMax = std::max(std::max(R, G), B);
	float cMin = std::min(std::min(R, G), B);

	float d = cMax - cMin;

	// TODO: rgb2hsv converter
}
