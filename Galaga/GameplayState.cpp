#include "GameplayState.h"
#include "GameObject.h"
#include "PlayerCP.h"
#include "Player2VersusCP.h"
#include "PlayerInputCP.h"
#include "Scene.h"
#include "FormationCP.h"
#include "AI_FormationCP.h"
#include "AI_GalagaCP.h"
#include "AI_ButterflyCP.h"
#include "GalagaStrings.h"
#include "GameOverState.h"
#include "Servicelocator.h"
#include "SoundIDs.h"
#include "TextComponent.h"
#include "SkipStageCommand.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include <iostream>
#include <cstdlib> // For std::rand

GameplayState::GameplayState(const std::string& gameMode)
	:m_GameMode{ gameMode }, m_pFormationCP{ nullptr }, m_vEnemiesData{ },
	m_IsGameOver{ false }, m_pPlayer1{ nullptr }, m_IsChangingState{ true }, m_ElapsedTime{ 0.f },
	MAX_TIME_CHANGE{ 3.f }, m_CurrentStage{ FIRST_STAGE }
{
	// Load all data paths 
	LoadDataPaths();
}

void GameplayState::LoadDataPaths()
{
	FormationJsonData formationData("Data/Formations/FormationStage1.json", "Data/Formations/FormationStage1-Order.json");
	std::pair stageData{ std::make_pair(FIRST_STAGE, formationData) };
	m_vEnemiesData.emplace_back(stageData);

	formationData.changeData("Data/Formations/FormationStage2.json", "Data/Formations/FormationStage2-Order.json");
	stageData.first = SECOND_STAGE;
	stageData.second = formationData;
	m_vEnemiesData.emplace_back(stageData);

	formationData.changeData("Data/Formations/FormationStage3.json", "Data/Formations/FormationStage3-Order.json");
	stageData.first = THIRD_STAGE;
	stageData.second = formationData;
	m_vEnemiesData.emplace_back(stageData);

	formationData.changeData("Data/Formations/FormationStage4.json", "Data/Formations/FormationStage4-Order.json");
	stageData.first = FOURTH_STAGE;
	stageData.second = formationData;
	m_vEnemiesData.emplace_back(stageData);
}

GameplayState::~GameplayState()
{
	//std::cout << "Gameplay State destructor" << std::endl;
}

void GameplayState::OnEnter()
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();

	// CURRENT STAGE INFO TEXT
	auto galaga_Font = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 18);
	SDL_Color blueColor = { 0,255, 222 };
	glm::vec3 stageInfoPos{ (window.width / 2.f) - 60.f, (window.height / 2.f) - 80.f, 0.f };
	m_StageInfoText = std::make_shared<engine::GameObject>(nullptr, "UI", stageInfoPos);
	m_StageInfoText->AddComponent<engine::RenderComponent>(m_StageInfoText.get());
	m_StageInfoText->AddComponent<TextComponent>(m_StageInfoText.get(), FIRST_STAGE, galaga_Font, blueColor);
	m_StageInfoText->SetIsActive(false);
	scene.Add(m_StageInfoText);

	auto& soundSystem = engine::Servicelocator::Get_Sound_System();
	soundSystem.PlaySound(short(Sounds::startSound));

	if (m_GameMode == "1 PLAYER" || m_GameMode == "2 PLAYERS" || m_GameMode == "VERSUS")
	{
		InitPlayer1();
		InitEnemies();
		if (m_GameMode == "2 PLAYERS")
		{
			// Add Player 2 too ( Player 2 will use the available controller)
			InitPlayer2();
		}
		if (m_GameMode == "VERSUS")
		{
			InitPlayer2Versus(); // Change into a new function where it disables the AI of one Galaga enemy and gives it extra bindings
		}
	}

}

