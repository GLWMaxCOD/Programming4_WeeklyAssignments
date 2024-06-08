#ifndef GAME_ENGINE_SPRITEANIMATORCP
#define GAME_ENGINE_SPRITEANIMATORCP
#include "Component.h"
#include <SDL.h>
#include <functional>

namespace engine
{
	class RenderComponent;

	// Handles sprite animation for a GameObject
	class SpriteAnimatorCP final : public engine::Component
	{
	public:
		// Different animation modes
		enum class AnimationMode
		{
			normal,                 // Play animation frames from start to end
			reverse,                // Play animation frames from end to start
			normalAndReverse        // Play animation frames from start to end, then reverse back to start
		};

		// Constructor for standard animation
		SpriteAnimatorCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate);

		// Constructor for custom animation with additional parameters
		SpriteAnimatorCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate, int frameInc, int limitFrame, int startFrame = 0, const AnimationMode& mode = AnimationMode::normal);

		~SpriteAnimatorCP() override;

		// Updates the component every frame
		virtual void Update(const float deltaTime) override;

		// Handles messages sent to this component
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		// Gets the source rectangle for the current sprite frame
		const SDL_Rect& GetSpriteRect() const;

		// Sets a callback function to be called when the animation completes
		void SetAnimationCompleteCallback(std::function<void()> callback);

		// Resets the animation to its initial state
		void ResetAnimation();

	private:
		// Updates the animation in normal mode
		void NormalUpdate();

		// Updates the animation in reverse mode
		void ReverseUpdate();

		// Swaps the start and limit frames for normalAndReverse mode
		void SwapStartAndLimit();

		// Updates the source rectangle for the current frame
		void UpdateSourceRect();

		int m_CurrentFrame;             // Current frame being rendered
		int m_TotalFrames;              // Total number of frames in the sprite sheet
		int m_LimitFrame;               // Frame at which the animation should stop or reverse

		int m_FrameInc;                 // Frame increment value
		int m_TotalCols;                // Number of columns in the sprite sheet
		int m_CurrentCol;               // Current column in the sprite sheet
		int m_CurrentRow;               // Current row in the sprite sheet
		int m_StartFrame;               // Initial frame of the animation

		float m_ElapsedFrameTime;       // Time elapsed since the last frame change
		float m_FrameRate;              // Duration of each frame

		engine::RenderComponent* m_pRenderComponent; // Associated render component
		SDL_Rect m_pSourceRect;                      // Source rectangle for the current frame
		AnimationMode m_AnimationMode;               // Animation mode

		bool m_NormalState;             // Indicates if the animation is in normal state for normalAndReverse mode

		std::function<void()> m_AnimationCompleteCallback; // Callback for animation completion
	};
}

#endif