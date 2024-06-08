#ifndef GALAGA_SHOOTCOMMAND
#define GALAGA_SHOOTCOMMAND

#include <Command.h>
#include <glm/glm.hpp>

// Command to handle shooting action for a game object
class ShootCommand final : public Command
{
public:
    explicit ShootCommand(engine::GameObject* actor, glm::vec3 direction);
    virtual ~ShootCommand() override;

    void Execute(float deltaTime) override;

private:
    glm::vec3 m_Direction;           // Normalized direction vector for shooting
    engine::GameObject* m_Actor;     // The actor that performs the shooting
};

#endif // GALAGA_SHOOTCOMMAND