void GameplayState::InitEnemies()
{
	auto& scene = engine::SceneManager::GetInstance().GetActiveScene();

	// FORMATION FOR ALL ENEMIES
	// Formation GO will be the one who creates all enemies with the correct info
	auto go_Formation = std::make_shared<engine::GameObject>(nullptr, STR_LEVEL_TAG, glm::vec3{ 0.f, 0.f, 0.f });
	m_pFormationCP = go_Formation->AddComponent<FormationCP>(go_Formation.get(), m_vEnemiesData.at(0).second.GetEnemiesData());
	go_Formation->AddComponent<AI_FormationCP>(go_Formation.get(), m_vEnemiesData.at(0).second.GetSpawnOrderData());
	scene.Add(go_Formation);

	// Assign specific butterflies to specific Galagas
	auto& galagas = m_pFormationCP->GetEnemies("galagas");
	auto& butterflies = m_pFormationCP->GetEnemies("butterflies");

	if (galagas.size() >= 4 && butterflies.size() >= 16)
	{
		for (int i = 0; i < 4; ++i)
		{
			auto galaga = galagas[i];
			auto butterflyLeft = butterflies[8 + (i * 2)];
			auto butterflyRight = butterflies[9 + (i * 2)];

			auto galagaAI = galaga->GetComponent<AI_GalagaCP>();
			auto butterflyLeftAI = dynamic_cast<AI_ButterflyCP*>(butterflyLeft->GetComponent<AI_ButterflyCP>());
			auto butterflyRightAI = dynamic_cast<AI_ButterflyCP*>(butterflyRight->GetComponent<AI_ButterflyCP>());

			if (galagaAI && butterflyLeftAI && butterflyRightAI)
			{
				galagaAI->SetEscortButterflies(butterflyLeftAI, butterflyRightAI);
				galagaAI->ReplaceDeadEscorts(butterflies);
			}
		}
	}
}

void GameplayState::InitPlayer1()
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();
	auto& input = engine::InputManager::GetInstance();

	// Add the PlayerCP to Player gameObject and the input for the Gameplay
	auto pPlayer1GO = scene.FindGameObjectByTag(STR_PLAYER_TAG);
	if (pPlayer1GO != nullptr)
	{
		// Bind a command to be able to Skip levels with the keyboard 
		SDL_KeyCode key_F1{ SDLK_F1 };
		std::unique_ptr<Command> skipLevelCommand = std::make_unique<SkipStageCommand>(this);
		input.BindCommand(std::move(skipLevelCommand), key_F1, engine::InputType::Up);

		m_pPlayer1 = pPlayer1GO;
		m_vPlayers.emplace_back(pPlayer1GO);
		pPlayer1GO->AddComponent<PlayerCP>(pPlayer1GO, 4, 1, glm::vec2{ window.width, window.height });

		auto pPlayerInputCP = pPlayer1GO->GetComponent<PlayerInputCP>();
		if (pPlayerInputCP != nullptr && (m_GameMode == "1 PLAYER" || m_GameMode == "VERSUS"))
		{
			// Player 1 can play with Keyboard and Controller
			pPlayerInputCP->GameplayControllerInput(unsigned int(0));
			pPlayerInputCP->GameplayKeyboardInput();
			return;
		}
	}
}
void GameplayState::InitPlayer2()
{

	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();

	glm::vec3 startPos{ (window.width / 2.f) + 50.f, window.height / 1.15f, 0.f };
	auto go_Player2 = std::make_shared<engine::GameObject>(nullptr, STR_PLAYER_TAG, startPos, glm::vec2{ 2.f, 2.f });
	go_Player2->AddComponent<PlayerCP>(go_Player2.get(), 4, 2, glm::vec2{ window.width, window.height });
	auto pPlayer2InputCP = go_Player2->AddComponent<PlayerInputCP>(go_Player2.get());
	pPlayer2InputCP->TwoPlayersGameplayInput();

	auto pPlayer1 = scene.FindGameObjectByTag(STR_PLAYER_TAG);
	if (pPlayer1 != nullptr)
	{
		// Player 2 will have the controller 1, so Player 1 will control the other controller 
		// if there is any available
		auto pPlayerInputCP = pPlayer1->GetComponent<PlayerInputCP>();
		if (pPlayerInputCP != nullptr)
		{
			pPlayerInputCP->TwoPlayersGameplayInput();
		}
	}

	scene.Add(go_Player2);
	m_vPlayers.emplace_back(go_Player2.get());
}

void GameplayState::InitPlayer2Versus()
{
	// Retrieve Galaga enemies
	auto& galagas = m_pFormationCP->GetEnemies("galagas");

	if (galagas.empty())
	{
		std::cerr << "No Galaga enemies found in the formation." << std::endl;
		return;
	}

	engine::GameObject* pGalaga = galagas.front();

	// Set the first Galaga to formation only and allow it to move within the formation
	auto pAIComponent = pGalaga->GetComponent<AI_GalagaCP>();
	if (pAIComponent)
	{
		pAIComponent->SetFormationOnly(true);
		pAIComponent->SetVersusMode(true);  // Set versus mode
	}

	// Assign Player2VersusCP component
	auto pPlayer2VersusCP = pGalaga->AddComponent<Player2VersusCP>(pGalaga, 2); // Assuming player index 2
	pPlayer2VersusCP->SetupControls(); // Setup controls specific to Versus mode

	// Add the Galaga enemy to the players vector to track its status
	m_vPlayers.emplace_back(pGalaga);
}

