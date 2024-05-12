#include "FormationState.h"
#include "SpawningState.h"
#include "SendingEnemiesState.h"

std::unique_ptr<SpawningState> FormationState::spawning = std::make_unique<SpawningState>();
std::unique_ptr<SendingEnemiesState> FormationState::sendingEnemies = std::make_unique<SendingEnemiesState>();