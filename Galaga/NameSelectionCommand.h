#ifndef GALAGA_NAMESELECTIONCOMMAND_H
#define GALAGA_NAMESELECTIONCOMMAND_H

#include <Command.h>

class NameSelectionCP;
class NameSelectionCommand final : public Command
{
public:
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
    NameSelectionCP* m_pNameSelection;
    Action m_Action;
};

#endif // GALAGA_NAMESELECTIONCOMMAND_H