#ifndef GALAGA_MENUSELECTIONCOMMAND
#define GALAGA_MENUSELECTIONCOMMAND

#include <Command.h>

class MenuSelectionCP;

// Command to handle menu selection actions
class MenuSelectionCommand final : public Command
{
public:
    explicit MenuSelectionCommand(MenuSelectionCP* pMenuSelectionCP, int menuAction);
    virtual ~MenuSelectionCommand() override;

    void Execute(float deltaTime) override;

private:
    MenuSelectionCP* m_pMenuSelection; // Pointer to the menu selection component
    int m_MenuAction;                  // Action: next (+), previous (-), or select (0)
};

#endif