#ifndef GALAGA_AI_BUTTERFLYCP
#define GALAGA_AI_BUTTERFLYCP

#include "Component.h"
#include "glm/vec3.hpp"

class EnemyCP;
class MoveComponent;
class MissileManagerCP;
class RotatorComponent;
namespace engine
{
	class TransformComponent;
	struct Window;
}

class AI_ButterflyCP final : public engine::Component
{
public:
	AI_ButterflyCP(engine::GameObject* pOwner);
	virtual ~AI_ButterflyCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Reset();

	bool IsOwnerActive() const;

	void FollowGalaga(const glm::vec3& galagaPos, float offset);
	void ReturnToFormation();
	bool IsEscorting() const { return m_IsEscorting; }
	void SetEscorting(bool escorting) { m_IsEscorting = escorting; }

private:
	enum class AttackState
	{
		breakFormation,
		loop,
		diagonalDive,
		zigZagSteer,
		followingGalaga
	};

	void InitAttackData(const glm::vec3& currentPos, const engine::Window& window);
	void InitLoopData();
	void UpdateAttack(const float deltaTime);
	void UpdateZigZagSteer(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);
	void UpdateDiagonalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);
	void UpdateBreakLoop(const float deltaTime);
	void UpdateFollowGalaga(const float deltaTime);
	void FireMissile();

	EnemyCP* m_pEnemyCP;
	MoveComponent* m_pMoveCP;
	engine::TransformComponent* m_pButterflyTransfCP;
	RotatorComponent* m_pRotatorCP;
	AttackState m_AttackState;

	// ATTACK STATE
	float m_ElapsedSec;
	float m_DiagonalDiveMaxTime;
	bool m_AtRightSide;							// Enemy at right side of the window? 
	glm::vec3 m_Direction;						// Keeps track of the direction the enemy is facing

	float m_MaxSteeringTime;					// For the steer right and left

	// For Loops
	float m_RotationTime;
	float m_RotationRadius;

	glm::vec3 m_OriginalFormationPos;           // To store original formation position
	glm::vec3 m_CurrentGalagaPos;               // To store the current position of the Galaga being followed
	float m_Offset;                             // Offset from the Galaga
	bool m_IsFollowingGalaga;                   // Flag to check if following Galaga
	float m_EscortSpeedMultiplier;              // Movement speed when escorting
	bool m_IsEscorting;
};

#endif