#ifndef GAME_ENGINE_COLLISIONCOMPONENT
#define GAME_ENGINE_COLLISIONCOMPONENT

#include "Component.h"
#include <memory>
#include "Subject.h"
#include "structs.h" // For Rectf
#include <glm/glm.hpp>

namespace engine
{
	class Observer;

	// Handles collision detection and response for the GameObject
	class CollisionComponent final : public engine::Component
	{
	public:
		CollisionComponent(engine::GameObject* pOwner, const glm::vec2& gameObjectSize);

		virtual ~CollisionComponent() override;

		virtual void Update(const float deltaTime) override;	// Update the component each frame
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override; // Handle received messages

		void CollisionWith(engine::GameObject* other, engine::CollisionComponent* otherCollisionCP); // Handle collision with another GameObject
		void AddObserver(engine::Observer* pObserver);			// Add an observer for collision events

		engine::Rectf GetBoundingBox() const;					// Get the bounding box of the GameObject
		glm::vec2 GetSize() const;								// Get the size of the GameObject
		void SetSize(const glm::vec2& size);					// Set the size of the GameObject
		void SetBoundingBox(const glm::vec3& gameObjectPos);	// Set the bounding box based on the GameObject's position

		void SetEnabled(bool enabled) { m_Enabled = enabled; }	// Enable or disable collision detection

	private:
		std::unique_ptr<engine::Subject> m_CollisionSubject;	// Subject to notify observers about collisions
		Rectf m_BoundingBox;									// Bounding box used for collision detection
		glm::vec2 m_Size;										// Size of the GameObject

		bool m_Enabled{ true };									// Flag to enable/disable collision detection
	};
}

#endif // GAME_ENGINE_COLLISIONCOMPONENT
