#ifndef GALAGA_MENUSTATE
#define GALAGA_MENUSTATE

#include "GameState.h"
#include <memory>
#include <vector>
#include "GameObject.h"

class MenuSelectionCP;
class MenuState final : public GameState
{
public:
    ~MenuState() override;
    void OnEnter() override;
    void OnExit() override;
    GameState* GetChangeState() override;
    void UpdateState(const float deltaTime) override;

    void StartGame();

private:
    void InitBackground();
    void InitMenuInput();
    void InitMenuUI();
    void HideUIElements();
    void ShowUIElements();

    std::vector<std::shared_ptr<engine::GameObject>> m_vBackgroundGO; // Background game objects for parallax effect
    std::vector<std::shared_ptr<engine::GameObject>> m_vMenuGO;       // Menu UI game objects
    MenuSelectionCP* m_pMenuSelectionCP{};                            // Component to handle menu selection
    engine::GameObject* m_pControlsImage{};                           // Controls image

    const std::string ONE_PLAYER_OPT{ "1 PLAYER" };
    const std::string TWO_PLAYERS_OPT{ "2 PLAYERS" };
    const std::string VERSUS_OPT{ "VERSUS" };
    const std::string CONTROLS_OPT{ "CONTROLS" };

    bool m_ChangeState = false;
    bool m_ShowingControls = false;
};

#endif