#include "HealthComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "LivesUIComponent.h"
#include "Observer.h"
#include <iostream>

HealthComponent::HealthComponent(engine::GameObject* pOwner, unsigned int lives)
	: Component("HealthCP", pOwner)
	, m_Lives{ lives }
{
}

HealthComponent::~HealthComponent()
{
	std::cout << "Health Component destructor" << std::endl;
}

void HealthComponent::ResetHealth(unsigned int newHealth)
{
	m_Lives = newHealth;
	if (m_Lives > 0)
	{
		//engine::Event HealthResetEvent{ "HealthReset" };
		//m_HealthSubject->NotifyObservers(GetOwner(), HealthResetEvent);
	}
}

// Add an observer to "Observe" the HealthComponent
void HealthComponent::AddObserver(engine::Observer* pObserver)
{
	if (m_HealthSubject == nullptr)
	{
		m_HealthSubject = std::make_unique<engine::Subject>();
	}

	m_HealthSubject->AddObserver(pObserver);
}

void HealthComponent::Update([[maybe_unused]] const float deltaTime)
{

}

void HealthComponent::DecrementHealth(unsigned int amount)
{
	if (m_Lives > 0 && amount > 0)
	{
		if (amount >= m_Lives)
		{
			m_Lives = 0;

			if (m_HealthSubject != nullptr)
			{
				engine::Event dieEvent{ "GameObjectDied" };
				m_HealthSubject->NotifyObservers(GetOwner(), dieEvent);
			}
		}
		else
		{
			m_Lives -= amount;
		}

		if (m_HealthSubject != nullptr)
		{
			engine::Event HealthDecEvent{ "HealthDecremented" };
			m_HealthSubject->NotifyObservers(GetOwner(), HealthDecEvent);
		}
	}
}

void HealthComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

const unsigned int HealthComponent::GetLives() const
{
	return m_Lives;
}