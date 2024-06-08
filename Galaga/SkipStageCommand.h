#ifndef GALAGA_SKIPSTAGECOMMAND
#define GALAGA_SKIPSTAGECOMMAND

#include <Command.h>

// Forward declaration
class GameplayState;

// Command to handle skipping to the next stage in the game
class SkipStageCommand final : public Command
{
public:
    explicit SkipStageCommand(GameplayState* gameplayState);
    virtual ~SkipStageCommand() override;

    void Execute(float deltaTime) override;

private:
    GameplayState* m_pGameplayState;  // Pointer to the current gameplay state
};

#endif // GALAGA_SKIPSTAGECOMMAND