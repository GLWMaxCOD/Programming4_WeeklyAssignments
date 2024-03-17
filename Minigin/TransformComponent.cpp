#include "TransformComponent.h"
#include "GameObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

TransformComponent::TransformComponent(dae::GameObject* pOwner, glm::vec3 position, glm::vec2 scale)
	: Component("TransformCP", pOwner),
	m_WorldPosition{ glm::vec3{ 0,0,0 } },
	m_LocalPosition{ glm::vec3{ 0,0,0 } },
	m_CenterOffset{ glm::vec3{ 0,0,0 } },
	m_IsPositionDirty{ false }
{
	// World position will automatically be updated when required
	SetLocalPosition(position);

	// TODO : Local and World Scale and local and world rotation

	SetScale(scale);
}

TransformComponent::~TransformComponent()
{
	std::cout << "TransformComponent destructor" << std::endl;
}

void TransformComponent::Update([[maybe_unused]] const float deltaTime)
{
	// Update position using physics (Velocity etc)
}

void TransformComponent::UpdateWorldPosition()
{
	if (m_IsPositionDirty)
	{
		auto pOwner = GetOwner();
		if (pOwner != nullptr)
		{
			if (pOwner->getParent() == nullptr)
			{
				// No parent -> WorldPos = LocalPos
				m_WorldPosition = m_LocalPosition + m_CenterOffset;
			}
			else
			{
				// Calculate the worldPosition with the parentWorldPos and the localPos of the child
				m_WorldPosition = pOwner->getParent()->GetWorldPosition() + m_LocalPosition;
			}

			pOwner->SavePreviousWorldPosition(m_WorldPosition);

			// Update children position to move along with the parent
			pOwner->UpdateChildrenPosition();
		}
	}
	m_IsPositionDirty = false;
}

void TransformComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

// Changing the position will always be the localPosition
// Next time worldPosition is asked, it will be updated with the local before returning
void TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	SetPositionDirty();  // Everytime localPosition is changed, the worldPosition also needs to be updated
}

void TransformComponent::SetCenterOffset(const glm::vec3& centerOffset)
{
	m_CenterOffset = centerOffset;
	SetPositionDirty();
}


void TransformComponent::SetScale(const glm::vec2& scale)
{
	m_Scale = scale;

}

// WorldPosition needs to be updated
void TransformComponent::SetPositionDirty()
{
	m_IsPositionDirty = true;
}

// Always check if the localPosition has changed before returning
const glm::vec3 TransformComponent::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		// Update the world position
		UpdateWorldPosition();
	}

	return m_WorldPosition;
}

const glm::vec3 TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::vec2 TransformComponent::GetScale() const
{
	return m_Scale;
}