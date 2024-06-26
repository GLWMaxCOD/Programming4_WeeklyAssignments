#ifndef GAME_ENGINE_ENGINE
#define GAME_ENGINE_ENGINE

#include <string>
#include <functional>

namespace engine
{
	struct Window;
	class Engine final
	{
	public:
		// Indicate where the data is and the window properties
		explicit Engine(const std::string& dataPath, const engine::Window& window);
		~Engine();
		void Run();

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;
	};
}

#endif