#ifndef GALAGA_TRACTORBEAMCOMMAND
#define GALAGA_TRACTORBEAMCOMMAND

#include <Command.h>
#include <glm/glm.hpp>

class TractorbeamCommand final : public Command
{
public:
	explicit TractorbeamCommand(engine::GameObject* actor);
	virtual ~TractorbeamCommand() override;
	void Execute(float deltaTime) override;

private:

	engine::GameObject* m_Actor;
};

#endif