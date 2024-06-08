#include "AI_FormationCP.h"
#include "AI_GalagaCP.h"
#include "GameObject.h"
#include "FormationCP.h"
#include "FormationReaderCP.h"
#include "EnemyCP.h"
#include <iostream>

// Constructor: Initializes the AI_FormationCP with the given GameObject owner and JSON path
AI_FormationCP::AI_FormationCP(engine::GameObject* pOwner, const std::string& JSONPath)
	: Component("AI_FormationCP", pOwner),
	m_FormationState{ FormationState::waiting }, m_MovingFTState{ SpawnOrderState::top_first },
	BEES_TYPE{ "bees" }, BUTTERFLIES_TYPE{ "butterflies" }, GALAGAS_TYPE{ "galagas" },
	m_SpawnFirstType{ true }, m_CurrentTimeSpawn{ 0.f }, m_TimeEnemySpawn{ 0.25f }, m_IsSpawnInfoReaded{ false },
	m_LastEnemyType{ }, m_TimeEnemySend{ 1.5f }, NEXT_GALAGA{ 4 }, m_EnemyToSend{ BEES_TYPE }, m_SendGalagaCount{ 0 },
	m_BeesActiveCount{ 0 }, m_ButterfliesActiveCount{ 0 }, m_GalagasActiveCount{ 0 },
	m_IsVersusMode{ false }, m_pFormationCP{ pOwner->GetComponent<FormationCP>() }
{
	auto jsonReaderCP = GetOwner()->GetComponent<FormationReaderCP>();
	if (jsonReaderCP != nullptr)
	{
		auto vSpawnInfo = std::move(jsonReaderCP->ReadSpawnOrder(JSONPath));
		for (auto& enemyInfo : vSpawnInfo)
		{
			m_vSpawningInfo.emplace_back(std::make_unique<FormationReaderCP::EnemySpawnInfo>(*enemyInfo));
		}
	}
	jsonReaderCP->ClearJSONFile();
}

// Destructor
AI_FormationCP::~AI_FormationCP()
{
}

// Set the versus mode state
void AI_FormationCP::SetVersusMode(bool isVersusMode)
{
	m_IsVersusMode = isVersusMode;
}

// Update the formation AI state each frame
void AI_FormationCP::Update(const float deltaTime)
{
	switch (m_FormationState)
	{
	case AI_FormationCP::FormationState::waiting:
		// Do nothing in waiting state
		break;
	case AI_FormationCP::FormationState::spawning_enemies:
		SpawnEnemies(deltaTime);
		break;
	case AI_FormationCP::FormationState::sending_enemies:
	{
		m_CurrentTimeSpawn += deltaTime;
		if (m_CurrentTimeSpawn > m_TimeEnemySend)
		{
			m_CurrentTimeSpawn = 0;
			SendEnemies();
		}
		break;
	}
	}
}

// Get enemy data of a specific type and fill the container with those enemies
void AI_FormationCP::GetEnemyData(const std::string& type, std::vector<engine::GameObject*>& container)
{
	auto formationCP = GetOwner()->GetComponent<FormationCP>();
	auto enemiesData = formationCP->GetEnemies(type);
	container.reserve(enemiesData.size());
	for (const auto& enemy : enemiesData)
	{
		container.emplace_back(enemy);
	}
}

// Spawn enemies progressively based on the current state
void AI_FormationCP::SpawnEnemies(const float deltaTime)
{
	m_CurrentTimeSpawn += deltaTime;

	if (m_CurrentTimeSpawn >= m_TimeEnemySpawn)
	{
		m_CurrentTimeSpawn = 0.f;

		switch (m_MovingFTState)
		{
		case AI_FormationCP::SpawnOrderState::top_first:
			UpdateSpawningBatch("top_first");
			break;
		case AI_FormationCP::SpawnOrderState::left:
		{
			// Alternate between spawning enemies on the left
			if (!m_IsSpawnInfoReaded)
			{
				// Only get info if not already read
				for (const auto& spawnInfo : m_vSpawningInfo)
				{
					if (spawnInfo->GetStartPos() == "left")
					{
						std::pair pair = std::make_pair(spawnInfo->GetEnemyType(), spawnInfo->GetSpawnAmount());
						m_vCurrentSpawnInfo.emplace_back(pair);
					}
				}
				m_IsSpawnInfoReaded = true;
			}

			bool allEnemiesSpawned{ false };
			for (const auto& pair : m_vCurrentSpawnInfo)
			{
				// Alternate between spawning enemies
				if (m_LastEnemyType.empty() || m_LastEnemyType != pair.first)
				{
					short totalActive{ GetEnemyTypeCount(pair.first) };
					if (totalActive != -1)
					{
						m_LastEnemyType = pair.first;
						if (totalActive == pair.second)
						{
							allEnemiesSpawned = true;
						}
						else
						{
							ActivateEnemy(pair.first);
							break;
						}
					}
					else
					{
						allEnemiesSpawned = true;
					}
				}
			}

			if (allEnemiesSpawned)
			{
				// This batch is finished -> Activate the next batch of enemies
				m_MovingFTState = SpawnOrderState::right;
				m_IsSpawnInfoReaded = false;
				m_vCurrentSpawnInfo.clear();
			}
			break;
		}
		case AI_FormationCP::SpawnOrderState::right:
			UpdateSpawningBatch("right");
			break;
		case AI_FormationCP::SpawnOrderState::top_second:
			UpdateSpawningBatch("top_second");
			break;
		case AI_FormationCP::SpawnOrderState::top_third:
			UpdateSpawningBatch("top_third");
			break;
		}
	}
}

