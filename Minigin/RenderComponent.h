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
	class RenderComponent final : public Component
	{
	public:

		RenderComponent(engine::GameObject* pOwner);
		RenderComponent(engine::GameObject* pOwner, const std::string& filename);
		~RenderComponent() override;

		virtual void Update(const float deltaTime) override;
		virtual void Render(const glm::vec3& position) const;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<engine::Texture2D> texture);
		void SetSpriteAnimatorCP(engine::SpriteAnimatorCP* spriteAnimatorCP);

		const glm::vec2 GetTextureSize();
		const glm::vec2 GetSpriteSize() const;
		const glm::vec2& GetScale();

		void SetVisible(bool visible);
		bool IsVisible() const;

	private:

		std::shared_ptr<engine::Texture2D> m_texture{};
		glm::vec2 m_Scale;
		glm::vec2 m_TextureSize;
		bool m_IsTextureDirty;							// To indicate if the texture changed or not
		bool m_IsVisible;								// To indicate if the component should be rendered

		engine::SpriteAnimatorCP* m_pSpriteAnimatorCP;	// For animated sprites only	

		void SetScale();
		void CalculateTextureSize();
	};
}

#endif
