#ifndef GALAGA_MOVECOMMAND
#define GALAGA_MOVECOMMAND

#include "Command.h"
#include <glm/glm.hpp>

// MOVEMENT COMMAND : MOVE THE ACTOR IN THE DESIRED DIRECTION AND SPEED
//                    Command to handle movement actions
class MoveCommand final : public Command
{
public:
    explicit MoveCommand(engine::GameObject* actor, glm::vec3 direction);
    virtual ~MoveCommand() override;

    void Execute(float deltaTime) override;

private:
    glm::vec3 m_Direction;      // Normalized direction vector
    engine::GameObject* m_Actor; // Pointer to the actor GameObject
};

#endif