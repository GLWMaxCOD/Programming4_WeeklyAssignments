#ifndef GAME_ENGINE_PLAYERINPUTCP
#define GAME_ENGINE_PLAYERINPUTCP

#include "Component.h"

class MenuSelectionCP;
class NameSelectionCP;
// SETUP ALL INPUT NEEDED IN ORDER TO CONTROL THE PLAYER CHARACTER
class PlayerInputCP final : public engine::Component
{
public:
	PlayerInputCP(engine::GameObject* pOwner);							// Using keyboard
	virtual ~PlayerInputCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	// INPUT SETUP
	void MenuInput(MenuSelectionCP* pMenuSelectionCP);					// Both keyboard and controller for Player 1
	void NameInput(NameSelectionCP* pNameSelectionCP);					
	void TwoPlayersGameplayInput();										// Two players playing setup
	void GameplayControllerInput(unsigned controllerIdx);				// Add this player a controller with the specified controllerIDX
	void GameplayKeyboardInput();

	void SetPlayerDied();

private:
	void AddMenuControllerInput(MenuSelectionCP* pMenuSelectionCP);			// Allow the Player 1 to control the menu
	void AddNameControllerInput(NameSelectionCP* pNameSelectionCP);			// Allow the Player 1 to control the name selection

	bool m_PlayerDied;			// This is to ensure we unbind commands associated to the gameObject when it dies and not when program closes
	int m_ControllerIdx;		// Controller being used
};

#endif