#include "AI_ButterflyCP.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "MissileManagerCP.h"
#include "EnemyCP.h"
#include "Scene.h"
#include <random>

AI_ButterflyCP::AI_ButterflyCP(engine::GameObject* pOwner)
	: Component("AI_ButterflyCP", pOwner),
	m_pEnemyCP{ nullptr }, m_pMoveCP{ nullptr }, m_pButterflyTransfCP{ nullptr },
	m_AttackState{ AttackState::breakFormation },
	m_DiagonalDiveMaxTime{ 0.f }, m_ElapsedSec{ 0.f }, m_AtRightSide{ false }, m_Direction{ 1.f, 1.f, 0.f },
	m_MaxSteeringTime{ 0.f }
{
	if (pOwner != nullptr)
	{
		m_pButterflyTransfCP = pOwner->GetComponent<engine::TransformComponent>();
		m_pMoveCP = pOwner->GetComponent<MoveComponent>();
		m_pEnemyCP = pOwner->GetComponent<EnemyCP>();
	}
}

AI_ButterflyCP::~AI_ButterflyCP()
{

}

void AI_ButterflyCP::Update(const float deltaTime)
{
	if (m_pMoveCP != nullptr && m_pEnemyCP != nullptr && m_pButterflyTransfCP != nullptr)
	{
		if (m_pEnemyCP->GetCurrentState() == EnemyCP::ENEMY_STATE::attack)
		{
			UpdateAttack(deltaTime);
		}
	}
}


void AI_ButterflyCP::UpdateAttack(const float deltaTime)
{
	auto currentPos = m_pButterflyTransfCP->GetWorldPosition();
	auto window = engine::SceneManager::GetInstance().GetSceneWindow();
	switch (m_AttackState)
	{
	case AI_ButterflyCP::AttackState::breakFormation:
		InitAttackData(currentPos, window);
		break;
	case AI_ButterflyCP::AttackState::diagonalDive:
		UpdateDiagonalDive(deltaTime, currentPos, window);
		break;

	case AI_ButterflyCP::AttackState::zigZagSteer:
		UpdateZigZagSteer(deltaTime, currentPos, window);
		break;
	}
}

// Init everything need for the Attack state
void AI_ButterflyCP::InitAttackData(const glm::vec3& currentPos, const engine::Window& window)
{
	if (currentPos.x > window.width / 2.f)
	{
		// Move to the left
		m_Direction.x = -1;
		m_AtRightSide = false;
	}
	else
	{
		m_Direction.x = 1;
		m_AtRightSide = true;
	}

	m_DiagonalDiveMaxTime = float(((std::rand()) / float(RAND_MAX / 1.f)) + 1.f);
	m_AttackState = AttackState::diagonalDive;

	if (m_pEnemyCP != nullptr)
	{
		// Calculate the missiles direction in order to fire 
		m_pEnemyCP->CalculateMissileDirection();
	}
}

void AI_ButterflyCP::UpdateDiagonalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window)
{
	// Downward Diagonal movement until max Time or before it leaves window boundaries
	if (m_ElapsedSec < m_DiagonalDiveMaxTime && (currentPos.x > 0 && currentPos.x < window.width))
	{
		m_ElapsedSec += deltaTime;
		m_pMoveCP->Move(deltaTime, m_Direction);
	}
	else
	{
		m_ElapsedSec = 0.f;
		m_AttackState = AttackState::zigZagSteer;

	}
}

void AI_ButterflyCP::UpdateZigZagSteer(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window)
{
	if (currentPos.y > window.height)
	{
		// Reset enemy position at the top of the screen when he leaves from the bottom		
		float formationXPos = m_pEnemyCP->GetFormationPos().x;
		float halfWindow{ window.width / 2 };
		if (formationXPos > halfWindow)
		{
			// Reset at the top right 
			m_pButterflyTransfCP->SetLocalPosition(glm::vec3{ halfWindow + 50.f, -150.f, 0.f });
		}
		else
		{
			// Reset at the top left 
			m_pButterflyTransfCP->SetLocalPosition(glm::vec3{ halfWindow - 50.f, -150.f, 0.f });
		}

		// Go back to his formation pos
		m_pEnemyCP->ChangeCurrentState(EnemyCP::ENEMY_STATE::moveToFormation);
		m_AttackState = AttackState::breakFormation;
	}
	else
	{
		if (m_ElapsedSec == 0)
		{
			// Everytime the steer right or left will take different amount of time
			m_MaxSteeringTime = float(((std::rand()) / float(RAND_MAX / 1)) + 0.5);

			// Steer in the opposite direction than last time
			if (m_Direction.x > 0.f)
			{
				// Steer left
				m_Direction.x = -1.f;
			}
			else
			{
				// Steer right
				m_Direction.x = 1.f;
			}
		}

		// Steer left and right until reaching the bottom limit
		m_ElapsedSec += deltaTime;
		if (m_ElapsedSec > m_MaxSteeringTime)
		{
			m_ElapsedSec = 0;
		}
		else
		{
			// Move in the corresponding direction
			m_pMoveCP->Move(deltaTime, m_Direction);
		}
	}
}

void AI_ButterflyCP::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TransformCP")
		{
			m_pButterflyTransfCP = nullptr;
		}
		if (value == "MoveCP")
		{
			m_pMoveCP = nullptr;
		}
	}
}