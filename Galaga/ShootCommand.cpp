#include "ShootCommand.h"
#include "GameObject.h"
#include "MissileManagerCP.h"

// Constructor to initialize the shoot command with the actor and direction
ShootCommand::ShootCommand(engine::GameObject* actor, glm::vec3 direction)
    : Command(),
    m_Actor(actor)
{
    m_Direction = glm::normalize(direction);   // Normalize the direction vector
}

// Destructor
ShootCommand::~ShootCommand()
{

}

// Execute the shoot command
void ShootCommand::Execute(float)
{
    if (m_Actor->IsActive())
    {
        auto missileManagerCP = m_Actor->GetComponent<MissileManagerCP>();
        if (missileManagerCP != nullptr)
        {
            missileManagerCP->Fire(m_Direction); // Fire a missile in the specified direction
        }
    }
}