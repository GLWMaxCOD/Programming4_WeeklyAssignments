#ifndef GAME_ENGINE_SPRITEANIMATORCP
#define GAME_ENGINE_SPRITEANIMATORCP
#include "Component.h"
#include <SDL.h>
#include <functional>

namespace engine
{
	class RenderComponent;
	class SpriteAnimatorCP final : public engine::Component
	{
	public:

		enum class AnimationMode 
		{
			normal,
			reverse,
			normalAndReverse				// First normal animation and when endFrame is reached it will go back in reverse to the beginFrame
		};

		SpriteAnimatorCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate);
		SpriteAnimatorCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate, int frameInc, int limitFrame, int startFrame = 0, const AnimationMode& mode = AnimationMode::normal);
		~SpriteAnimatorCP() override;

		virtual void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		const SDL_Rect& GetSpriteRect() const;

		void SetAnimationCompleteCallback(std::function<void()> callback);
		void ResetAnimation();

	private:

		void NormalUpdate();
		void ReverseUpdate();
		void SwapStartAndLimit();
		void UpdateSourceRect();

		int m_CurrentFrame;				// Current frame being render
		int m_TotalFrames;				// Amount of frames from the spritesheet
		int m_LimitFrame;				// In case when not all frames should be rendered

		int m_FrameInc;					// In case user wants to inc with different values than default (1)
		int m_TotalCols;
		int m_CurrentCol;
		int m_CurrentRow;
		int m_StartFrame;

		float m_ElapsedFrameTime;
		float m_FrameRate;

		engine::RenderComponent* m_pRenderComponent;
		SDL_Rect m_pSourceRect;
		AnimationMode m_AnimationMode;
		bool m_NormalState;				// For normalAndReverse state

		std::function<void()> m_AnimationCompleteCallback;
	};
}

#endif