﻿#include "FormationCP.h"
#include "GameObject.h"
#include "EnemyCP.h"
#include "AI_BeeCP.h"
#include "HealthComponent.h"
#include "FormationReaderCP.h"
#include "Scene.h"
#include <memory>
#include <iostream>

FormationCP::FormationCP(engine::GameObject* pOwner, const std::string& positionsJSONPath)
	:Component("FormationCP", pOwner),
	m_LeftLimitFormation{},
	m_RighttLimitFormation{}
{
	pOwner->AddComponent<FormationReaderCP>(pOwner);
	ReadFormationFromJSON(positionsJSONPath);

	auto window = engine::SceneManager::GetInstance().GetSceneWindow();
	m_LeftLimitFormation = 0 + 100.f;
	m_RighttLimitFormation = window.width - 100.f;
}

void FormationCP::ReadFormationFromJSON(const std::string& JSONPath)
{
	auto& scene = engine::SceneManager::GetInstance().GetActiveScene();
	auto jsonReaderCP = GetOwner()->GetComponent<FormationReaderCP>();

	m_vBees.clear();
	m_vButterflies.clear();
	m_vGalagas.clear();

	// Read all info about the enemies (startPos, formationPos etc) from JSON
	std::vector<std::pair<std::string, glm::vec3>> bees = jsonReaderCP->ReadFormation(JSONPath, "bees");
	std::vector<std::pair<std::string, glm::vec3>> butterflies = jsonReaderCP->ReadFormation(JSONPath, "butterflies");
	std::vector<std::pair<std::string, glm::vec3>> galagas = jsonReaderCP->ReadFormation(JSONPath, "galagas");

	glm::vec3 startPos{};

	auto ParentFormationPos = GetOwner()->GetComponent<engine::TransformComponent>()->GetWorldPosition();

	// Enemies will be owned by the Formation Game Object
	// Create Bee enemies
	for (const auto& pair : bees)
	{
		SetStartingPos(pair.first, startPos);

		glm::vec3 formationPos{ ParentFormationPos.x + pair.second.x, ParentFormationPos.y + pair.second.y ,
			ParentFormationPos.z + pair.second.z };

		auto go_BeeEnemy = std::make_shared<engine::GameObject>(nullptr, "Enemy", startPos, glm::vec2{ 2.f, 2.f });
		go_BeeEnemy->AddComponent<EnemyCP>(go_BeeEnemy.get(), "Sprites/Bee.png", formationPos, 1);
		go_BeeEnemy->AddComponent<AI_BeeCP>(go_BeeEnemy.get());
		go_BeeEnemy->GetComponent<HealthComponent>()->AddObserver(this);
		// Inactive at start
		go_BeeEnemy->SetIsActive(false);

		scene.Add(go_BeeEnemy);
		m_vBees.emplace_back(go_BeeEnemy);
	}

	// Butterfly enemies
	for (const auto& pair : butterflies)
	{
		SetStartingPos(pair.first, startPos);

		glm::vec3 formationPos{ ParentFormationPos.x + pair.second.x, ParentFormationPos.y + pair.second.y ,
			ParentFormationPos.z + pair.second.z };

		auto go_Butterfly = std::make_shared<engine::GameObject>(nullptr, "Enemy", startPos, glm::vec2{ 2.f, 2.f });
		go_Butterfly->AddComponent<EnemyCP>(go_Butterfly.get(), "Sprites/Butterfly.png", formationPos, 1);
		go_Butterfly->AddComponent<AI_BeeCP>(go_Butterfly.get());
		go_Butterfly->GetComponent<HealthComponent>()->AddObserver(this);
		// Inactive at start
		go_Butterfly->SetIsActive(false);

		scene.Add(go_Butterfly);
		m_vButterflies.emplace_back(go_Butterfly);
	}

	// Galaga enemies
	for (const auto& pair : galagas)
	{
		SetStartingPos(pair.first, startPos);

		glm::vec3 formationPos{ ParentFormationPos.x + pair.second.x, ParentFormationPos.y + pair.second.y ,
			ParentFormationPos.z + pair.second.z };

		auto go_Galagas = std::make_shared<engine::GameObject>(nullptr, "Enemy", startPos, glm::vec2{ 2.f, 2.f });
		go_Galagas->AddComponent<EnemyCP>(go_Galagas.get(), "Sprites/Galaga.png", formationPos, 2);
		go_Galagas->AddComponent<AI_BeeCP>(go_Galagas.get());
		go_Galagas->GetComponent<HealthComponent>()->AddObserver(this);
		// Inactive at start
		go_Galagas->SetIsActive(false);

		scene.Add(go_Galagas);
		m_vGalagas.emplace_back(go_Galagas);
	}

	// Done reading everything needed
	jsonReaderCP->ClearJSONFile();
}

