#pragma once

#include <list>
#include <Time.h>
#include <map>
#include <algorithm>
#include <cassert>

namespace IoT { namespace Core {

class StreamPart
{

public:
	virtual ~StreamPart();

	void trigger();

	//       interval   ellapsed
	//----|-----------|------
	Time ellapsed() const;

	Time interval() const
	{
		return m_interval;
	}

	virtual StreamPart& operator|(StreamPart& target);

	virtual StreamPart& operator^(StreamPart& target);

	virtual void attach(StreamPart& source);
	virtual void addTarget(StreamPart& target);

	virtual void detach(StreamPart& source);
	virtual void removeSource(StreamPart& source);
	virtual void removeTarget(StreamPart& target);

protected:
	virtual void onTrigger(StreamPart* source);

protected:
	std::list<StreamPart*> m_targets {};
	std::list<StreamPart*> m_sources {};
	Time m_lastTrigger;
	Time m_interval;
};

}}
