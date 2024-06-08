#include "GameCP.h"
#include "MenuState.h"

// Constructor: Initializes the component with the owner and sets the initial game state to MenuState
GameCP::GameCP(engine::GameObject* pOwner)
    : Component("GameCP", pOwner),
    m_pGameState{ nullptr }
{
    // Initial state is the menu
    m_pGameState = new MenuState();
    m_pGameState->OnEnter(); // Call the on-enter logic for the initial state
}

// Destructor: Cleans up the current game state
GameCP::~GameCP()
{
    delete m_pGameState;
}

// Update the current game state
void GameCP::Update(float deltaTime)
{
    GameState* pNewGameState = m_pGameState->GetChangeState();

    if (pNewGameState != nullptr)
    {
        // Transition to a new state
        delete m_pGameState;            // Clean up the old state
        m_pGameState = pNewGameState;   // Switch to the new state
        pNewGameState->OnEnter();       // Initialize the new state
    }

    m_pGameState->UpdateState(deltaTime); // Update the current state
}

// Handle received messages (currently not used)
void GameCP::ReceiveMessage(const std::string&, const std::string&)
{

}