// Update the current batch of spawning enemies based on the batch name
void AI_FormationCP::UpdateSpawningBatch(const std::string& batch)
{
	if (!m_IsSpawnInfoReaded)
	{
		// Only get info if not already read
		for (const auto& spawnInfo : m_vSpawningInfo)
		{
			if (spawnInfo->GetStartPos() == batch)
			{
				std::pair pair = std::make_pair(spawnInfo->GetEnemyType(), spawnInfo->GetSpawnAmount());
				m_vCurrentSpawnInfo.emplace_back(pair);
			}
		}
		m_IsSpawnInfoReaded = true;
	}

	bool allEnemiesSpawned{ false };
	for (const auto& pair : m_vCurrentSpawnInfo)
	{
		short totalActive{ GetEnemyTypeCount(pair.first) };
		if (totalActive != -1)
		{
			if (totalActive == pair.second)
			{
				allEnemiesSpawned = true;
			}
			else
			{
				ActivateEnemy(pair.first);
			}
		}
		else
		{
			allEnemiesSpawned = true;
		}
	}

	if (allEnemiesSpawned)
	{
		// This batch is finished -> Activate the next batch of enemies
		if (m_MovingFTState == SpawnOrderState::top_first)
		{
			m_MovingFTState = SpawnOrderState::left;
		}
		else if (m_MovingFTState == SpawnOrderState::right)
		{
			m_MovingFTState = SpawnOrderState::top_second;
		}
		else if (m_MovingFTState == SpawnOrderState::top_second)
		{
			m_MovingFTState = SpawnOrderState::top_third;
		}
		else if (m_MovingFTState == SpawnOrderState::top_third)
		{
			m_FormationState = FormationState::sending_enemies;
		}

		m_vCurrentSpawnInfo.clear();
		m_IsSpawnInfoReaded = false;
	}
}

// Send enemies to attack progressively
void AI_FormationCP::SendEnemies()
{
	std::vector<engine::GameObject*> galagas = m_pFormationCP->GetEnemies(GALAGAS_TYPE);

	// Always check if versus Galaga can be sent
	for (auto& enemy : galagas)
	{
		auto aiComponent = enemy->GetComponent<AI_GalagaCP>();
		if (aiComponent && aiComponent->IsVersusMode())
		{
			if (enemy->IsActive() && aiComponent->GetAttackState() == AI_GalagaCP::AttackState::formationOnly)
			{
				aiComponent->SetAttackState(AI_GalagaCP::AttackState::breakFormation);
				return;
			}
		}
	}

	// Existing logic for sending other enemies
	std::string nextEnemy = GetNextEnemyToSend();

	std::vector<engine::GameObject*> enemies = m_pFormationCP->GetEnemies(nextEnemy);
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		auto& enemy = enemies[i];
		if (enemy->IsActive())
		{
			EnemyCP* pEnemyCP = enemy->GetComponent<EnemyCP>();
			if (pEnemyCP->GetCurrentState() == EnemyCP::ENEMY_STATE::waiting)
			{
				// Active enemy and in waiting state -> Send him to attack
				pEnemyCP->ChangeCurrentState(EnemyCP::ENEMY_STATE::attack);
				break;
			}
		}
	}
}

