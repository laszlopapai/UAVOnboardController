#include <Math/Vector3.h>
#include <iostream>
#include "C2ComplementerFilter.h"


using namespace IoT::Device;

C2ComplementerFilter::C2ComplementerFilter(const float k)
	: m_k(k)
	, m_filtered(0)
	, m_integratorY1(0)
{}

void C2ComplementerFilter::filter(float &input1, float &input2, const float dt) {
	const float integratorX1 = (input1 - m_filtered) * m_k * m_k;
	m_integratorY1 = dt * integratorX1 + m_integratorY1;
	const float integratorX2 = (input1 - m_filtered) * 2 * m_k + m_integratorY1 + input2;
	m_filtered = dt * integratorX2 + m_filtered;
}