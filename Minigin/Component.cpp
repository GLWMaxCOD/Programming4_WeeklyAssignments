#include "Component.h"
#include "GameObject.h"


Component::Component(const std::string& name, dae::GameObject* pOwner)
	: m_Name{ name },
	m_pOwner{ pOwner }
{

}