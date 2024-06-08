#include "SkipStageCommand.h"
#include "SceneManager.h"
#include "GameplayState.h"

// Constructor to initialize the skip stage command with the gameplay state
SkipStageCommand::SkipStageCommand(GameplayState* gameplayState)
    : m_pGameplayState{ gameplayState }
{

}

// Destructor
SkipStageCommand::~SkipStageCommand()
{

}

// Execute the skip stage command
void SkipStageCommand::Execute(float)
{
    auto& sceneManager = engine::SceneManager::GetInstance();
    if (!sceneManager.AreScenesLeft())
        return; // No more levels to skip

    if (m_pGameplayState != nullptr && m_pGameplayState->ArePlayersAlive())
    {
        // Go to the next stage and deactivate the enemies from the current stage
        m_pGameplayState->SkipStage();
    }
}