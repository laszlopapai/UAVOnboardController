#pragma once

namespace IoT {
	namespace Device {

/// Continuous 2nd order complementer filter
		class C2ComplementerFilter
		{

		public:
			explicit C2ComplementerFilter(float k);

			float &getFiltered() {
				return m_filtered;
			}

			void filter(float &input1, float &input2, float dt);

		private:
			const float m_k;
			float m_filtered;
			float m_integratorY1;
		};
	}
}
