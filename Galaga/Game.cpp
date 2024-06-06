#include "Game.h"
#include "GameObject.h"
#include "GameCP.h"
#include "Scene.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "Servicelocator.h"
#include "SoundIDs.h"
#include "GalagaStrings.h"
#include <memory>
#include <iostream>

Game::Game()
{
	Initialize();
}

void Game::Initialize()
{
	// Create all scenes and set the First_Stage as the first scene
	auto& sceneManager = engine::SceneManager::GetInstance();
	sceneManager.CreateScene(FIRST_STAGE);
	sceneManager.CreateScene(SECOND_STAGE);
	sceneManager.CreateScene(THIRD_STAGE);
	sceneManager.SetActiveScene(FIRST_STAGE);

	auto& scene = sceneManager.GetActiveScene();

	auto go_GAME = std::make_shared<engine::GameObject>(nullptr, "GAME", glm::vec3{ 0.f, 0.f, 0.f }, glm::vec2{ 2.f, 2.f });
	go_GAME->AddComponent<GameCP>(go_GAME.get());

	scene.Add(go_GAME);

	AddAllSounds();
}

// Add all the sounds that our game will need
void Game::AddAllSounds()
{
	auto& soundSystem = engine::Servicelocator::Get_Sound_System();

	// Register sounds ID with its path (dont load them yet)
	soundSystem.RegisterSoundID(short(Sounds::startSound), "../Data/Sounds/Start.mp3", 80);
	soundSystem.RegisterSoundID(short(Sounds::playerFire), "../Data/Sounds/PlayerShoot.wav", 15);
	soundSystem.RegisterSoundID(short(Sounds::enemyDie), "../Data/Sounds/EnemyDies.wav", 15);
	soundSystem.RegisterSoundID(short(Sounds::tractorBeam), "../Data/Sounds/TractorBeam.mp3", 50);
	soundSystem.RegisterSoundID(short(Sounds::galagaDive), "../Data/Sounds/galagaDive.mp3", 30);
	soundSystem.RegisterSoundID(short(Sounds::playerDeath), "../Data/Sounds/PlayerDies.mp3", 50);
	soundSystem.RegisterSoundID(short(Sounds::confirm), "../Data/Sounds/Confirm.mp3", 100);
}

Game::~Game()
{
	std::cout << "Game destructor" << std::endl;
}