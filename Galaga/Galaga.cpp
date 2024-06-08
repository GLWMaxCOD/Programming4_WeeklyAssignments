#include <SDL.h>

#if _DEBUG
// Include Visual Leak Detector if available (debug build only)
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "Game.h"
#include "structs.h"
#include <memory>
#include <ctime>

int main(int, char* [])
{
    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Define the window properties
    engine::Window window{ "Galaga", 560.f , 640.f };

    // Initialize the engine with the data path and window properties
    engine::Engine engine("../Data/", window);

    // Create the game instance
    std::unique_ptr<Game> game = std::make_unique<Game>();

    // Run the engine (main game loop)
    engine.Run();

    return 0;
}