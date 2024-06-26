#include "GameObject.h"
#include "SpriteAnimatorCP.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "iostream"

// Constructor for standard animation
engine::SpriteAnimatorCP::SpriteAnimatorCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate)
	:Component("SpriteAnimatorCP", pOwner),
	m_CurrentFrame{ 0 }, m_ElapsedFrameTime{ 0.f }, m_FrameRate{ frameRate }, m_TotalFrames{ totalFrames },
	m_CurrentCol{ 0 }, m_CurrentRow{ 0 }, m_TotalCols{ numberCols }, m_pRenderComponent{ nullptr },
	m_FrameInc{ 1 }, m_LimitFrame{ m_TotalFrames - 1 }, m_StartFrame{ 0 }, m_AnimationMode{ AnimationMode::normal },
	m_NormalState{ true }
{
	m_pRenderComponent = pOwner->GetComponent<engine::RenderComponent>();

	if (m_pRenderComponent != nullptr)
	{
		auto textureSize = m_pRenderComponent->GetTextureSize();
		m_pSourceRect.w = int(textureSize.x / m_TotalCols);
		m_pSourceRect.h = int(textureSize.y / (m_TotalFrames / m_TotalCols));

		auto textureScale = m_pRenderComponent->GetScale();
		m_pSourceRect.x = int((m_pSourceRect.w / textureScale.x) * m_CurrentCol);
		m_pSourceRect.y = int((m_pSourceRect.h / textureScale.y) * m_CurrentRow);

		m_pRenderComponent->SetSpriteAnimatorCP(this);
	}
}

// Constructor for custom animation with additional parameters
engine::SpriteAnimatorCP::SpriteAnimatorCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate,
	int frameInc, int limitFrame, int startFrame, const AnimationMode& mode)
	: SpriteAnimatorCP(pOwner, numberCols, totalFrames, frameRate)
{
	m_FrameInc = frameInc;
	m_LimitFrame = limitFrame;
	m_CurrentFrame = startFrame;
	m_StartFrame = startFrame;
	m_AnimationMode = mode;
}

// Destructor
engine::SpriteAnimatorCP::~SpriteAnimatorCP()
{

}

// Updates the component every frame
void engine::SpriteAnimatorCP::Update(const float deltaTime)
{
	m_ElapsedFrameTime += deltaTime;

	// Check if the elapsed time is larger than the frame rate
	if (m_ElapsedFrameTime > m_FrameRate)
	{
		// Elapsed time more than frame rate --> Next frame 
		switch (m_AnimationMode)
		{
		case engine::SpriteAnimatorCP::AnimationMode::normal:
			NormalUpdate();
			break;
		case engine::SpriteAnimatorCP::AnimationMode::reverse:
			ReverseUpdate();
			break;
		case engine::SpriteAnimatorCP::AnimationMode::normalAndReverse:
			if (m_NormalState)
			{
				NormalUpdate();
			}
			else
			{
				ReverseUpdate();
			}
			break;
		}

		m_ElapsedFrameTime -= m_FrameRate; // Reset accumulated time so it counts again
		UpdateSourceRect();
	}
}

// Updates the animation in normal mode
void engine::SpriteAnimatorCP::NormalUpdate()
{
	m_CurrentFrame += m_FrameInc;
	if (m_CurrentFrame > m_LimitFrame)
	{
		if (m_AnimationMode == engine::SpriteAnimatorCP::AnimationMode::normalAndReverse)
		{
			SwapStartAndLimit();
		}
		else
		{
			// Reset to start if we reached the limit
			m_CurrentFrame = 0;
			if (m_AnimationCompleteCallback)
			{
				m_AnimationCompleteCallback();
			}
		}
	}
}

// Updates the animation in reverse mode
void engine::SpriteAnimatorCP::ReverseUpdate()
{
	m_CurrentFrame -= m_FrameInc;
	if (m_CurrentFrame < m_LimitFrame)
	{
		if (m_AnimationMode == engine::SpriteAnimatorCP::AnimationMode::normalAndReverse)
		{
			SwapStartAndLimit();
		}
		else
		{
			// Reset to start if we reached the limit
			m_CurrentFrame = m_StartFrame;
			if (m_AnimationCompleteCallback)
			{
				m_AnimationCompleteCallback();
			}
		}
	}
}

// Swaps the start and limit frames for normalAndReverse mode
void engine::SpriteAnimatorCP::SwapStartAndLimit()
{
	m_NormalState = !m_NormalState;
	int saveStart = m_StartFrame;
	m_StartFrame = m_LimitFrame;
	m_LimitFrame = saveStart;
}

// Updates the source rectangle for the current frame
void engine::SpriteAnimatorCP::UpdateSourceRect()
{
	m_CurrentRow = m_CurrentFrame / static_cast<int>(m_TotalCols);
	m_CurrentCol = m_CurrentFrame % static_cast<int>(m_TotalCols);

	auto textureScale = m_pRenderComponent->GetScale();
	m_pSourceRect.x = int((m_pSourceRect.w / textureScale.x) * m_CurrentCol);
	m_pSourceRect.y = int((m_pSourceRect.h / textureScale.y) * m_CurrentRow);
}

// Gets the source rectangle for the current sprite frame
const SDL_Rect& engine::SpriteAnimatorCP::GetSpriteRect() const
{
	return m_pSourceRect;
}

// Sets a callback function to be called when the animation completes
void engine::SpriteAnimatorCP::SetAnimationCompleteCallback(std::function<void()> callback)
{
	m_AnimationCompleteCallback = std::move(callback);
}

// Resets the animation to its initial state
void engine::SpriteAnimatorCP::ResetAnimation()
{
	m_CurrentFrame = m_StartFrame;  // Reset to the initial start frame
	m_ElapsedFrameTime = 0.f;		// Reset elapsed time
	m_NormalState = true;			// Reset to normal state if using normalAndReverse mode

	// Ensure the limit frame and start frame are set correctly based on animation mode
	if (m_AnimationMode == AnimationMode::normalAndReverse)
	{
		m_LimitFrame = m_TotalFrames - 1;
		m_StartFrame = 0;
		m_CurrentFrame = m_StartFrame;
	}

	UpdateSourceRect();				// Update the source rect to the initial frame
}

// Handles messages sent to this component
void engine::SpriteAnimatorCP::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "RenderCP")
		{
			m_pRenderComponent = nullptr;
		}
	}
}