#ifndef GALAGA_NAMESELECTIONCOMMAND_H
#define GALAGA_NAMESELECTIONCOMMAND_H

#include <Command.h>

class NameSelectionCP;

// Command to handle name selection actions in the game
class NameSelectionCommand final : public Command
{
public:
    // Enum representing different name selection actions
    enum class Action
    {
        CycleUp,
        CycleDown,
        MoveLeft,
        MoveRight,
        Confirm
    };

    NameSelectionCommand(NameSelectionCP* pNameSelectionCP, Action action);
    virtual ~NameSelectionCommand() override;

    void Execute(float deltaTime) override;

private:
    NameSelectionCP* m_pNameSelection; // Pointer to the name selection component
    Action m_Action;                   // The action to perform
};

#endif // GALAGA_NAMESELECTIONCOMMAND_H