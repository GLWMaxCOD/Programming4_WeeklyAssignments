#include "Component.h"
#include "GameObject.h"

// Constructor: Initializes the Component with its name and the GameObject it belongs to
engine::Component::Component(const std::string& name, engine::GameObject* pOwner)
	: m_Tag{ name },
	m_pOwner{ pOwner }
{
	
}