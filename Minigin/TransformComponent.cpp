#include "TransformComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

// Constructor with optional position and scale parameters
engine::TransformComponent::TransformComponent(engine::GameObject* pOwner, glm::vec3 position, glm::vec2 scale)
	: Component("TransformCP", pOwner),
	m_WorldPosition{ glm::vec3{ 0,0,0 } },
	m_LocalPosition{ glm::vec3{ 0,0,0 } },
	m_Scale{ scale },						// TODO : Local and World Scale and local and world rotation
	m_CenterOffset{ glm::vec3{ 0,0,0 } },
	m_IsPositionDirty{ true },
	m_pCollisionCP{ nullptr }
{
	SetLocalPosition(position);
}

// Sets the local position of the GameObject
void engine::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	m_IsPositionDirty = true;  // Mark the position as dirty to update world position later
}

// Gets the local position of the GameObject
const glm::vec3 engine::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

// Gets the world position of the GameObject
const glm::vec3 engine::TransformComponent::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		// Update the world position if it is marked as dirty
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}

// Updates the world position based on the local position and parent's world position
void engine::TransformComponent::UpdateWorldPosition()
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

			// Update bounding box for collision detection
			if (m_pCollisionCP)
			{
				m_pCollisionCP->SetBoundingBox(m_WorldPosition);
			}
		}
		m_IsPositionDirty = false; // Reset the dirty flag after updating
	}
}

// Updates the component every frame
void engine::TransformComponent::Update([[maybe_unused]] const float deltaTime)
{
	// This function can be used to update the position using physics (e.g., velocity)
}

// Sets the scale of the GameObject
void engine::TransformComponent::SetScale(const glm::vec2& scale)
{
	m_Scale = scale;
}

// Marks the position as dirty, indicating it needs to be recalculated
void engine::TransformComponent::SetPositionDirty()
{
	m_IsPositionDirty = true;
}

// Sets the center offset for rotation around its own center
void engine::TransformComponent::SetCenterOffset(const glm::vec3& centerOffset)
{
	m_CenterOffset = centerOffset;
	m_IsPositionDirty = true;
}

// Adds a collision component to be updated with the transform
void engine::TransformComponent::AddCollisionCP(engine::CollisionComponent* pComponent)
{
	m_pCollisionCP = pComponent;
}

// Gets the scale of the GameObject
const glm::vec2 engine::TransformComponent::GetScale() const
{
	return m_Scale;
}

// Handles messages sent to this component
void engine::TransformComponent::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "CollisionCP")
		{
			m_pCollisionCP = nullptr;
		}
	}
}

// Destructor
engine::TransformComponent::~TransformComponent()
{
	//std::cout << "TransformComponent destructor" << std::endl;
}