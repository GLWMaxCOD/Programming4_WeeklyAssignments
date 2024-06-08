#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "SpriteAnimatorCP.h"
#include <iostream>

// Constructor: Initializes the render component without a texture
engine::RenderComponent::RenderComponent(GameObject* pOwner)
	: Component("RenderCP", pOwner),
	m_texture{ nullptr },
	m_Scale{ glm::vec2{ 1.f, 1.f } },
	m_TextureSize{ glm::vec2{} },
	m_IsTextureDirty{ false }, m_IsVisible{ true }, m_pSpriteAnimatorCP{ nullptr }
{
	SetScale();
}

// Constructor: Initializes the render component with a texture from the specified file
engine::RenderComponent::RenderComponent(GameObject* pOwner, const std::string& filename)
	: Component("RenderCP", pOwner),
	m_Scale{ glm::vec2{ 1.f, 1.f } },
	m_TextureSize{ glm::vec2{} },
	m_IsTextureDirty{ true }, m_IsVisible{ true }, m_pSpriteAnimatorCP{ nullptr }
{
	SetTexture(filename);
	SetScale();
}

// Destructor
engine::RenderComponent::~RenderComponent()
{
	// std::cout << "RenderComponent destructor" << std::endl;
}

// Sets the scale of the texture or sprite based on the transform component of the owner GameObject
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

// Loads a new texture from the specified file and sets it
void engine::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);

	if (m_texture != nullptr)
	{
		// New texture loaded
		m_IsTextureDirty = true;
	}
}

// Updates the component every frame, recalculates the texture size if needed
void engine::RenderComponent::Update(const float)
{
	if (m_IsTextureDirty)
	{
		CalculateTextureSize();
	}
}

// Handles messages sent to this component
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

// Sets the visibility of the component
void engine::RenderComponent::SetVisible(bool visible)
{
	m_IsVisible = visible;
}

// Checks if the component is visible
bool engine::RenderComponent::IsVisible() const
{
	return m_IsVisible;
}

// Renders the texture or sprite at the given position
void engine::RenderComponent::Render(const glm::vec3& position) const
{
	if (m_texture != nullptr && m_IsVisible)
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

// Sets the sprite animator component for animated sprites
void engine::RenderComponent::SetSpriteAnimatorCP(engine::SpriteAnimatorCP* spriteAnimatorCP)
{
	m_pSpriteAnimatorCP = spriteAnimatorCP;
	m_IsTextureDirty = true;
}

// Sets the texture directly
void engine::RenderComponent::SetTexture(std::shared_ptr<engine::Texture2D> texture)
{
	m_texture = texture;

	if (m_texture != nullptr)
	{
		m_IsTextureDirty = true;
	}
}

// Calculates the size of the texture based on its original size and scale
void engine::RenderComponent::CalculateTextureSize()
{
	glm::ivec2 textureSize{ m_texture->GetSize() };
	// Scale the texture 
	m_TextureSize.x = textureSize.x * m_Scale.x;
	m_TextureSize.y = textureSize.y * m_Scale.y;

	m_IsTextureDirty = false;
}

// Gets the size of the texture, recalculates if needed
const glm::vec2 engine::RenderComponent::GetTextureSize()
{
	if (m_IsTextureDirty)
	{
		// In case someone requests the texture size we calculate it
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

// Gets the size of the sprite if a sprite animator component is attached
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

// Gets the scale of the texture or sprite
const glm::vec2& engine::RenderComponent::GetScale()
{
	return m_Scale;
}