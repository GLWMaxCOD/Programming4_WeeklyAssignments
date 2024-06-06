#include "Servicelocator.h"

// Initialize static variables
engine::Null_Sound_System engine::Servicelocator::_default_ss;
engine::SoundSystem* engine::Servicelocator::_ss_instance{ &_default_ss };