// Get the next enemy type to send
const std::string& AI_FormationCP::GetNextEnemyToSend()
{
	// First check if it is time to send a Galaga (every 4 enemies, one Galaga is sent)
	if (m_SendGalagaCount >= NEXT_GALAGA)
	{
		if (m_pFormationCP->AreEnemiesLeft(GALAGAS_TYPE))
		{
			// Only reset if there are galagas left
			m_SendGalagaCount = 0;
			return GALAGAS_TYPE;
		}
	}

	// If not. Check if send bees or butterfly (If last one was bees, then now is turn of butterflies)
	if (m_EnemyToSend == BEES_TYPE)
	{
		m_SendGalagaCount++;
		if (m_pFormationCP->AreEnemiesLeft(BUTTERFLIES_TYPE))
		{
			m_EnemyToSend = BUTTERFLIES_TYPE;
		}

		if (m_pFormationCP->AreEnemiesLeft(BEES_TYPE))
		{
			return BEES_TYPE;
		}
	}

	if (m_EnemyToSend == BUTTERFLIES_TYPE)
	{
		m_SendGalagaCount++;
		m_EnemyToSend = BEES_TYPE;
		if (m_pFormationCP->AreEnemiesLeft(BUTTERFLIES_TYPE))
		{
			return BUTTERFLIES_TYPE;
		}
	}

	// If we reach this part means that there is no butterflies and bees left
	// so we only need to send galagas
	m_SendGalagaCount = NEXT_GALAGA;
	return GALAGAS_TYPE;
}

// Activate the corresponding enemy from the formation
void AI_FormationCP::ActivateEnemy(const std::string& type)
{
	if (m_pFormationCP != nullptr)
	{
		short currentActive = GetEnemyTypeCount(type);

		if (currentActive != -1)
		{
			std::vector<engine::GameObject*>& enemies = m_pFormationCP->GetEnemies(type);
			// Make sure we get a valid index
			if (currentActive < int(enemies.size()))
			{
				enemies[currentActive]->SetIsActive(true);
				SetEnemyTypeCount(type);
			}
		}
	}
}

// Get the count of active enemies of a certain type
short AI_FormationCP::GetEnemyTypeCount(const std::string& type) const
{
	if (type == BEES_TYPE)
	{
		return m_BeesActiveCount;
	}
	else if (type == BUTTERFLIES_TYPE)
	{
		return m_ButterfliesActiveCount;
	}
	else if (type == GALAGAS_TYPE)
	{
		return m_GalagasActiveCount;
	}

	// Invalid enemy type
	return -1;
}

// Increment the count of active enemies of a certain type
void AI_FormationCP::SetEnemyTypeCount(const std::string& type)
{
	if (type == BEES_TYPE)
	{
		m_BeesActiveCount++;
	}
	else if (type == BUTTERFLIES_TYPE)
	{
		m_ButterfliesActiveCount++;
	}
	else if (type == GALAGAS_TYPE)
	{
		m_GalagasActiveCount++;
	}
}

// Handle received messages
void AI_FormationCP::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "FormationCP")
		{
			m_pFormationCP = nullptr;
		}
	}
}

// Reset the AI state with a new formation
void AI_FormationCP::Reset(const std::string& JSONPath)
{
	m_FormationState = FormationState::waiting; // Wait until order to spawn enemies
	m_MovingFTState = SpawnOrderState::top_first;
	m_SpawnFirstType = true;
	m_CurrentTimeSpawn = 0.f;
	m_IsSpawnInfoReaded = false;
	m_LastEnemyType = " ";
	m_EnemyToSend = BEES_TYPE;
	m_SendGalagaCount = 0;
	m_BeesActiveCount = 0;
	m_ButterfliesActiveCount = 0;
	m_GalagasActiveCount = 0;
	m_vSpawningInfo.clear();

	auto jsonReaderCP = GetOwner()->GetComponent<FormationReaderCP>();
	if (jsonReaderCP != nullptr)
	{
		auto vSpawnInfo = std::move(jsonReaderCP->ReadSpawnOrder(JSONPath));
		for (auto& enemyInfo : vSpawnInfo)
		{
			m_vSpawningInfo.emplace_back(std::make_unique<FormationReaderCP::EnemySpawnInfo>(*enemyInfo));
		}
	}

	jsonReaderCP->ClearJSONFile();
}

// Start the enemy spawning process
void AI_FormationCP::SpawnEnemies()
{
	m_FormationState = AI_FormationCP::FormationState::spawning_enemies;
}

// Change the formation state to waiting
void AI_FormationCP::ChangeToWaitState()
{
	m_FormationState = FormationState::waiting;
	m_MovingFTState = SpawnOrderState::top_first;
}