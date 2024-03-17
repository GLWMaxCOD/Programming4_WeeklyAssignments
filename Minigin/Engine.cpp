#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Engine.h"
#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Game.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "structs.h"

SDL_Window* g_window{};

using namespace std::chrono;

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
		printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Engine::Engine(const std::string& dataPath, const Window& window)
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		window.title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		int(window.width),
		int(window.height),
		SDL_WINDOW_OPENGL
	);

	if (g_window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Engine::~Engine()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Engine::Run(const std::function<Game*()>& loadGame)
{
	// The '()' will invoke the lambda function
	Game* pGame = loadGame();

	auto& input = InputManager::GetInstance();

	bool doContinue = true;

	// Limiting the FPS and reduce CPU usage :) / Human eye can't process FPS's higher then 120 about, varies by everyone
	const int TARGET_FRAME_RATE{ 60 };
	const int TARGET_FRAME_TIME{ 1000 / TARGET_FRAME_RATE };

	auto lastTime = std::chrono::high_resolution_clock::now();
	while (doContinue)
	{
		const auto currentTime = high_resolution_clock::now();
		const float deltaTime = duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		doContinue = input.ProcessInput();

		// TO DO : FixedUpdate to fix the CPU usage?
		// Update Game
		pGame->Update(deltaTime);

		// Render Game
		pGame->Render();
		
		const auto sleepTime = currentTime + milliseconds(TARGET_FRAME_TIME) - high_resolution_clock::now();
		if (sleepTime.count() > 0)
		{
			// Sleep to avoid going too fast and max our fps to 60
			std::this_thread::sleep_for(sleepTime);
		}

	}

	delete pGame;
}