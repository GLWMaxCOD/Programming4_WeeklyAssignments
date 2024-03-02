#include "Scene.h"
#include "GameObject.h"
#include <iostream>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

// Remove all "dead" objects after update
void Scene::RemoveDeadObjects()
{
	for (auto& object : m_objects)
	{
		if (object->IsMarkedAsDead())
		{
			//object.reset();
			std::cout << object.use_count() << std::endl;
			object->Destroy();
			Remove(object);
		}
	}
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update([[maybe_unused]] const float deltaTime)
{
	bool isADeadObject = false;
	for (auto& object : m_objects)
	{
		if (!object->IsMarkedAsDead())
		{
			object->Update(deltaTime);
		}
		else
		{
			// Object needs to be delete it after updating all objects
			isADeadObject = true;
		}
	}

	if (isADeadObject)
	{
		// At least one game object is marked as "Dead" and needs to be removed 
		RemoveDeadObjects();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

