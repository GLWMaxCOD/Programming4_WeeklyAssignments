#include "SkipStageCommand.h"
#include "SceneManager.h"
#include "GameplayState.h"
#include <iostream>

SkipStageCommand::SkipStageCommand(GameplayState* gameplayState)
	: m_pGameplayState{ gameplayState }
{

}

SkipStageCommand::~SkipStageCommand()
{

}

void SkipStageCommand::Execute(float)
{

	auto& sceneManager = engine::SceneManager::GetInstance();
	if (!sceneManager.AreScenesLeft())
		return;		// No more levels to skip from

	if (m_pGameplayState != nullptr && m_pGameplayState->ArePlayersAlive())
	{
		// Go next stage & deactivate the enemies from the current stage
		m_pGameplayState->SkipStage();
	}

}