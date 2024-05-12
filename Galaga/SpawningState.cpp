#include "SpawningState.h"
#include "SendingEnemiesState.h"
#include "FormationState.h"

SpawningState::SpawningState()
{

}

void SpawningState::UpdateState(const float)
{

}

FormationState* SpawningState::GetNewState() 
{
    return FormationState::sendingEnemies.get();
}