#ifndef GALAGA_STARTCOMMAND
#define GALAGA_STARTCOMMAND

#include <Command.h>

// Forward declaration
class MenuState;

// Command to handle starting the game from the menu
class StartCommand final : public Command
{
public:
    explicit StartCommand(MenuState* gameState);
    virtual ~StartCommand() override;

    void Execute(float deltaTime) override;

private:
    MenuState* m_pMenuState;  // Pointer to the menu state
};

#endif // GALAGA_STARTCOMMAND