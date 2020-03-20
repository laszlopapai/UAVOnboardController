#pragma once

#include "StreamNode.h"
#include <vector>
#include <fstream>

namespace IoT {
	namespace Core {

		template<typename T>
		class Recorder
			: public StreamNode<T>
		{
		public:
			Recorder() 
			: StreamNode<T>([](auto&){})
			{}

			~Recorder() { }

			float state() {
				return m_playbackIndex / (float)m_records.size();
			}

			bool loadData(std::string file)
			{
				std::ifstream ifs(file, std::ios::binary);

				if (!ifs.good())
					return false;

				decltype(m_records.size()) size;
				ifs >> size;
				m_records.resize(size);
				ifs.read(reinterpret_cast<char *>(m_records.data()), sizeof(Timed<T>) * m_records.size());
				auto result = ifs.good();
				ifs.close();

				if (result)
					m_playbackIndex = 0;

				return result;
			}

			bool saveData(std::string file)
			{
				std::ofstream ofs(file, std::ios::binary);

				if (!ofs.good())
					return false;

				int s = /*sizeof(T) / (float)sizeof(Timed<T>) */ m_records.size();
				ofs << s;
				ofs.write(reinterpret_cast<char *>(m_records.data()), sizeof(Timed<T>) * s);
				auto result = ofs.good();
				ofs.close();
					
				return result;
			}

		protected:
			void onTrigger(StreamPart* sourcePtr) override
			{
				auto& m_sourceInfo = StreamNode<T>::m_dataInfo;// static_cast<StreamNode<T>*>(this)->m_dataInfo;

				m_sourceInfo.m_propagate = true;
				m_sourceInfo.m_source = sourcePtr;
					
				auto& source = *static_cast<StreamNode<T>*>(sourcePtr);

				if (m_playbackIndex == -1) {
					Timed<T> record;
					record.time = source.interval();
					source >> record.data;
					m_records.push_back(record);
				}
				else {
					auto& record = m_records[m_playbackIndex];
					m_sourceInfo.m_propagate = record.time.getMicros() < StreamPart::ellapsed().getMicros();
					m_sourceInfo.m_data = record.data;
					if (m_sourceInfo.m_propagate) {
						m_playbackIndex++;
						m_playbackIndex %= m_records.size();
					}
				}

				if (m_sourceInfo.m_propagate)
					StreamPart::onTrigger(sourcePtr);
			}

		private:
			int m_playbackIndex{-1};
			std::vector<Timed<T>> m_records;
		};

	}
}
