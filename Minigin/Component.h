#ifndef GAME_ENGINE_COMPONENT
#define GAME_ENGINE_COMPONENT

#include <string>

namespace engine
{
	class GameObject;

	// Base class for all components attached to GameObjects
	class Component
	{
	public:
		// Constructor to initialize the component with a name and its owning GameObject
		Component(const std::string& name, engine::GameObject* pOwner);
		virtual ~Component() = default;

		// Delete copy and move constructors and assignment operators
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		// Update the component every frame
		virtual void Update(const float deltaTime) = 0;

		// Receive messages sent to this component
		virtual void ReceiveMessage(const std::string& message, const std::string& value) = 0;

		// Get the name of the component
		const std::string& GetName() const { return m_Tag; };

	protected:
		std::string m_Tag;											// Tag or name of the component
		engine::GameObject* GetOwner() const { return m_pOwner; };	// Get the owner GameObject

	private:
		engine::GameObject* m_pOwner; // Pointer to the owning GameObject
	};
}

#endif // GAME_ENGINE_COMPONENT
