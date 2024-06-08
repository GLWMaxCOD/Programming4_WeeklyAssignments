#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "SpriteAnimatorCP.h"
#include <iostream>

engine::RenderComponent::RenderComponent(GameObject* pOwner)
	: Component("RenderCP", pOwner),
	m_texture{ nullptr },
	m_Scale{ glm::vec2{ 1.f, 1.f } },
	m_TextureSize{ glm::vec2{} },
	m_IsTextureDirty{ false }, m_IsVisible{ true }, m_pSpriteAnimatorCP{ nullptr }
{
	SetScale();
}

engine::RenderComponent::RenderComponent(GameObject* pOwner, const std::string& filename)
	: Component("RenderCP", pOwner),
	m_Scale{ glm::vec2{ 1.f, 1.f } },
	m_TextureSize{ glm::vec2{} },
	m_IsTextureDirty{ true }, m_IsVisible{ true }, m_pSpriteAnimatorCP{ nullptr }
{
	SetTexture(filename);
	SetScale();
}

// -----------------------------------------------------------------------------
//				*Set the scale of the texture*
// The scale will be defined by the transformCP from the GameObject owner
// It doesn't actually change the size of the texture, is only used for the render
// visually scale the texture
// -----------------------------------------------------------------------------
void engine::RenderComponent::SetScale()
{
	GameObject* pOwner{ GetOwner() };
	if (pOwner != nullptr)
	{
		TransformComponent* t = pOwner->GetComponent<TransformComponent>();
		if (t != nullptr)
		{
			m_Scale = t->GetScale();
		}
	}
}

// Try to create a new texture in the resourceManager and returns it created
void engine::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);

	if (m_texture != nullptr)
	{
		// New texture loaded
		m_IsTextureDirty = true;
	}

}

void engine::RenderComponent::Update(const float)
{
	if (m_IsTextureDirty)
	{
		CalculateTextureSize();
	}
}

void engine::RenderComponent::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "SpriteAnimatorCP")
		{
			m_pSpriteAnimatorCP = nullptr;
		}
	}
}

engine::RenderComponent::~RenderComponent()
{
	//std::cout << "RenderComponent destructor" << std::endl;
}

void engine::RenderComponent::SetVisible(bool visible)
{
	m_IsVisible = visible;
}

bool engine::RenderComponent::IsVisible() const
{
	return m_IsVisible;
}

void engine::RenderComponent::Render(const glm::vec3& position) const
{
	if (m_texture != nullptr)
	{
		if (m_IsVisible == true) // Check visibility before rendering
		{
			if (m_pSpriteAnimatorCP != nullptr)
			{

				auto spriteRect{ m_pSpriteAnimatorCP->GetSpriteRect() };
				float scaledSrcWidth = spriteRect.w / m_Scale.x;
				float scaledSrcHeight = spriteRect.h / m_Scale.y;

				Renderer::GetInstance().RenderSprite(*m_texture, position.x, position.y, float(spriteRect.w), float(spriteRect.h), float(spriteRect.x), float(spriteRect.y), scaledSrcWidth, scaledSrcHeight);
			}
			else
			{
				// Normal sprites without any animation
				Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y, m_TextureSize.x, m_TextureSize.y);
			}
		}
	}
}

void engine::RenderComponent::SetSpriteAnimatorCP(engine::SpriteAnimatorCP* spriteAnimatorCP)
{
	m_pSpriteAnimatorCP = spriteAnimatorCP;
	m_IsTextureDirty = true;

}

void engine::RenderComponent::SetTexture(std::shared_ptr<engine::Texture2D> texture)
{
	m_texture = texture;

	if (m_texture != nullptr)
	{
		m_IsTextureDirty = true;
	}
}
void engine::RenderComponent::CalculateTextureSize()
{
	glm::ivec2 textureSize{ m_texture->GetSize() };
	// Scale the texture 
	m_TextureSize.x = textureSize.x * m_Scale.x;
	m_TextureSize.y = textureSize.y * m_Scale.y;

	m_IsTextureDirty = false;
}

const glm::vec2 engine::RenderComponent::GetTextureSize()
{
	if (m_IsTextureDirty)
	{
		// In case someone request the texture size we calculate it
		CalculateTextureSize();
	}
	if (m_pSpriteAnimatorCP != nullptr)
	{
		auto spriteSize{ m_pSpriteAnimatorCP->GetSpriteRect() };

		glm::vec2 size{ spriteSize.w, spriteSize.h };
		return size;
	}
	return m_TextureSize;
}

const glm::vec2 engine::RenderComponent::GetSpriteSize() const
{
	if (m_pSpriteAnimatorCP != nullptr)
	{
		auto spriteSize{ m_pSpriteAnimatorCP->GetSpriteRect() };

		glm::vec2 size{ spriteSize.w, spriteSize.h };
		return size;
	}

	return glm::vec2{};
}

const glm::vec2& engine::RenderComponent::GetScale()
{
	return m_Scale;
}
