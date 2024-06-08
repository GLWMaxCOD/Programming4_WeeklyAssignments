#include "CollisionComponent.h"
#include "GameObject.h"
#include <iostream>
#include "Event.h"

// Constructor: Initializes the CollisionComponent with the owner GameObject and its size
engine::CollisionComponent::CollisionComponent(engine::GameObject* pOwner, const glm::vec2& gameObjectSize)
	: Component("CollisionCP", pOwner), m_Size{ gameObjectSize }
{
	auto pTransformCP = pOwner->GetComponent<engine::TransformComponent>();
	if (pTransformCP != nullptr)
	{
		auto gameObjectPos = pTransformCP->GetWorldPosition();
		SetBoundingBox(gameObjectPos);

		// Register this collision component with the transform component for position updates
		pTransformCP->AddCollisionCP(this);
	}
}

// Destructor
engine::CollisionComponent::~CollisionComponent()
{
}

// Add an observer for collision events
void engine::CollisionComponent::AddObserver(engine::Observer* pObserver)
{
	if (m_CollisionSubject == nullptr)
	{
		m_CollisionSubject = std::make_unique<engine::Subject>();
	}
	m_CollisionSubject->AddObserver(pObserver);
}

// Handle collision with another GameObject
void engine::CollisionComponent::CollisionWith(engine::GameObject* other, engine::CollisionComponent* otherCollisionCP)
{
	if (!m_Enabled) return; // If collision is disabled, do nothing

	// Only check for collisions if there is an observer and the GameObjects are of different types
	if (m_CollisionSubject != nullptr && other->Tag() != GetOwner()->Tag())
	{
		engine::Rectf otherBoundingBox = otherCollisionCP->GetBoundingBox();

		// Check for overlap in both x and y axes
		if (m_BoundingBox.left < otherBoundingBox.width && m_BoundingBox.width > otherBoundingBox.left)
		{
			if (m_BoundingBox.bottom < otherBoundingBox.height && m_BoundingBox.height > otherBoundingBox.bottom)
			{
				// Both GameObjects are colliding -> Notify observers
				engine::Event collisionEvent{ "CollisionWith " + other->Tag() };
				m_CollisionSubject->NotifyObservers(other, collisionEvent);
			}
		}
	}
}

// Get the bounding box of the GameObject
engine::Rectf engine::CollisionComponent::GetBoundingBox() const
{
	return m_BoundingBox;
}

// Get the size of the GameObject
glm::vec2 engine::CollisionComponent::GetSize() const
{
	return m_Size;
}

// Set the size of the GameObject
void engine::CollisionComponent::SetSize(const glm::vec2& size)
{
	m_Size = size;
	SetBoundingBox(GetOwner()->GetComponent<engine::TransformComponent>()->GetWorldPosition());
}

// Set the bounding box based on the GameObject's position
void engine::CollisionComponent::SetBoundingBox(const glm::vec3& gameObjectPos)
{
	m_BoundingBox.left = gameObjectPos.x;
	m_BoundingBox.bottom = gameObjectPos.y;
	m_BoundingBox.width = m_BoundingBox.left + m_Size.x;
	m_BoundingBox.height = m_BoundingBox.bottom + m_Size.y;
}

// Update the component each frame (empty implementation here)
void engine::CollisionComponent::Update(const float)
{
}

// Handle received messages (empty implementation here)
void engine::CollisionComponent::ReceiveMessage(const std::string&, const std::string&)
{
}
