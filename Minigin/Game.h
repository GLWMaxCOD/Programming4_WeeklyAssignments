#pragma once

#include <memory>

#include "TextureObject.h"
#include "TextObject.h"

namespace dae
{
	class Renderer;
	class SceneManager;
	class Scene;

	class Game final
	{
	public:

		Game();
		~Game();
		//void load();
		void Update([[maybe_unused]] const float deltaTime);
		void Render() const;

	private:

		Renderer* m_pRenderer;
		SceneManager* m_pSceneManager;
		Scene* m_pScene;
		std::shared_ptr<TextureObject> go;
		std::shared_ptr<TextObject> to;
		std::shared_ptr<TextObject> fpsObject;
	};
}