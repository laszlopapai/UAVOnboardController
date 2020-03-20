#pragma once

#include <cstdint>

namespace IoT {
	namespace Core {

		class ColorConvert {
		public:
			static void hsv2rgb(float H, float S, float V, float *r, float *g, float *b);
			static void rgb2hsv(float R, float G, float B, float *h, float *s, float *v);
		};
	}
}
