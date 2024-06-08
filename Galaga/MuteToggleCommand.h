#ifndef GALAGA_MUTETOGGLECOMMAND
#define GALAGA_MUTETOGGLECOMMAND

#include <Command.h>

// Command to toggle mute state of the sound system
class MuteToggleCommand final : public Command
{
public:
    explicit MuteToggleCommand();
    virtual ~MuteToggleCommand() override;

    void Execute(float deltaTime) override;
};

#endif