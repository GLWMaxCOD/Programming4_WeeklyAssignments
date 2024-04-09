#include "Event.h"



engine::Event::Event(const std::string& eventType)
	: m_Type{ eventType }
{ }

// Compares both strings to see if they are the same or not
bool engine::Event::IsSameEvent(const std::string& eventType) const
{
	return m_Type.compare(eventType) == 0;   // Will return true if both strings are equal
}

const std::string& engine::Event::GetType() const
{
	return m_Type;
}