#ifndef GALAGA_AI_FORMATIONCP
#define GALAGA_AI_FORMATIONCP

#include <Component.h>
#include <vector>

class FormationCP;
// Manages all enemies from the formation, acting like a Squad leader giving orders to them
class AI_FormationCP final : public engine::Component
{

public:
	AI_FormationCP(engine::GameObject* pOwner, const std::string& JSONPath);
	~AI_FormationCP() override;

	void Update(const float deltaTime) override;
	void ReceiveMessage(const std::string&, const std::string&) override;

private:

	enum class FormationState
	{
		setupFormation,
		attacking
	};

	enum class MovingFormationState
	{
		top,
		left,
		right,
		backToTop
	};

	void GetEnemyData(const std::string& type, std::vector<engine::GameObject*>& container);
	void MoveIntoFormation(const float deltaTime);
	void ActivateEnemy(const std::string& type, unsigned short& currentActive);

	FormationCP* m_pFormationCP;
	FormationState m_FormationState;
	MovingFormationState m_MovingFTState;

	// SETUP FORMATION DATA
	unsigned short m_TopFormationAmount;
	unsigned short m_LeftFormationAmount;
	unsigned short m_Top1FormationAmount;
	unsigned short m_RightFormationAmount;
	unsigned short m_BeesActive;
	unsigned short m_ButterfliesActive;
	unsigned short m_GalagasActive;
	bool m_SpawnFirstType;				// For the left batch of enemies.

	const float m_TimeEnemySpawn;
	float m_CurrentTimeSpawn;
};

#endif