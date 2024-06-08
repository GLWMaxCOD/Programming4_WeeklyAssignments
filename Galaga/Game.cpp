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

// Constructor: Initializes the game by setting up scenes and sounds
Game::Game()
{
    Initialize();
}

// Destructor: Clean up resources if needed (currently no additional cleanup)
Game::~Game()
{
    // std::cout << "Game destructor" << std::endl;
}

// Initialize game scenes and set the initial active scene
void Game::Initialize()
{
    auto& sceneManager = engine::SceneManager::GetInstance();

    // Create game scenes
    sceneManager.CreateScene(FIRST_STAGE);
    sceneManager.CreateScene(SECOND_STAGE);
    sceneManager.CreateScene(THIRD_STAGE);
    sceneManager.CreateScene(FOURTH_STAGE);

    // Set the initial active scene
    sceneManager.SetActiveScene(FIRST_STAGE);

    auto& scene = sceneManager.GetActiveScene();

    // Create a GameObject to manage game state and add it to the scene
    auto go_GAME = std::make_shared<engine::GameObject>(nullptr, "GAME", glm::vec3{ 0.f, 0.f, 0.f }, glm::vec2{ 2.f, 2.f });
    go_GAME->AddComponent<GameCP>(go_GAME.get());

    scene.Add(go_GAME);

    // Register all necessary sounds
    AddAllSounds();
}

// Register all the sounds required for the game
void Game::AddAllSounds()
{
    auto& soundSystem = engine::Servicelocator::Get_Sound_System();

    // Register sound IDs with their corresponding file paths and volume levels
    soundSystem.RegisterSoundID(short(Sounds::startSound), "../Data/Sounds/Start.mp3", 80);
    soundSystem.RegisterSoundID(short(Sounds::playerFire), "../Data/Sounds/PlayerShoot.wav", 15);
    soundSystem.RegisterSoundID(short(Sounds::enemyDie), "../Data/Sounds/EnemyDies.wav", 15);
    soundSystem.RegisterSoundID(short(Sounds::tractorBeam), "../Data/Sounds/TractorBeam.mp3", 50);
    soundSystem.RegisterSoundID(short(Sounds::galagaDive), "../Data/Sounds/galagaDive.mp3", 30);
    soundSystem.RegisterSoundID(short(Sounds::playerDeath), "../Data/Sounds/PlayerDies.mp3", 50);
    soundSystem.RegisterSoundID(short(Sounds::confirm), "../Data/Sounds/Confirm.mp3", 100);
}