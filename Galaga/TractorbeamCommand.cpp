#include "TractorbeamCommand.h"
#include "AI_GalagaCP.h"
#include "GameObject.h"

// Constructor to initialize the tractor beam command with the actor
TractorbeamCommand::TractorbeamCommand(engine::GameObject* actor)
    : Command(),
    m_Actor(actor)
{

}

TractorbeamCommand::~TractorbeamCommand()
{
    // Destructor
}

// Execute the tractor beam command
void TractorbeamCommand::Execute(float)
{
    if (m_Actor->IsActive())
    {
        // Change state into breakFormation
        auto aiComponent = m_Actor->GetComponent<AI_GalagaCP>();
        if (aiComponent && !aiComponent->GetIsAttacking())
        {
            aiComponent->SetFormationOnly(false);
            aiComponent->SetAttackState(AI_GalagaCP::AttackState::breakFormation);
        }
    }
}