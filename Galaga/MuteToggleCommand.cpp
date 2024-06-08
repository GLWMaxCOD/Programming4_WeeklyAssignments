#include "MuteToggleCommand.h"
#include "Servicelocator.h"

MuteToggleCommand::MuteToggleCommand()
    : Command()
{
    // Constructor
}

MuteToggleCommand::~MuteToggleCommand()
{
    // Destructor
}

void MuteToggleCommand::Execute(float)
{
    // Toggle the mute state of the sound system
    auto& soundSystem = engine::Servicelocator::Get_Sound_System();
    soundSystem.ToggleSoundSystemSound();
}