#include "AI_FormationCP.h"
#include "GameObject.h"
#include "FormationCP.h"
#include "FormationReaderCP.h"
#include <iostream>

AI_FormationCP::AI_FormationCP(engine::GameObject* pOwner, const std::string& JSONPath)
	: Component("AI_FormationCP", pOwner),
	m_FormationState{ FormationState::setupFormation }, m_MovingFTState{ MovingFormationState::top },
	m_Top1FormationAmount{ 0 }, m_TopFormationAmount{ 0 }, m_RightFormationAmount{ 0 }, m_LeftFormationAmount{ 0 }, m_SpawnFirstType{ true },
	m_CurrentTimeSpawn{ 0.f }, m_TimeEnemySpawn{ 0.5f }
{
	auto jsonReaderCP = GetOwner()->GetComponent<FormationReaderCP>();
	if (jsonReaderCP != nullptr)
	{
		auto orderInfoMap = std::move(jsonReaderCP->ReadOrder(JSONPath));
		for (const auto& order : orderInfoMap)
		{
			std::string type = order.first;
			if (type == "top")
			{
				m_TopFormationAmount = order.second;
				continue;
			}
			if (type == "top1")
			{
				m_Top1FormationAmount = order.second;
				continue;
			}
			if (type == "left")
			{
				m_LeftFormationAmount = order.second;
				continue;
			}
			if (type == "right")
			{
				m_RightFormationAmount = order.second;
				continue;
			}
		}
	}

	jsonReaderCP->ClearJSONFile();

	GetEnemyData("bees", m_vBees);
	GetEnemyData("butterflies", m_vButterflies);
	GetEnemyData("galagas", m_vGalagas);
}

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

AI_FormationCP::~AI_FormationCP()
{

}

void AI_FormationCP::Update(const float deltaTime)
{
	switch (m_FormationState)
	{
	case AI_FormationCP::FormationState::setupFormation:
		MoveIntoFormation(deltaTime);
		break;
	case AI_FormationCP::FormationState::attacking:
		m_vBees.clear();
		m_vGalagas.clear();
		m_vButterflies.clear();
		break;

	}
}

// Activate enemies to move into the formation progresively
void AI_FormationCP::MoveIntoFormation(const float deltaTime)
{
	m_CurrentTimeSpawn += deltaTime;

	if (m_CurrentTimeSpawn >= m_TimeEnemySpawn)
	{
		m_CurrentTimeSpawn = 0.f;


		switch (m_MovingFTState)
		{
		case AI_FormationCP::MovingFormationState::top:
		{
			// Activate a bee and a buttefly enemy

			ActivateEnemy(m_vBees, m_BeesActive);
			ActivateEnemy(m_vButterflies, m_ButterfliesActive);

			if (m_BeesActive == m_TopFormationAmount && m_ButterfliesActive == m_TopFormationAmount)
			{
				// This batch is finished -> Activate the next batch of enemies
				m_MovingFTState = MovingFormationState::left;
			}

			break;
		}
		case AI_FormationCP::MovingFormationState::left:
		{
			// Left always will alternate between the type of enemies to be spawned
			if (m_SpawnFirstType == true)
			{
				ActivateEnemy(m_vGalagas, m_GalagasActive);
				m_SpawnFirstType = false;

			}
			else
			{
				ActivateEnemy(m_vButterflies, m_ButterfliesActive);
				m_SpawnFirstType = true;

			}

			if ((m_GalagasActive == m_LeftFormationAmount) &&
				m_ButterfliesActive == m_TopFormationAmount + m_LeftFormationAmount)
			{
				m_MovingFTState = MovingFormationState::right;
			}

			break;
		}
		case AI_FormationCP::MovingFormationState::right:
		{
			ActivateEnemy(m_vButterflies, m_ButterfliesActive);
			if (m_ButterfliesActive == m_TopFormationAmount + m_LeftFormationAmount + m_RightFormationAmount)
			{
				m_MovingFTState = MovingFormationState::backToTop;
			}
			break;
		}
		case AI_FormationCP::MovingFormationState::backToTop:
		{
			ActivateEnemy(m_vBees, m_BeesActive);
			if (m_BeesActive == m_TopFormationAmount + m_LeftFormationAmount + m_RightFormationAmount
				+ (m_Top1FormationAmount * 2))
			{
				m_FormationState = FormationState::attacking;
			}
			break;
		}

		}
	}
}

// Activate the corresponding enemy from the formation
void AI_FormationCP::ActivateEnemy(std::vector<engine::GameObject*> enemies, unsigned short& currentActive)
{
	if (currentActive < int(enemies.size()))
	{
		enemies[currentActive]->SetIsActive(true);
		currentActive++;
	}
}

void AI_FormationCP::ReceiveMessage(const std::string&, const std::string&)
{

}