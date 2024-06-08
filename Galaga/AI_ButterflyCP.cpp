#include "AI_ButterflyCP.h"
#include "AI_GalagaCP.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "MissileManagerCP.h"
#include "EnemyCP.h"
#include "Scene.h"
#include "RotatorComponent.h"
#include <glm/gtc/constants.hpp>		// For pi definitions
#include <random>

AI_ButterflyCP::AI_ButterflyCP(engine::GameObject* pOwner)
	: Component("AI_ButterflyCP", pOwner),
	m_pEnemyCP{ nullptr }, m_pMoveCP{ nullptr }, m_pButterflyTransfCP{ nullptr }, m_pRotatorCP{ nullptr },
	m_AttackState{ AttackState::breakFormation },
	m_DiagonalDiveMaxTime{ 0.f }, m_ElapsedSec{ 0.f }, m_AtRightSide{ false }, m_Direction{ 1.f, 1.f, 0.f },
	m_MaxSteeringTime{ 0.f }, m_RotationTime{ 1.5f }, m_RotationRadius{ 30.f },
	m_OriginalFormationPos{ 0.f, 0.f, 0.f }, m_CurrentGalagaPos{ 0.f, 0.f, 0.f }, m_Offset{ 0.f }, m_IsFollowingGalaga{ false }, 
	m_EscortSpeedMultiplier{ 1.75f }, m_IsEscorting{ false }, m_ShootElapsedTime{ 0.f }, m_ShootInterval{ 0.f }, m_pGalagaCP{ nullptr }
{
	if (pOwner != nullptr)
	{
		m_pButterflyTransfCP = pOwner->GetComponent<engine::TransformComponent>();
		m_pMoveCP = pOwner->GetComponent<MoveComponent>();
		m_pEnemyCP = pOwner->GetComponent<EnemyCP>();
		m_pRotatorCP = pOwner->GetComponent<RotatorComponent>();
	}

	ResetShootInterval();
}

AI_ButterflyCP::~AI_ButterflyCP()
{

}

bool AI_ButterflyCP::IsOwnerActive() const
{
	return GetOwner()->IsActive();
}

void AI_ButterflyCP::ResetShootInterval()
{
	m_ShootInterval = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (3.0f - 1.0f)));
	m_ShootElapsedTime = 0.0f;
}

void AI_ButterflyCP::FireMissile()
{
	if (m_pEnemyCP != nullptr)
	{
		m_pEnemyCP->CalculateMissileDirection();
		m_pEnemyCP->FireMissile(0.0f);
	}
}

void AI_ButterflyCP::Update(const float deltaTime)
{
	if (m_pMoveCP != nullptr && m_pEnemyCP != nullptr && m_pButterflyTransfCP != nullptr)
	{
		if (m_IsFollowingGalaga)
		{
			// Check if the Galaga is active
			if (m_pGalagaCP && !m_pGalagaCP->IsOwnerActive())
			{
				ReturnToFormation();
				return;
			}

			UpdateFollowGalaga(deltaTime);

			// Update shooting timer
			m_ShootElapsedTime += deltaTime;
			if (m_ShootElapsedTime >= m_ShootInterval)
			{
				FireMissile();
				ResetShootInterval();
			}
			return;
		}

		if (m_pEnemyCP->GetCurrentState() == EnemyCP::ENEMY_STATE::attack)
		{
			UpdateAttack(deltaTime);
		}
	}
}

void AI_ButterflyCP::Shoot()
{
	if (m_pEnemyCP != nullptr)
	{
		m_pEnemyCP->CalculateMissileDirection();
		m_pEnemyCP->FireMissile(0.0f); // Fire a missile immediately
	}
}

void AI_ButterflyCP::UpdateFollowGalaga(const float deltaTime)
{
	glm::vec3 targetPos = m_CurrentGalagaPos + glm::vec3(m_Offset, 0.f, 0.f);
	glm::vec3 direction = glm::normalize(targetPos - m_pButterflyTransfCP->GetWorldPosition());

	float speedMultiplier = m_EscortSpeedMultiplier;
	if (std::abs(targetPos.y - m_pButterflyTransfCP->GetWorldPosition().y) < 1.0f)
	{
		// Revert to normal speed when close to same height
		speedMultiplier = 1.0f;
	}

	// Ensures continuous movement even when at the same height
	m_pMoveCP->Move(deltaTime, direction * speedMultiplier);
}

void AI_ButterflyCP::FollowGalaga(const glm::vec3& galagaPos, AI_GalagaCP* galagaCP, float offset)
{
	m_CurrentGalagaPos = galagaPos;
	m_Offset = offset;
	m_IsFollowingGalaga = true;

	m_pGalagaCP = galagaCP;

	if (m_pEnemyCP != nullptr)
	{
		m_pEnemyCP->CalculateMissileDirection();
	}
	ResetShootInterval();
}

void AI_ButterflyCP::ReturnToFormation()
{
	m_IsFollowingGalaga = false;
	m_AttackState = AttackState::breakFormation;
	m_pEnemyCP->ChangeCurrentState(EnemyCP::ENEMY_STATE::moveToFormation);
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
	case AI_ButterflyCP::AttackState::loop:
		UpdateBreakLoop(deltaTime);
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

	InitLoopData();

	m_AttackState = AttackState::loop;

}

void AI_ButterflyCP::InitLoopData()
{
	if (m_pRotatorCP != nullptr)
	{
		bool positiveRot = false;
		auto rotationCenter = m_pButterflyTransfCP->GetLocalPosition();
		float rotationAngle{}, targetRotation{};

		// Rotate on the opposite direction that the enemy will dive
		if (m_Direction.x > 0)
		{
			rotationCenter.x -= m_RotationRadius;
			rotationAngle = 0.f;
			targetRotation = -glm::pi<float>();
		}
		else
		{
			rotationCenter.x += m_RotationRadius;
			rotationAngle = glm::pi<float>();
			targetRotation = glm::two_pi<float>();		// Half rotation
			positiveRot = true;
		}

		m_pRotatorCP->SetDataRotation(m_RotationRadius, m_RotationTime, targetRotation,
			rotationCenter, positiveRot, rotationAngle);
	}
}

void AI_ButterflyCP::UpdateBreakLoop(const float deltaTime)
{
	if (m_pRotatorCP != nullptr)
	{
		m_pRotatorCP->Rotate(deltaTime);
		if (m_pRotatorCP->IsRotationFinished())
		{
			m_AttackState = AttackState::diagonalDive;
			if (m_pEnemyCP != nullptr)
			{
				m_pEnemyCP->CalculateMissileDirection();
			}
		}
	}
	else
	{
		// If no rotator just go directly to next behaviour
		AI_ButterflyCP::AttackState::diagonalDive;
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

void AI_ButterflyCP::Reset()
{
	m_AttackState = AttackState::breakFormation;
	m_DiagonalDiveMaxTime = 0.f;
	m_ElapsedSec = 0.f;
	m_AtRightSide = false;
	m_Direction = glm::vec3{ 1.f, 1.f, 0.f };
	m_MaxSteeringTime = 0.f;
	m_IsFollowingGalaga = false;

	ResetShootInterval();

	GetOwner()->SetIsActive(true);
}