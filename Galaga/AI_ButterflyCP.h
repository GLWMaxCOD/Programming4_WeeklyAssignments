#ifndef GALAGA_AI_BUTTERFLYCP
#define GALAGA_AI_BUTTERFLYCP

#include "Component.h"
#include "glm/vec3.hpp"

class EnemyCP;
class MoveComponent;
class MissileManagerCP;
class RotatorComponent;
class AI_GalagaCP;

namespace engine
{
	class TransformComponent;
	struct Window;
}

// AI component for controlling the behavior of Butterfly enemies
class AI_ButterflyCP final : public engine::Component
{
public:
	AI_ButterflyCP(engine::GameObject* pOwner);   // Constructor
	virtual ~AI_ButterflyCP() override;           // Destructor

	virtual void Update(const float deltaTime) override;   // Update the Butterfly AI state each frame
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;   // Handle received messages

	void Reset();  // Reset the AI state

	bool IsOwnerActive() const;																// Check if the owner is active
	void Shoot();																			// Execute the shooting action
	void FollowGalaga(const glm::vec3& galagaPos, AI_GalagaCP* galagaCP, float offset);		// Follow the Galaga with a specified offset
	void ReturnToFormation();																// Return the butterfly to its formation
	bool IsEscorting() const { return m_IsEscorting; }										// Check if the butterfly is escorting
	void SetEscorting(bool escorting) { m_IsEscorting = escorting; }						// Set the escorting state

private:
	enum class AttackState
	{
		breakFormation,    // Break formation and prepare for attack
		loop,              // Perform a loop before diving
		diagonalDive,      // Dive diagonally
		zigZagSteer,       // Perform zigzag steering
		followingGalaga    // Follow the Galaga
	};

	void InitAttackData(const glm::vec3& currentPos, const engine::Window& window);  // Initialize attack data
	void InitLoopData();  // Initialize data for the loop
	void UpdateAttack(const float deltaTime);  // Update the attack behavior
	void UpdateZigZagSteer(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);  // Update zigzag steer behavior
	void UpdateDiagonalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);  // Update diagonal dive behavior
	void UpdateBreakLoop(const float deltaTime);  // Update loop break behavior
	void UpdateFollowGalaga(const float deltaTime);  // Update following Galaga behavior

	EnemyCP* m_pEnemyCP;               // Pointer to the Enemy component
	MoveComponent* m_pMoveCP;          // Pointer to the Move component
	engine::TransformComponent* m_pButterflyTransfCP;  // Pointer to the Transform component
	RotatorComponent* m_pRotatorCP;    // Pointer to the Rotator component
	AttackState m_AttackState;         // Current state of the attack behavior

	// Attack state data
	float m_ElapsedSec;                // Time elapsed during the current state
	float m_DiagonalDiveMaxTime;       // Maximum time for diagonal dive
	bool m_AtRightSide;                // Is the enemy on the right side of the window?
	glm::vec3 m_Direction;             // Direction the enemy is facing

	float m_MaxSteeringTime;           // Maximum time for steering left and right

	// Loop data
	float m_RotationTime;              // Time to complete rotation
	float m_RotationRadius;            // Radius of the rotation

	glm::vec3 m_OriginalFormationPos;  // Original formation position
	glm::vec3 m_CurrentGalagaPos;      // Current position of the Galaga being followed
	float m_Offset;                    // Offset from the Galaga
	bool m_IsFollowingGalaga;          // Flag to check if following Galaga
	float m_EscortSpeedMultiplier;     // Movement speed when escorting
	bool m_IsEscorting;                // Is the butterfly escorting?

	// Shooting mechanism
	float m_ShootElapsedTime;          // Time elapsed since the last shot
	float m_ShootInterval;             // Interval between shots
	void FireMissile();                // Fire a missile
	void ResetShootInterval();         // Reset the shoot interval

	AI_GalagaCP* m_pGalagaCP;          // Pointer to the Galaga being followed
};

#endif // GALAGA_AI_BUTTERFLYCP