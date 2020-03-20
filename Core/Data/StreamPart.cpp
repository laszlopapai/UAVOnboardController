#include "StreamPart.h"
#include <algorithm>

using namespace IoT::Core;

StreamPart::~StreamPart()
{
	for (auto& source : m_sources) {
		source->removeTarget(*this);
	}

	for (auto& target : m_targets) {
		target->removeSource(*this);
	}
}

void StreamPart::trigger()
{
	onTrigger(this);
}

Time StreamPart::ellapsed() const
{
	return Time::getNow() - m_lastTrigger;
}

void StreamPart::attach(StreamPart& source)
{
	m_sources.push_back(&source);
	source.addTarget(*this);
}

void StreamPart::addTarget(StreamPart& target)
{
	m_targets.push_back(&target);
}

void StreamPart::detach(StreamPart& source)
{
	removeSource(source);
	source.removeTarget(*this);
}

void StreamPart::removeSource(StreamPart& source)
{
	auto it = std::find(m_sources.begin(), m_sources.end(), &source);
	if (it != m_sources.end())
		m_sources.erase(it);
}

void StreamPart::removeTarget(StreamPart& target)
{
	auto it = std::find(m_targets.begin(), m_targets.end(), &target);
	if (it != m_targets.end())
		m_targets.erase(it);
}

StreamPart& StreamPart::operator|(StreamPart& target)
{
	target.attach(*this);
	return target;
}

StreamPart& StreamPart::operator^(StreamPart& target)
{
	target.detach(*this);
	return target;
}

void StreamPart::onTrigger(StreamPart* source)
{
	m_interval = ellapsed();
	m_lastTrigger = Time::getNow();
	for (auto& target : m_targets) {
		target->onTrigger(this);
	}
}
