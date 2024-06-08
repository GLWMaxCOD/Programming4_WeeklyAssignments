#ifndef GAME_ENGINE_COMMAND
#define GAME_ENGINE_COMMAND

namespace engine
{
	class GameObject;
}

// Interface for the Command Pattern
class Command
{
public:
	// Constructor
	explicit Command() {};

	// Deleted copy constructor to prevent copying
	Command(const Command& other) = delete;

	// Deleted move constructor to prevent moving
	Command(Command&& other) = delete;

	// Deleted copy assignment operator to prevent copying
	Command& operator=(const Command& other) = delete;

	// Deleted move assignment operator to prevent moving
	Command& operator=(Command&& other) = delete;

	// Virtual destructor for proper cleanup of derived classes
	virtual ~Command() = default;

	// Pure virtual function that must be overridden in derived classes
	// Executes the command with the provided delta time
	virtual void Execute(float deltaTime) = 0;
};

#endif