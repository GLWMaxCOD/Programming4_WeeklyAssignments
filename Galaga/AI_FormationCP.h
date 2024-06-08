#ifndef GALAGA_AI_FORMATIONCP
#define GALAGA_AI_FORMATIONCP

#include <Component.h>
#include <memory>
#include <vector>
#include "FormationReaderCP.h"

class FormationCP;

// Manages all enemies in the formation, acting like a Squad leader giving orders to them
class AI_FormationCP final : public engine::Component
{
public:
	AI_FormationCP(engine::GameObject* pOwner, const std::string& JSONPath);
	~AI_FormationCP() override;

	void Update(const float deltaTime) override; // Update the formation AI state each frame
	void ReceiveMessage(const std::string&, const std::string&) override; // Handle received messages

	void Reset(const std::string& JSONPath); // Reset the AI state with a new formation
	void SetVersusMode(bool isVersusMode);	// Set the versus mode state
	void SpawnEnemies();					// Start the enemy spawning process
	void ChangeToWaitState();				// Change the formation state to waiting

private:
	// Enum for the formation states
	enum class FormationState
	{
		waiting,			// Waiting for orders
		spawning_enemies,	// Spawning enemies into formation
		sending_enemies		// Sending enemies to attack
	};

	// Enum for the spawn order states
	enum class SpawnOrderState
	{
		top_first,
		left,
		right,
		top_second,
		top_third
	};

	FormationCP* m_pFormationCP;		// Pointer to the formation component
	FormationState m_FormationState;	// Current formation state
	SpawnOrderState m_MovingFTState;	// Current spawn order state

	const std::string BEES_TYPE;
	const std::string BUTTERFLIES_TYPE;
	const std::string GALAGAS_TYPE;

	// Spawning state management
	void GetEnemyData(const std::string& type, std::vector<engine::GameObject*>& container);
	void SpawnEnemies(const float deltaTime);											// Spawn enemies progressively
	void UpdateSpawningBatch(const std::string& batch);									// Update the current batch of spawning enemies
	void ActivateEnemy(const std::string& type);										// Activate an enemy from the formation
	short GetEnemyTypeCount(const std::string& type) const;								// Get the count of active enemies of a certain type
	void SetEnemyTypeCount(const std::string& type);									// Increment the count of active enemies of a certain type

	std::vector<std::unique_ptr<FormationReaderCP::EnemySpawnInfo>> m_vSpawningInfo;	// All spawn info for all enemies
	std::vector < std::pair<std::string, short>> m_vCurrentSpawnInfo;					// Info from the current batch spawning

	bool m_IsSpawnInfoReaded;
	std::string m_LastEnemyType;				// Last type of enemy spawned
	short m_BeesActiveCount;
	short m_ButterfliesActiveCount;
	short m_GalagasActiveCount;
	bool m_SpawnFirstType;

	const float m_TimeEnemySpawn;
	float m_CurrentTimeSpawn;

	// Sending enemies state management
	void SendEnemies();							// Send enemies to attack
	const std::string& GetNextEnemyToSend();	// Get the next enemy type to send

	std::string m_EnemyToSend;
	const int NEXT_GALAGA;						// Every 4 enemies, one Galaga is sent
	int m_SendGalagaCount;						// Counter to determine when to send a Galaga

	const float m_TimeEnemySend;

	bool m_IsVersusMode;
};

#endif // GALAGA_AI_FORMATIONCP
