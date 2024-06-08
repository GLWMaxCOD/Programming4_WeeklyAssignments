#ifndef GAME_ENGINE_RENDERCOMPONENT
#define GAME_ENGINE_RENDERCOMPONENT

#include <memory>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Component.h"

namespace engine
{
	class Texture2D;
	class SpriteAnimatorCP;

	// RenderComponent handles the rendering of textures and sprites attached to a GameObject
	class RenderComponent final : public Component
	{
	public:
		RenderComponent(engine::GameObject* pOwner);
		RenderComponent(engine::GameObject* pOwner, const std::string& filename);
		~RenderComponent() override;

		// Updates the component every frame
		virtual void Update(const float deltaTime) override;

		// Renders the texture or sprite at the given position
		virtual void Render(const glm::vec3& position) const;

		// Handles messages sent to this component
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		// Sets the texture using a file path
		void SetTexture(const std::string& filename);

		// Sets the texture directly
		void SetTexture(std::shared_ptr<engine::Texture2D> texture);

		// Sets the sprite animator component
		void SetSpriteAnimatorCP(engine::SpriteAnimatorCP* spriteAnimatorCP);

		// Gets the size of the texture
		const glm::vec2 GetTextureSize();

		// Gets the size of the sprite
		const glm::vec2 GetSpriteSize() const;

		// Gets the scale of the texture or sprite
		const glm::vec2& GetScale();

		// Sets the visibility of the component
		void SetVisible(bool visible);

		// Checks if the component is visible
		bool IsVisible() const;

	private:
		std::shared_ptr<engine::Texture2D> m_texture;     // The texture to be rendered
		glm::vec2 m_Scale;                                // Scale of the texture or sprite
		glm::vec2 m_TextureSize;                          // Size of the texture
		bool m_IsTextureDirty;                            // Indicates if the texture needs to be updated
		bool m_IsVisible;                                 // Indicates if the component should be rendered

		engine::SpriteAnimatorCP* m_pSpriteAnimatorCP;    // For animated sprites only

		// Sets the scale of the texture or sprite based on the transform component
		void SetScale();

		// Calculates the size of the texture based on its original size and scale
		void CalculateTextureSize();
	};
}

#endif // GAME_ENGINE_RENDERCOMPONENT