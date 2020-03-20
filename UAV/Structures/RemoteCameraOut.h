#pragma once

#include <Network/IPackage.h>
#include <Thread/IObservable.h>
#include <Thread/NotifyPipe.hpp>
#include <Thread/AtomicMethod.hpp>

namespace IoT {
	namespace UAV {

		class RemoteCameraOut
			: public Core::IPackage
		{
		public:

			void deserialize(const std::vector<uint8_t> &data) override { }

			void serialize(std::vector<uint8_t> &data) override {
				data = *m_vector;
				delete m_vector;
				m_vector = nullptr;
			}

			void setVector(std::vector<uint8_t> *vector) {
				delete m_vector;
				m_vector = vector;
			}

		private:
			std::vector<uint8_t> *m_vector { nullptr };
		};


		class FrameToRemoteCameraOutConverter
			: Core::IObserver
		{

		public:
			void setFrameIn(Core::NotifyPipe<std::vector<uint8_t >*> & pipe) {
				m_frameIn = &pipe;
				pipe.setObserver(this);
			}

			void setCameraPackageOut(Core::AtomicMethod<RemoteCameraOut> & cameraOut) {
				m_cameraPackageOut = &cameraOut;
			}

			void observableChanged() override {
				std::vector<uint8_t> *frame;

				if (m_frameIn->popData(&frame)) {
					m_cameraPackageOut->executeAtomicMethod([&](RemoteCameraOut & remoteCameraOut){
						remoteCameraOut.setVector(frame);
					});
					m_cameraPackageOut->triggerChange();
				}
			}

		private:
			Core::NotifyPipe<std::vector<uint8_t>*> *m_frameIn { nullptr };
			Core::AtomicMethod<RemoteCameraOut> *m_cameraPackageOut { nullptr };
		};

	}
}
