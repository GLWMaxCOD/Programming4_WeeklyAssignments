#include "PlayerInputCP.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "ShootCommand.h"
#include "MenuSelectionCommand.h"
#include "NameSelectionCommand.h"
#include "MuteToggleCommand.h"
#include "GameObject.h"
#include "Controller.h"
#include "PlayerCP.h"

PlayerInputCP::PlayerInputCP(engine::GameObject* pOwner)
	: Component("PlayerInputCP", pOwner),
	m_PlayerDied{ false },
	m_ControllerIdx{ -1 }
{

}


// When two players are playing :
//		PLAYER 1 -> Keyboard + 2º controller
//		PLAYER 2 -> 1º controller
void PlayerInputCP::TwoPlayersGameplayInput()
{
	auto playerIdx = GetOwner()->GetComponent <PlayerCP>()->GetPlayerIndex();
	if (playerIdx == 2)
	{
		// Player 2 -> Only Add controller input. No keyboard
		// Player 2 will have Controller 0
		GameplayControllerInput(playerIdx - 2);
		return;
	}

	// ADD an extra Controller for the Player 1
	auto& input = engine::InputManager::GetInstance();
	m_ControllerIdx = input.GetFreeController();

	// PLAYER 1 KEYBOARD AND CONTROLLER INPUT
	GameplayKeyboardInput();
	GameplayControllerInput(m_ControllerIdx);

}

// INPUT SETUP FOR THE MENU NAVIGATION
void PlayerInputCP::MenuInput(MenuSelectionCP* pMenuSelectionCP)
{
	auto& input = engine::InputManager::GetInstance();

	auto playerCP{ GetOwner()->GetComponent<PlayerCP>() };

	// Only Player 1 can control the menu
	if (playerCP != nullptr && playerCP->GetPlayerIndex() == 2)
		return;

	// Keyboard input for the MENU
	SDL_KeyCode keyEnter{ SDLK_RETURN };
	SDL_KeyCode keyArrowDown{ SDLK_DOWN };
	SDL_KeyCode keyArrowUP{ SDLK_UP };

	std::unique_ptr<Command> upSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, -1);
	std::unique_ptr<Command> downSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 1);
	std::unique_ptr<Command> selectOption = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 0);

	input.BindCommand(std::move(upSelectionCommand), keyArrowUP, engine::InputType::Down);
	input.BindCommand(std::move(downSelectionCommand), keyArrowDown, engine::InputType::Down);
	input.BindCommand(std::move(selectOption), keyEnter, engine::InputType::Down);

	// Menu controller input for Player 1
	AddMenuControllerInput(pMenuSelectionCP);
}

void PlayerInputCP::NameInput(NameSelectionCP* pNameSelectionCP)
{
	auto& input = engine::InputManager::GetInstance();

	// Bind keyboard input for name selection
	SDL_KeyCode keyEnter{ SDLK_RETURN };
	SDL_KeyCode keyArrowUp{ SDLK_UP };
	SDL_KeyCode keyArrowDown{ SDLK_DOWN };
	SDL_KeyCode keyArrowLeft{ SDLK_LEFT };
	SDL_KeyCode keyArrowRight{ SDLK_RIGHT };

	input.BindCommand(std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::CycleUp), keyArrowUp, engine::InputType::Down);
	input.BindCommand(std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::CycleDown), keyArrowDown, engine::InputType::Down);
	input.BindCommand(std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::MoveLeft), keyArrowLeft, engine::InputType::Down);
	input.BindCommand(std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::MoveRight), keyArrowRight, engine::InputType::Down);
	input.BindCommand(std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::Confirm), keyEnter, engine::InputType::Down);

	// Name controller input for Player 1
	AddNameControllerInput(pNameSelectionCP);
}

