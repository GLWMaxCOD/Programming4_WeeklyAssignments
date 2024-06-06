#ifndef GALAGA_GAMEOVERSTATE
#define GALAGA_GAMEOVERSTATE

#include "GameState.h"
#include "PlayerScoreCP.h"
#include <vector>
#include <memory>
#include <string>

namespace engine
{
	class GameObject;
}
class NameSelectionCP;
class GameOverState final : public GameState
{
public:
	GameOverState() = default;
	~GameOverState() override;

	void OnEnter() override;
	void OnExit() override;

	GameState* GetChangeState() override;
	void UpdateState(const float deltaTime) override;

private:
	void UpdateUIObjects(const float deltaTime);
	std::vector<std::pair<std::string, int>> LoadHighScores();
	void SaveScoreToJson(PlayerScoreCP* pPlayerScore, const std::string& playerName);
	void UpdateHighScoresUI(const std::vector<std::pair<std::string, int>>& highScores);

	float m_elapsedSec{ 0.f };
	float m_MaxTime{ 1.5f };

	std::vector<engine::GameObject*> m_GameOverObjects{};
	int m_CurrentShowing{ 0 };

	NameSelectionCP* m_pNameSelectionCP{ nullptr };
	bool m_NameEntered{ false };

	std::vector<engine::GameObject*> m_HighScoresObjects{};
};

#endif