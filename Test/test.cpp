#include <limits.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>
#include <gtest/gtest_pred_impl.h>

#include "Thread/Pipe.h"
#include "Data/StreamNode.h"
#include "ILogger.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

IoT::Core::ILogger* IoT::Core::ILogger::s_instance = nullptr;

TEST(Pipe, PushOnePopAvilable) {
	IoT::Core::Pipe<float> pipe;

	const auto currentTime = IoT::Core::Time::getNow();

	pipe.pushData(1.8f, currentTime);

	float data;
	IoT::Core::Time time;

	ASSERT_TRUE(pipe.popData(&data, &time));

	ASSERT_EQ(data, 1.8f);
	ASSERT_EQ(currentTime.getMicros(), time.getMicros());

	ASSERT_TRUE(!pipe.popData(&data, &time));

	ASSERT_EQ(data, 1.8f);
	ASSERT_EQ(currentTime.getMicros(), time.getMicros());
}


TEST(Subject, SubjectPipeing) {

	float num = 0.0f;
	IoT::Core::StreamNode<float> sIn([&num](IoT::Core::DataInfo<float>& dataInfo)
	{
		dataInfo.m_propagate = true;
		num += 1.0f;
		dataInfo.m_data = num;
	});

	IoT::Core::StreamNode<float> dup([](auto &dataInfo)
	{
		dataInfo.m_propagate = true;
		auto s = static_cast<IoT::Core::StreamNode<float>*>(dataInfo.m_source);

		dataInfo.m_data = s->current() * 2.0f;
	});
	IoT::Core::StreamNode<float> add([](auto &dataInfo)
	{
		dataInfo.m_propagate = true;
		auto s = static_cast<IoT::Core::StreamNode<float>*>(dataInfo.m_source);

		dataInfo.m_data = s->current() + 10.0f;
	});
	std::vector<float> result;
	IoT::Core::StreamNode<float> sOut([&result](auto &dataInfo)
	{
		auto s = static_cast<IoT::Core::StreamNode<float>*>(dataInfo.m_source);
		float f = s->current();
		result.push_back(f);
	});

	sIn | sOut;
	sIn | dup | sOut;
	sIn | add | sOut;

	for (int i = 0; i < 10; i++) {
		sIn.trigger();
	}

	for (int i = 0; i < 10; i++) {
		float n = static_cast<float>(i + 1);
		ASSERT_EQ(result[0 + i * 3], n);
		ASSERT_EQ(result[1 + i * 3], n * 2);
		ASSERT_EQ(result[2 + i * 3], n + 10);
	}

	sIn ^ sOut;
	for (int i = 0; i < 10; i++) {
		sIn.trigger();
	}

	ASSERT_EQ(result.size(), 50);
}

void ASSERT_VECTOR3_EQ(IoT::Core::Vector3& v, IoT::Core::Vector3 expected)
{
	// ASSERT_TRUE(v.x <= expected.x + bias && v.x >= expected.x - bias);
	// ASSERT_TRUE(v.y <= expected.y + bias && v.y >= expected.y - bias);
	// ASSERT_TRUE(v.z <= expected.z + bias && v.z >= expected.z - bias);
	ASSERT_FLOAT_EQ(v.x, expected.x);
	ASSERT_FLOAT_EQ(v.y, expected.y);
	ASSERT_FLOAT_EQ(v.z, expected.z);
}

