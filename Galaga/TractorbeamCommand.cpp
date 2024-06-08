#include "TractorbeamCommand.h"
#include "AI_GalagaCP.h"
#include "GameObject.h"

#include <iostream>

TractorbeamCommand::TractorbeamCommand(engine::GameObject* actor)
	: Command(),
	m_Actor(actor)
{
}

TractorbeamCommand::~TractorbeamCommand()
{
	//std::cout << "Tractorbeam Command destructor" << std::endl;
}

void TractorbeamCommand::Execute(float)
{
	if (m_Actor->IsActive())
	{
		// Change state into breakFormation
		auto aiComponent = m_Actor->GetComponent<AI_GalagaCP>();
		if (aiComponent && aiComponent->GetIsAttacking() != true)
		{
			aiComponent->SetFormationOnly(false);
			aiComponent->SetAttackState(AI_GalagaCP::AttackState::breakFormation); 
		}
	}
}