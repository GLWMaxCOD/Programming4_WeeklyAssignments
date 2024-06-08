#ifndef GAME_ENGINE_TRANSFORMCOMPONENT
#define GAME_ENGINE_TRANSFORMCOMPONENT

#include "Component.h"
#include <glm/glm.hpp>

namespace engine
{
	class CollisionComponent;

	// Manages the position, scale, and other transformations of a GameObject
	class TransformComponent final : public Component
	{
	public:
		// Constructor with optional position and scale parameters
		TransformComponent(engine::GameObject* pOwner, glm::vec3 position = glm::vec3{ 0.f, 0.f, 0.f },
			glm::vec2 scale = glm::vec2{ 1.f, 1.f });

		virtual ~TransformComponent() override;

		// Updates the component every frame
		virtual void Update(const float deltaTime) override;

		// Handles messages sent to this component
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		// Updates the world position based on local position and parent transformation
		void UpdateWorldPosition();

		// Gets the world position of the GameObject
		const glm::vec3 GetWorldPosition();

		// Gets the local position of the GameObject
		const glm::vec3 GetLocalPosition() const;

		// Gets the scale of the GameObject
		const glm::vec2 GetScale() const;

		// Sets the local position of the GameObject
		void SetLocalPosition(const glm::vec3& position);

		// Sets the center offset for rotation around its own center
		void SetCenterOffset(const glm::vec3& centerOffset);

		// Marks the position as dirty, indicating it needs to be recalculated
		void SetPositionDirty();

		// Sets the scale of the GameObject
		void SetScale(const glm::vec2& scale);

		// Adds a collision component to be updated with the transform
		void AddCollisionCP(engine::CollisionComponent* pComponent);

	private:
		glm::vec3 m_WorldPosition;        // Global position relative to the world space
		glm::vec3 m_LocalPosition;        // Position relative to the parent (local space)
		glm::vec2 m_Scale;                // Scale of the GameObject
		glm::vec3 m_CenterOffset;         // Offset for rotation around its own center

		bool m_IsPositionDirty;           // Flag to indicate if the position needs to be recalculated
		engine::CollisionComponent* m_pCollisionCP; // Pointer to the associated collision component
	};
}

#endif