TEST(Vector3, RotationDownPos) {
	IoT::Core::Vector3 down { IoT::Core::Vector3::Down };

	IoT::Core::Quaternion q1;	
	q1.fromAngleAxis(M_PI / 4, IoT::Core::Vector3::Forward);
	auto downQ1 = q1 * down;
	downQ1.normalise();
	auto rotationQ1 = down.eulerFromDirection(downQ1);
	ASSERT_VECTOR3_EQ(rotationQ1, IoT::Core::Vector3( M_PI / 4, 0, 0 ));

	IoT::Core::Quaternion q2;
	q2.fromAngleAxis(M_PI / 4, IoT::Core::Vector3::Right);
	auto downQ2 = q2 * down;
	downQ2.normalise();
	auto rotationQ2 = down.eulerFromDirection(downQ2);
	ASSERT_VECTOR3_EQ(rotationQ2, IoT::Core::Vector3(0, M_PI / 4, 0));
	
	auto q3 = q1 * q2;
	auto downQ3 = q3 * down;
	downQ3.normalise();
	auto rotationQ3 = down.eulerFromDirection(downQ3);
	ASSERT_VECTOR3_EQ(rotationQ3, IoT::Core::Vector3(M_PI / 4, M_PI / 4, 0));

	auto notNaN =  IoT::Core::Vector3::Down.eulerFromDirection(IoT::Core::Vector3::Down);
	ASSERT_TRUE(!notNaN.isNaN());
}

TEST(Vector3, RotationDownNeg) {
	IoT::Core::Vector3 down{ IoT::Core::Vector3::Down };

	IoT::Core::Quaternion q1;
	q1.fromAngleAxis(M_PI / 4, IoT::Core::Vector3::Backward);
	auto downQ1 = q1 * down;
	downQ1.normalise();
	auto rotationQ1 = down.eulerFromDirection(downQ1);
	ASSERT_VECTOR3_EQ(rotationQ1, IoT::Core::Vector3(-M_PI / 4, 0, 0));

	IoT::Core::Quaternion q2;
	q2.fromAngleAxis(M_PI / 4, IoT::Core::Vector3::Left);
	auto downQ2 = q2 * down;
	downQ2.normalise();
	auto rotationQ2 = down.eulerFromDirection(downQ2);
	ASSERT_VECTOR3_EQ(rotationQ2, IoT::Core::Vector3(0, -M_PI / 4, 0));

	auto q3 = q1 * q2;
	auto downQ3 = q3 * down;
	downQ3.normalise();
	auto rotationQ3 = down.eulerFromDirection(downQ3);
	ASSERT_VECTOR3_EQ(rotationQ3, IoT::Core::Vector3(-M_PI / 4, -M_PI / 4, 0));
		
	auto notNaN = IoT::Core::Vector3::Down.eulerFromDirection(IoT::Core::Vector3::Down);
	ASSERT_TRUE(!notNaN.isNaN());

	IoT::Core::Vector3 v(-4.66013335e-05, 0.997142971, 0.0755382553);
	notNaN = IoT::Core::Vector3::Down.eulerFromDirection(v);
	ASSERT_TRUE(!notNaN.isNaN());
}

class Vector3Euler :
	public testing::TestWithParam<int> {
	
	void SetUp() override
	{
		float p = GetParam() / 50.0f;
		phase1 = p;
		phase2 = p * 10.0f;

		if (phase2 > 1.0f)
			phase2 -= static_cast<int>(phase2);
	}
	void TearDown() override {
		phase1 = .0f;
		phase2 = .0f;
	}

public:
	float phase1{ 0 };
	float phase2{ 0 };
};

TEST_P(Vector3Euler, RotationFromBottom) {
	IoT::Core::Vector3 down{ IoT::Core::Vector3::Down };

	const float xRot = -M_PI + (2 * M_PI * phase1);
	IoT::Core::Quaternion xQuat;
	xQuat.fromAngleAxis(xRot, IoT::Core::Vector3::Forward);

	const float yRot = -M_PI + (2 * M_PI * phase2);
	IoT::Core::Quaternion yQuat;
	yQuat.fromAngleAxis(yRot, IoT::Core::Vector3::Right);

	
	auto rotatedDown = xQuat * yQuat * down;
	rotatedDown.normalise();
	auto rotationQ1 = down.eulerFromDirection(rotatedDown);
	ASSERT_VECTOR3_EQ(rotationQ1, IoT::Core::Vector3(xRot, yRot, 0));
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, Vector3Euler, testing::Range(0, 51));

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