bool GameplayState::NextStage()
{
	if (m_pFormationCP != nullptr)
	{
		auto& sceneManager = engine::SceneManager::GetInstance();
		if (sceneManager.LoadNextScene())
		{
			// New scene loaded succesfully -> Reset enemies data
			for (size_t enemiesIdx{ 0 }; enemiesIdx < m_vEnemiesData.size(); ++enemiesIdx)
			{
				auto enemiesData = m_vEnemiesData.at(enemiesIdx);
				if (enemiesData.first == sceneManager.GetActiveSceneName())
				{
					// Get the corresponding data from this new stage
					m_pFormationCP->Reset(enemiesData.second.GetEnemiesData(), enemiesData.second.GetSpawnOrderData());
					return true;
				}
			}
		}

	}
	// No more scenes or no data found 
	return false;
}

void GameplayState::SkipStage()
{
	m_pFormationCP->DeactivateAllEnemies();
	m_ElapsedTime = 0;
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto nextSceneName = sceneManager.GetNextSceneName();
	if (nextSceneName != "No more scenes")
	{
		m_CurrentStage = nextSceneName;
		m_StageInfoText->GetComponent<TextComponent>()->SetText(nextSceneName);
		m_IsChangingState = true;
	}
}

void GameplayState::OnExit()
{
	// Remove stage text
	m_StageInfoText->MarkAsDead();

	// Unbind the skipStage command
	auto& input = engine::InputManager::GetInstance();
	input.UnbindAllCommands();
}

GameState* GameplayState::GetChangeState()
{
	if (m_IsGameOver)
	{
		//OnExit();
		return new GameOverState();
	}

	return nullptr;
}

void GameplayState::UpdateState(const float deltaTime)
{
	if (m_IsChangingState)
	{
		UpdateChangingStage(deltaTime);
	}
	else if (m_pFormationCP != nullptr)
	{
		if (ArePlayersAlive())
		{
			// Player(s) still alive -> Check if there are enemies left
			if (!m_pFormationCP->AreEnemiesLeft())
			{
				if (m_IsChangingState == false)
				{
					// wait a few seconds before loading enemies from next stage
					auto& sceneManager = engine::SceneManager::GetInstance();
					if (sceneManager.GetNextSceneName() != "No more scenes")
					{
						m_StageInfoText->GetComponent<TextComponent>()->SetText(sceneManager.GetNextSceneName());
						m_IsChangingState = true;
					}
					else
					{
						// No more scenes to load
						m_IsGameOver = true;
					}
					return;
				}
			}
		}
		else
		{
			m_IsGameOver = true;
		}

	}
}

bool GameplayState::ArePlayersAlive()
{
	if (m_GameMode == "VERSUS")
	{
		if (m_pPlayer1 == nullptr || !m_pPlayer1->IsActive())
		{
			// End game if Player 1 is not active
			return false;
		}
	}

	for (const auto& player : m_vPlayers)
	{
		if (player != nullptr && player->IsActive())
		{
			// At least one player is still alive
			return true;
		}
	}
	return false;
}

void GameplayState::UpdateChangingStage(const float deltaTime)
{
	// Show stage number info
	m_StageInfoText->SetIsActive(true);
	m_ElapsedTime += deltaTime;

	if (m_ElapsedTime > MAX_TIME_CHANGE)
	{
		// When time over load enemies from the next stage
		m_StageInfoText->SetIsActive(false);
		m_IsChangingState = false;
		m_ElapsedTime = 0.f;
		if (m_CurrentStage == FIRST_STAGE)
		{
			// If first stage -> Enemies already loaded
			m_pFormationCP->SpawnEnemies();
			m_CurrentStage = SECOND_STAGE;
			return;
		}

		if (m_pFormationCP != nullptr)
		{
			// If nextStage returns false then we go the GameOver state
			if (!NextStage())
			{
				m_IsGameOver = true;
			}
			m_pFormationCP->SpawnEnemies();
		}

	}
}