void PlayerInputCP::AddNameControllerInput(NameSelectionCP* pNameSelectionCP)
{
	auto& input = engine::InputManager::GetInstance();
	if (m_ControllerIdx == -1)
	{
		// No controller assigned yet to this player
		m_ControllerIdx = input.GetFreeController();
		if (m_ControllerIdx == -1)
			return;		// No free controller found
	}

	// Input for the name selection
	std::unique_ptr<Command> cycleUpCommand = std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::CycleUp);
	std::unique_ptr<Command> cycleDownCommand = std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::CycleDown);
	std::unique_ptr<Command> moveLeftCommand = std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::MoveLeft);
	std::unique_ptr<Command> moveRightCommand = std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::MoveRight);
	std::unique_ptr<Command> confirmCommand = std::make_unique<NameSelectionCommand>(pNameSelectionCP, NameSelectionCommand::Action::Confirm);

	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadUp, engine::InputType::Down, std::move(cycleUpCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadDown, engine::InputType::Down, std::move(cycleDownCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadLeft, engine::InputType::Down, std::move(moveLeftCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadRigth, engine::InputType::Down, std::move(moveRightCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::ButtonA, engine::InputType::Down, std::move(confirmCommand));
}

void PlayerInputCP::AddMenuControllerInput(MenuSelectionCP* pMenuSelectionCP)
{
	auto& input = engine::InputManager::GetInstance();
	if (m_ControllerIdx == -1)
	{
		// No controller assigned yet to this player
		m_ControllerIdx = input.GetFreeController();
		if (m_ControllerIdx == -1)
			return;		// No free controller found
	}

	// Input for the menu
	std::unique_ptr<Command> upSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, -1);
	std::unique_ptr<Command> downSelectionCommand = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 1);
	std::unique_ptr<Command> selectOption = std::make_unique<MenuSelectionCommand>(pMenuSelectionCP, 0);

	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadUp, engine::InputType::Down, std::move(upSelectionCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadDown, engine::InputType::Down, std::move(downSelectionCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::ButtonA, engine::InputType::Down, std::move(selectOption));

}

// Assign the controller buttons for Gameplay to the specified controller 
void PlayerInputCP::GameplayControllerInput(unsigned controllerIdx)
{
	m_ControllerIdx = controllerIdx;

	auto& input = engine::InputManager::GetInstance();
	// Input for the Gameplay
	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ -1, 0, 0 });
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ 1, 0, 0 });
	std::unique_ptr<Command> shootCommand = std::make_unique<ShootCommand>(GetOwner(), glm::vec3{ 0, -1, 0 });

	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadLeft, engine::InputType::Pressed, std::move(moveLeftCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::DPadRigth, engine::InputType::Pressed, std::move(moveRightCommand));
	input.BindCommand(m_ControllerIdx, Controller::XboxControllerButton::ButtonA, engine::InputType::Down, std::move(shootCommand));
}

void PlayerInputCP::GameplayKeyboardInput()
{
	auto& input = engine::InputManager::GetInstance();

	// Keys A and D will be used to move Player from left to right
	SDL_KeyCode keyA{ SDLK_a };
	SDL_KeyCode keyD{ SDLK_d };
	SDL_KeyCode key_Space{ SDLK_SPACE };
	SDL_KeyCode keyM{ SDLK_m };

	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ -1, 0, 0 });
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ 1, 0, 0 });
	std::unique_ptr<Command> shootCommand = std::make_unique<ShootCommand>(GetOwner(), glm::vec3{ 0, -1, 0 });
	std::unique_ptr<Command> toggleMuteCommand = std::make_unique<MuteToggleCommand>();

	// Bind all commands with their corresponding keys
	input.BindCommand(std::move(moveLeftCommand), keyA, engine::InputType::Pressed);
	input.BindCommand(std::move(moveRightCommand), keyD, engine::InputType::Pressed);
	input.BindCommand(std::move(shootCommand), key_Space, engine::InputType::Down);
	input.BindCommand(std::move(toggleMuteCommand), keyM, engine::InputType::Down);
}

void PlayerInputCP::Update(const float)
{

}

void PlayerInputCP::ReceiveMessage(const std::string&, const std::string&)
{

}

void  PlayerInputCP::SetPlayerDied()
{
	m_PlayerDied = true;
}

PlayerInputCP::~PlayerInputCP()
{
	// Make sure all commands are unbind if the player dies during gameplay (not when closing game)
	if (m_PlayerDied)
	{
		auto& input = engine::InputManager::GetInstance();

		input.UnbindAllCommands(m_ControllerIdx);

		m_PlayerDied = false;
	}
}