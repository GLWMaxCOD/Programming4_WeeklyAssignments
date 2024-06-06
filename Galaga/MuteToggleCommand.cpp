#include "MuteToggleCommand.h"
#include "Servicelocator.h"

MuteToggleCommand::MuteToggleCommand()
	: Command()
{

}
MuteToggleCommand::~MuteToggleCommand()
{

}

void MuteToggleCommand::Execute(float)
{
	auto& soundSystem = engine::Servicelocator::Get_Sound_System();
	soundSystem.ToggleSoundSystemSound();
}