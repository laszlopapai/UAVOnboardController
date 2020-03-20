#include "Pilot.h"
#include "Math/AngleConvert.h"

using namespace IoT::UAV;
using namespace IoT::Core;

Pilot::Pilot()
	: m_angularPositionStableScale(Vector3(10, 10, 5))
	, m_angularPositionStablePID(1, 0, 0, 0)

	, m_angularPositionRateScale(Vector3::One * 1.0f / 720)
	, m_angularPositionRatePID(1, .25, 0, 45)

	, m_angularPositionControlPID([this](auto& dataInfo) { this->angularPositionPID(dataInfo); })
	, m_angularVelocitySetpointScale([this](auto& dataInfo) { this->angularVelocitySetpointScale(dataInfo); })
	, m_angularVelocityControlPID([this](auto& dataInfo) { this->angularVelocityControlPID(dataInfo); })
	, m_angularVelocityControlScale([this](auto& dataInfo) { this->angularVelocityControlScale(dataInfo); })
{
}

bool Pilot::initialize()
{
	m_angularPosition | m_angularPositionControlPID;
	m_remoteIn | m_angularPositionControlPID | m_angularVelocitySetpointScale;

	m_angularVelocitySetpointScale | m_angularVelocityControlPID;
	m_angularVelocity | m_angularVelocityControlPID | m_angularVelocityControlScale;

	return true;
}

void Pilot::angularPositionPID(DataInfo<Vector3>& dataInfo)
{
	auto lastAngularPosition = m_angularPosition.current();
	const auto& lastRemote = m_remoteIn.current();
	
	assert(!lastAngularPosition.isNaN());
	assert(!lastRemote.getSetPoint().isNaN() && !std::isnan(lastRemote.getThrust()));

	dataInfo.m_data =
		m_angularPositionStablePID.calculate(
			lastRemote.getSetPoint(),
			lastAngularPosition,
			dataInfo.m_this->ellapsed().getSecs<float>()
		);
	dataInfo.m_propagate = true;
}

void Pilot::angularVelocitySetpointScale(DataInfo<Vector3>& dataInfo) const
{
	dataInfo.m_data = dataInfo.getSource<Vector3>().current() *
	m_angularPositionStableScale *
	AngleConvert::ToDegree;
	dataInfo.m_propagate = true;

	assert(!dataInfo.m_data.isNaN());
}

void Pilot::angularVelocityControlPID(DataInfo<Vector3>& dataInfo)
{
	auto lastAngularVelocity = m_angularVelocity.current();
	const auto& lastAngularVelocitySetpoint = m_angularVelocitySetpointScale.current();
	
	assert(!lastAngularVelocity.isNaN());
	assert(!lastAngularVelocitySetpoint.isNaN());

	dataInfo.m_data =
		m_angularPositionStablePID.calculate(
			lastAngularVelocitySetpoint,
			lastAngularVelocity,
			dataInfo.m_this->ellapsed().getSecs<float>()
		);
	dataInfo.m_propagate = true;
}

void Pilot::angularVelocityControlScale(DataInfo<AngularControlTrust>& dataInfo) const
{
	dataInfo.m_data.angles = dataInfo.getSource<Vector3>().current() *
		m_angularPositionRateScale;
	dataInfo.m_data.thrust = m_remoteIn.current().getThrust();
	dataInfo.m_propagate = true;

	assert(!dataInfo.m_data.angles.isNaN());
}
