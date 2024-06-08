#include "StartCommand.h"
#include "MenuState.h"

// Constructor to initialize the start command with the menu state
StartCommand::StartCommand(MenuState* gameState)
    : Command(),
    m_pMenuState{ gameState }
{

}

// Destructor
StartCommand::~StartCommand()
{
    // Destructor implementation
}

// Execute the start command
void StartCommand::Execute(float)
{
    if (m_pMenuState != nullptr)
    {
        m_pMenuState->StartGame();
    }
}