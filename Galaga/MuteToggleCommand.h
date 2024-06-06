#ifndef GALAGA_MUTETOGGLECOMMAND
#define GALAGA_MUTETOGGLECOMMAND

#include <Command.h>

class MuteToggleCommand final : public Command
{
public:
	explicit MuteToggleCommand();
	virtual ~MuteToggleCommand() override;
	void Execute(float deltaTime) override;

};

#endif