#ifndef GALAGA_TRACTORBEAMCOMMAND
#define GALAGA_TRACTORBEAMCOMMAND

#include <Command.h>

// Command to handle the tractor beam action
class TractorbeamCommand final : public Command
{
public:
    explicit TractorbeamCommand(engine::GameObject* actor);
    virtual ~TractorbeamCommand() override;
    void Execute(float deltaTime) override;

private:
    engine::GameObject* m_Actor;  // Pointer to the actor performing the action
};

#endif // GALAGA_TRACTORBEAMCOMMAND