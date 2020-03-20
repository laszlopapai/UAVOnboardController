#pragma once

#include<functional>
#include "StreamPart.h"

namespace IoT {
	namespace Core {

		template<typename T>
		class StreamNode;

		template<typename T>
		struct DataInfo
		{
			template<typename ST>
			StreamNode<ST>& getSource()
			{
				return *static_cast<StreamNode<ST>*>(m_source);
			}

			bool m_propagate;
			StreamPart *m_source;
			StreamNode<T> *m_this;
			T m_data;
		};

		template<typename T>
		class StreamNode
			: public StreamPart
		{

		public:

			StreamNode(std::function<void(DataInfo<T>&)> f)
				: m_function({ std::move(f) })
			{ }

			StreamNode()
			{ }

			~StreamNode() { }

			const T& current() const
			{
				return m_dataInfo.m_data;
			}


			StreamNode<T>& operator<<(const T &data)
			{
				m_dataInfo.m_data = data;
				StreamPart::onTrigger(this);
				return *this;
			}

			const StreamNode<T>& operator>>(T &data) const
			{
				data = m_dataInfo.m_data;
				return *this;
			}

			bool isReady() const
			{
				return m_isDataReady;
			}

		protected:
			void onTrigger(StreamPart* sourcePtr) override
			{
				m_dataInfo.m_propagate = false;
				m_dataInfo.m_source = sourcePtr;
				m_dataInfo.m_this = this;
					
				if (m_function)
					m_function(m_dataInfo);
				else {
					(*static_cast<StreamNode<T>*>(sourcePtr)) >> m_dataInfo.m_data;
					m_dataInfo.m_propagate = true;
				}

				m_isDataReady = m_dataInfo.m_propagate || m_isDataReady;

				if (m_dataInfo.m_propagate)
					StreamPart::onTrigger(sourcePtr);
			}

		protected:
			bool m_isDataReady{ false };
			DataInfo<T> m_dataInfo;
			std::function<void(DataInfo<T>&)> m_function;
		};

	}
}
