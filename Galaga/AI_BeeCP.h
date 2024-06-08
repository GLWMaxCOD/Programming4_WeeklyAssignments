#ifndef GALAGA_AI_BEECP
#define GALAGA_AI_BEECP

#include <Component.h>
#include "glm/vec3.hpp"

class EnemyCP;
class MoveComponent;
class RotatorComponent;
class MissileManagerCP;

namespace engine
{
	class TransformComponent;
	struct Window;
}

// AI component for controlling the behavior of Bee enemies
class AI_BeeCP final : public engine::Component
{
public:
	AI_BeeCP(engine::GameObject* pOwner);   // Constructor
	virtual ~AI_BeeCP() override;           // Destructor

	virtual void Update(const float deltaTime) override;   // Update the Bee AI state each frame
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;   // Handle received messages

	void Reset();  // Reset the AI state

private:
	// Different states of the Bee's attack behavior
	enum class AttackState
	{
		breakFormation,      // Break the formation and set up attack data
		loop,                // Perform a loop before diving
		diagonalDive,        // Dive diagonally
		verticalDive,        // Dive vertically
		roundSwoop           // Loop backwards to return to formation
	};

	void InitAttackData(const glm::vec3& currentPos, const engine::Window& window);  // Initialize attack data
	void InitLoopData();  // Initialize data for the loop
	void UpdateAttack(const float deltaTime);  // Update the attack behavior
	void UpdateDiagonalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);  // Update diagonal dive behavior
	void UpdateVerticalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);  // Update vertical dive behavior
	void UpdateBreakLoop(const float deltaTime);  // Update loop break behavior

	EnemyCP* m_pEnemyCP;               // Pointer to the Enemy component
	MoveComponent* m_pMoveCP;          // Pointer to the Move component
	engine::TransformComponent* m_pTransformCP;  // Pointer to the Transform component
	RotatorComponent* m_pRotatorCP;    // Pointer to the Rotator component
	AttackState m_AttackState;         // Current state of the attack behavior

	// Attack state data
	float m_ElapsedDiagonalDive;       // Time elapsed during diagonal dive
	float m_DiagonalDiveMaxTime;       // Maximum time for diagonal dive
	bool m_AtRightSide;                // Is the enemy on the right side of the window?

	glm::vec3 m_Direction;             // Direction the enemy is facing

	// Swoop round data
	float m_RotationTime;              // Time to complete rotation
	float m_RotationRadius;            // Radius of the rotation
};

#endif // GALAGA_AI_BEECP