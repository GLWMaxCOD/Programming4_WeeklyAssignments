#ifndef GALAGA_PLAYER2VERSUSCP_H
#define GALAGA_PLAYER2VERSUSCP_H

#include "Component.h"

class Player2VersusCP : public engine::Component
{
public:
    Player2VersusCP(engine::GameObject* pOwner, unsigned int playerIdx);
    virtual ~Player2VersusCP() override;

    virtual void Update(const float deltaTime) override;
    virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

    void SetupControls();

private:
    unsigned int m_PlayerIdx;
};

#endif // GALAGA_PLAYER2VERSUSCP_H