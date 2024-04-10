#ifndef GALAGA_SHOOTCOMMAND
#define GALAGA_SHOOTCOMMAND

#include <Command.h>
#include <glm/glm.hpp>

class ShootCommand final : public Command
{
public:
	explicit ShootCommand(engine::GameObject* actor, glm::vec3 direction);
	virtual ~ShootCommand() override;
	void Execute(float deltaTime) override;

private:

	glm::vec3 m_Direction;
	engine::GameObject* m_Actor;
};

#endif