// Set where will the enemies will start spawning at the start of the level
void FormationCP::SetStartingPos(const std::string& commingFrom, glm::vec3& startPos)
{
	auto window = engine::SceneManager::GetInstance().GetSceneWindow();
	if (commingFrom == "top")
	{
		startPos = glm::vec3{ (window.width / 2.f) - 20.f, -10, 0 };
	}
	else
	{
		if (commingFrom == "left")
		{
			startPos = glm::vec3{ -10, window.height / 2.f, 0 };
		}
		else if (commingFrom == "right")
		{
			startPos = glm::vec3{ window.width + 10.f , window.height / 2.f, 0 };
		}
		else
		{
			startPos = glm::vec3{ (window.width / 2.f) + 20.f, -10, 0 };
		}
	}
}

FormationCP::~FormationCP()
{

}

void FormationCP::Update(const float)
{

}

void FormationCP::ReceiveMessage(const std::string&, const std::string&)
{

}

void FormationCP::OnNotify(engine::GameObject*, const engine::Event& event)
{

	if (event.IsSameEvent("GameObjectDied"))
	{
		SearchForDeadEnemy();
	}
}

void FormationCP::SearchForDeadEnemy()
{
	bool foundEnemy = false;

	for (size_t beeGOIdx{ 0 }; beeGOIdx < m_vBees.size() && !foundEnemy;)
	{
		if (m_vBees[beeGOIdx]->IsMarkedAsDead())
		{
			// Found it
			// Kill the enemy first here and then, completely destroy the gameObject shared_ptr from the scene
			// after the Update is finished. So the enemy will be actually destroyed after the update is done, and not during
			m_vBees.erase(std::remove(m_vBees.begin(), m_vBees.end(), m_vBees[beeGOIdx]), m_vBees.end());

			std::cout << "(Shinda / dead)" << std::endl;
			break;
		}
		else
		{
			beeGOIdx++;
		}
	}

	for (size_t butterflyGOIdx{ 0 }; butterflyGOIdx < m_vButterflies.size() && !foundEnemy;)
	{
		if (m_vButterflies[butterflyGOIdx]->IsMarkedAsDead())
		{
			// Found it
			m_vButterflies.erase(std::remove(m_vButterflies.begin(), m_vButterflies.end(), m_vButterflies[butterflyGOIdx]), m_vButterflies.end());

			std::cout << "(Shinda / dead)" << std::endl;
			break;
		}
		else
		{
			butterflyGOIdx++;
		}
	}

	for (size_t galagaGOIdx{ 0 }; galagaGOIdx < m_vGalagas.size() && !foundEnemy;)
	{
		if (m_vGalagas[galagaGOIdx]->IsMarkedAsDead())
		{
			// Found it
			m_vBees.erase(std::remove(m_vBees.begin(), m_vBees.end(), m_vBees[galagaGOIdx]), m_vBees.end());

			std::cout << "(Shinda / dead)" << std::endl;
			break;
		}
		else
		{
			galagaGOIdx++;
		}
	}

}

std::vector< std::shared_ptr<engine::GameObject>> FormationCP::GetEnemies(const std::string& type)
{
	if (type == "bees")
	{
		return m_vBees;
	}

	if (type == "butterflies")
	{
		return m_vButterflies;
	}

	if (type == "galagas")
	{
		return m_vGalagas;
	}

	return m_vBees;

}