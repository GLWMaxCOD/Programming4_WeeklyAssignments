#ifndef GAME_ENGINE_SCENE
#define GAME_ENGINE_SCENE

#include "SceneManager.h"

namespace engine
{
	class GameObject;
	class CollisionComponent;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void AddAll(std::vector < std::shared_ptr<GameObject>>& m_objects);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveDeadObjects();
		void RemoveAll();
		void MarkAsDeadGameObject(const engine::GameObject* pGameObjectToMark);
		void MarkAsDeadGameObject(const std::shared_ptr<GameObject>& pGameObjectToMark);

		void Update(const float deltaTime);
		void Render() const;

		void CheckCollisions(size_t objectIdx1, engine::CollisionComponent* pCollisionCP);

		engine::GameObject* FindGameObjectByTag(const std::string& tag, bool IsActive = true);

		std::vector < std::shared_ptr<engine::GameObject>>& GetAll();

		const std::string& Name() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter;
	};

}

#endif
