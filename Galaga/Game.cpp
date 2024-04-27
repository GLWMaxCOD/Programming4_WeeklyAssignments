#include "Game.h"
#include "GameIncludes.h"
#include <iostream>

Game::Game(const engine::Window& window)
	: FIRST_STAGE{ "FirstStage " },
	SECOND_STAGE{ "SecondStage " },
	THIRD_STAGE{ "ThirdStage " },
	PLAYER_TAG{ "Player" },
	ENEMY_TAG{ "Enemy" },
	LEVEL_TAG{ "Level" },
	UI_TAG{ "UI" },
	m_Window{ window }
{
	Initialize();
}

void Game::Initialize()
{
	// Setup all available scenes and the active scene
	auto& sceneManager = engine::SceneManager::GetInstance();
	sceneManager.CreateScene(FIRST_STAGE);
	sceneManager.CreateScene(SECOND_STAGE);
	sceneManager.CreateScene(THIRD_STAGE);
	sceneManager.SetActiveScene(FIRST_STAGE);

	auto& scene = sceneManager.GetActiveScene();

	SetupSounds();
	SetupBackground();

	// PLAYER 1
	glm::vec3 startPos{ m_Window.width / 2.f, m_Window.height / 1.15f, 0.f };
	auto go_Player = std::make_shared<engine::GameObject>(nullptr, PLAYER_TAG, startPos, glm::vec2{ 2.f, 2.f });
	go_Player->AddComponent<PlayerCP>(go_Player.get(), 1, glm::vec2{ m_Window.width, m_Window.height });
	scene.Add(go_Player);

	// ENEMIES
	glm::vec3 startPos2{ 300.f, 200.f, 0.f };
	auto go_Enemy = std::make_shared<engine::GameObject>(nullptr, ENEMY_TAG, startPos2, glm::vec2{ 2.f, 2.f });
	go_Enemy->AddComponent<EnemyCP>(go_Enemy.get(), 1);
	scene.Add(go_Enemy);

	glm::vec3 startPos3{ 250.f, 200.f, 0.f };
	auto go_Enemy2 = std::make_shared<engine::GameObject>(nullptr, ENEMY_TAG, startPos3, glm::vec2{ 2.f, 2.f });
	go_Enemy2->AddComponent<EnemyCP>(go_Enemy2.get(), 1);
	scene.Add(go_Enemy2);

	glm::vec3 startPos4{ 150.f, 200.f, 0.f };
	auto go_Enemy3 = std::make_shared<engine::GameObject>(nullptr, ENEMY_TAG, startPos4, glm::vec2{ 2.f, 2.f });
	go_Enemy3->AddComponent<EnemyCP>(go_Enemy3.get(), 1);
	scene.Add(go_Enemy3);

	glm::vec3 startPos5{ 100.f, 200.f, 0.f };
	auto go_Enemy4 = std::make_shared<engine::GameObject>(nullptr, ENEMY_TAG, startPos5, glm::vec2{ 2.f, 2.f });
	go_Enemy4->AddComponent<EnemyCP>(go_Enemy4.get(), 1);
	scene.Add(go_Enemy4);

	glm::vec3 startPos6{ 350.f, 200.f, 0.f };
	auto go_Enemy5 = std::make_shared<engine::GameObject>(nullptr, ENEMY_TAG, startPos6, glm::vec2{ 2.f, 2.f });
	go_Enemy5->AddComponent<EnemyCP>(go_Enemy5.get(), 1);
	scene.Add(go_Enemy5);

	glm::vec3 startPos7{ 200.f, 200.f, 0.f };
	auto go_Enemy6 = std::make_shared<engine::GameObject>(nullptr, ENEMY_TAG, startPos7, glm::vec2{ 2.f, 2.f });
	go_Enemy6->AddComponent<EnemyCP>(go_Enemy6.get(), 1);
	scene.Add(go_Enemy6);

	// How to play text
	auto go_HowToPlayText = std::make_shared<engine::GameObject>(nullptr, UI_TAG, glm::vec3{ 40, 20, 0 });
	go_HowToPlayText->AddComponent<engine::RenderComponent>(go_HowToPlayText.get());
	auto font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	go_HowToPlayText->AddComponent<TextComponent>(go_HowToPlayText.get(), "Press Space to shoot and A/D keys to move ", font);

	scene.Add(go_HowToPlayText);
}

// Draw two backgrounds one on top of each other to give a scrolling parallax effect
void Game::SetupBackground()
{
	auto& scene = engine::SceneManager::GetInstance().GetActiveScene();

	glm::vec2 backgroundScale{ 0.3f, 0.32f };
	// Background 1
	auto go_Background1 = std::make_shared<engine::GameObject>(nullptr, LEVEL_TAG, glm::vec3{ 0.f, 0.f, 0.f },
		backgroundScale);
	go_Background1->AddComponent<engine::RenderComponent>(go_Background1.get(), "background.png");
	go_Background1->AddComponent<ParallaxScrollingCP>(go_Background1.get());
	scene.Add(go_Background1);

	// Background 2 (on top of background1 at start)
	auto backSize = go_Background1->GetComponent<engine::RenderComponent>()->GetTextureSize();
	glm::vec3 background2StartPos{ go_Background1->GetWorldPosition() - glm::vec3{0, backSize.y, 0.f} };

	auto go_Background2 = std::make_shared<engine::GameObject>(nullptr, LEVEL_TAG, background2StartPos,
		backgroundScale);
	go_Background2->AddComponent<engine::RenderComponent>(go_Background2.get(), "background.png");
	go_Background2->AddComponent<ParallaxScrollingCP>(go_Background2.get());
	scene.Add(go_Background2);
}

void Game::SetupSounds()
{
	auto& soundSystem = engine::Servicealocator::Get_Sound_System();

	// Register sounds ID with its path (dont load them yet)
	soundSystem.RegisterSoundID(short(Sounds::startSound), "../Data/Start.wav", 50);
	soundSystem.RegisterSoundID(short(Sounds::playerFire), "../Data/PlayerShoot.wav", 50);
	soundSystem.RegisterSoundID(short(Sounds::enemyDie), "../Data/EnemyDies.wav", 70);
}

Game::~Game()
{
	std::cout << "Game destructor" << std::endl;
}