#ifndef GAME_ENGINE_GAME
#define GAME_ENGINE_GAME

#include <string>

// Main Game class that handles initialization and setting up game scenes and sounds
class Game final
{
public:
    Game();
    ~Game();
    Game(const Game& other) = delete;
    Game(Game&& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game& operator=(Game&& other) = delete;

private:
    // Initialize game components and scenes
    void Initialize();

    // Register all sounds used in the game
    void AddAllSounds();
};

#endif