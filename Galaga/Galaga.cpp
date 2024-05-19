#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "Game.h"
#include "structs.h"
#include <memory>

int main(int, char* [])
{
	// Seed the random number generator with the current time
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	engine::Window window{ "Galaga", 560.f , 640.f };
	engine::Engine engine("../Data/", window);

	std::unique_ptr<Game> game = std::make_unique<Game>();
	engine.Run();

	return 0;
}