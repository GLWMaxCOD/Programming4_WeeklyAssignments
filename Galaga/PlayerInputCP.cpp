#include "PlayerInputCP.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "ShootCommand.h"
#include "GameObject.h"
#include "Controller.h"

PlayerInputCP::PlayerInputCP(engine::GameObject* pOwner)
	: Component("PlayerInputCP", pOwner)
{
	auto& input = engine::InputManager::GetInstance();

	// Keys A and D will be used to move Player from left to right
	SDL_KeyCode keyA{ SDLK_a };
	SDL_KeyCode keyD{ SDLK_d };
	SDL_KeyCode key_Space{ SDLK_SPACE };

	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ -1, 0, 0 });
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(pOwner, glm::vec3{ 1, 0, 0 });
	std::unique_ptr<Command> shootCommand = std::make_unique<ShootCommand>(pOwner, glm::vec3{ 0, -1, 0 });

	// Bind all commands with their corresponding keys
	input.BindCommand(std::move(moveLeftCommand), keyA, engine::InputType::Pressed);
	input.BindCommand(std::move(moveRightCommand), keyD, engine::InputType::Pressed);

	input.BindCommand(std::move(shootCommand), key_Space, engine::InputType::Down);
}

PlayerInputCP::PlayerInputCP(engine::GameObject* pOwner, unsigned controllerIdx)
	: Component("PlayerInputCP", pOwner)
{
	AddControllerMovement(controllerIdx);
}


void PlayerInputCP::AddControllerMovement(unsigned controllerIdx)
{
	auto& input = engine::InputManager::GetInstance();

	std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ -1, 0, 0 });
	std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ 1, 0, 0 });

	input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadLeft, engine::InputType::Pressed, std::move(moveLeftCommand));
	input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadRigth, engine::InputType::Pressed, std::move(moveRightCommand));

}

void PlayerInputCP::Update([[maybe_unused]] const float deltaTime)
{


}

void PlayerInputCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

PlayerInputCP::~PlayerInputCP()
{
	/*
	// Make sure we dont try to move
	auto& input = engine::InputManager::GetInstance();
	for (const auto& key : m_Keys)
	{
		input.UnbindCommand(key);
	}
	*/
}