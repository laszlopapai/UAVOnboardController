#include <thread>
#include <wiringPi.h>
#include <iostream>
#include "HCSR04Rangefinder.h"


using namespace IoT::Core;
using namespace IoT::Device;

HCSR04Rangefinder::HCSR04Rangefinder(uint32_t trigPin, uint32_t echoPin)
	: m_trigPin(trigPin)
	, m_echoPin(echoPin)
	, m_logger(ILogger::getDefault())
{ }

bool HCSR04Rangefinder::initialize() {
	pinMode (m_trigPin, OUTPUT) ;
	pinMode (m_echoPin, INPUT)  ;

	m_acquireSignal.setWake();

	return true;
}

void HCSR04Rangefinder::run() {

	while (m_acquireSignal.waitSignal() != AwaitStopSignal::Stop) {
		float result = -1;

		digitalWrite(m_trigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(m_trigPin, LOW);

		bool ok = true;
		int32_t t  = 0; // microsec
		int32_t dt = 0; // microsec

		t = micros();
		do {
			dt = micros() - t;
		} while (digitalRead(m_echoPin) == LOW && dt < 5e3) ;

		if (dt >= 5e3) {
			m_logger->log(Warning, "Rangefinder", "Echo signal wont raise in " + std::to_string(dt * 1e-6) + "s");
			result = -std::numeric_limits<float>::infinity();

			ok = false;
		}

		t = micros();
		do {
			dt = micros() - t;
		} while (ok && digitalRead(m_echoPin) == HIGH && dt < maxDistanceWait * 1e6) ;

		// If the receive process takes more than maxSecWait than it never ends.
		if (ok && dt >= maxDistanceWait * 1e6)
		{
//			m_logger->log(Notice, Rangefinder, "Obstacle farther than measurable maximum distance");
			result = std::numeric_limits<float>::infinity();

			ok = false;
		}

		if (ok) {
			result = 5e-7f * c * dt;
		}

		m_rangeOut.pushData(result);
		delay(40);
	}
}

void HCSR04Rangefinder::onStop() {
	m_acquireSignal.setStop();
}
