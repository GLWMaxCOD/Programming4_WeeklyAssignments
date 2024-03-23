#ifndef GAME_ENGINE_RENDERCOMPONENT
#define GAME_ENGINE_RENDERCOMPONENT

#include <memory>
#include <glm/glm.hpp>
#include <string>
#include "Component.h"


namespace dae
{
	class Texture2D;
	class RenderComponent final : public Component
	{
	public:

		RenderComponent(dae::GameObject* pOwner);
		RenderComponent(dae::GameObject* pOwner, const std::string& filename);
		~RenderComponent() override;

		virtual void Update(const float deltaTime) override;
		virtual void Render(const glm::vec3& position) const;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<dae::Texture2D> texture);

	private:

		std::shared_ptr<dae::Texture2D> m_texture{};
		glm::vec2 m_Scale;
		glm::vec2 m_TextureSize;
		bool m_IsTextureDirty;							// To indicate if the texture changed or not

		void SetScale();
	};
}

#endif
