#include "Player2VersusCP.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "ShootCommand.h"
#include "TractorbeamCommand.h"
#include "GameObject.h"

Player2VersusCP::Player2VersusCP(engine::GameObject* pOwner, unsigned int playerIdx)
    : Component("Player2VersusCP", pOwner), m_PlayerIdx{ playerIdx }
{
}

Player2VersusCP::~Player2VersusCP()
{
}

void Player2VersusCP::SetupControls()
{
    auto& input = engine::InputManager::GetInstance();

    //setup the controller inputs
    unsigned int controllerIdx = m_PlayerIdx - 2;
    //std::unique_ptr<Command> moveLeftCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ -1, 0, 0 });
    //std::unique_ptr<Command> moveRightCommand = std::make_unique<MoveCommand>(GetOwner(), glm::vec3{ 1, 0, 0 });
    std::unique_ptr<Command> fireCommand = std::make_unique<ShootCommand>(GetOwner(), glm::vec3{ 0, 1, 0 });
    std::unique_ptr<Command> tractorbeamCommand = std::make_unique<TractorbeamCommand>(GetOwner());

    //input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadLeft, engine::InputType::Pressed, std::move(moveLeftCommand));
    //input.BindCommand(controllerIdx, Controller::XboxControllerButton::DPadRigth, engine::InputType::Pressed, std::move(moveRightCommand));
    input.BindCommand(controllerIdx, Controller::XboxControllerButton::ButtonA, engine::InputType::Down, std::move(fireCommand));
    input.BindCommand(controllerIdx, Controller::XboxControllerButton::ButtonB, engine::InputType::Down, std::move(tractorbeamCommand));
}

void Player2VersusCP::Update(const float deltaTime)
{
    (void)deltaTime;
    // Update logic for Player 2 controlling a Galaga enemy
}

void Player2VersusCP::ReceiveMessage(const std::string& message, const std::string& value)
{
    (void)message;
    (void)value;
    // Handle messages if necessary
}