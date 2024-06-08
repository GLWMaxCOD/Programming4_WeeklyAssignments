#ifndef GALAGA_SKIPSTAGECOMMAND
#define GALAGA_SKIPSTAGECOMMAND

#include <Command.h>
class GameplayState;
class SkipStageCommand final : public Command
{
public:
	explicit SkipStageCommand(GameplayState* gameplayState);
	virtual ~SkipStageCommand() override;
	void Execute(float deltaTime) override;

private:
	GameplayState* m_pGameplayState;

};

#endif