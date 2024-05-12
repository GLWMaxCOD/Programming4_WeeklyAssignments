#ifndef GALAGA_FORMATION_STATE
#define GALAGA_FORMATION_STATE

#include <memory>

class SpawningState;
class SendingEnemiesState;

class FormationState {
public:
    static std::unique_ptr<SpawningState> spawning;
    static std::unique_ptr<SendingEnemiesState> sendingEnemies;

    virtual void UpdateState(const float deltaTime) = 0;
    virtual FormationState* GetNewState() = 0;
};

#endif