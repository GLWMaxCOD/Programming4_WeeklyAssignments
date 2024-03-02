#pragma once
#include "Component.h"


namespace dae
{
	class TextComponent;
	class FPSComponent final : public Component
	{
	public:

		FPSComponent(TextComponent* pTextCP);

		void Update([[maybe_unused]] const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	private:

		unsigned int m_FrameCount;
		float m_FpsTimer;
		const float MAX_SECOND;

		TextComponent* m_pTextCP;

	};
}