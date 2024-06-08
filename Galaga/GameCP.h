#ifndef GALAGA_GAMECP
#define GALAGA_GAMECP

#include <Component.h>

class GameState;

// Component that manages the game states and updates them
class GameCP final : public engine::Component
{
public:
    GameCP(engine::GameObject* pOwner);
    ~GameCP() override;

    // Update the current game state
    void Update(float deltaTime) override;

    // Handle received messages (not used in this implementation)
    void ReceiveMessage(const std::string& message, const std::string& value) override;

private:
    GameState* m_pGameState; // Pointer to the current game state
};

#endif