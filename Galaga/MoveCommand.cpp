#include "MoveCommand.h"
#include "GameObject.h"
#include "MoveComponent.h"

MoveCommand::MoveCommand(engine::GameObject* actor, glm::vec3 direction)
    : Command(),
    m_Actor(actor)
{
    m_Direction = glm::normalize(direction);   // Normalize the direction vector
}

MoveCommand::~MoveCommand()
{
    // Destructor
}

void MoveCommand::Execute(float deltaTime)
{
    if (m_Actor->IsActive())
    {
        auto moveCP = m_Actor->GetComponent<MoveComponent>();

        if (moveCP != nullptr)
        {
            moveCP->Move(deltaTime, m_Direction);
        }
    }
}