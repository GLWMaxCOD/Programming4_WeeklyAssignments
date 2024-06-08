#ifndef GALAGA_AI_GALAGACP
#define GALAGA_AI_GALAGACP

#include "Component.h"
#include "Observer.h"
#include "Event.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

class MoveComponent;
class RotatorComponent;
class MissileManagerCP;
class EnemyCP;
namespace engine
{
	class TransformComponent;
	struct Window;
	class CollisionComponent;
}

class AI_ButterflyCP;

class AI_GalagaCP final : public engine::Component, public engine::Observer
{
public:
	AI_GalagaCP(engine::GameObject* pOwner);
	virtual ~AI_GalagaCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;
	void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

	void Reset();
	void ChangeSprite(const std::string& spritePath);

	void SetAIActive(bool isActive);
	void SetFormationOnly(bool formationOnly);

	enum class AttackState
	{
		breakFormation,
		startLoop,
		doAttack,
		finishAttack,
		formationOnly //For VERSUS Galaga
	};

	AttackState GetAttackState() const;
	bool GetIsAttacking();
	void SetAttackState(AttackState newState);
	void SetVersusMode(bool isVersusMode);
	bool IsVersusMode() const;

	void SetEscortButterflies(AI_ButterflyCP* butterflyLeft, AI_ButterflyCP* butterflyRight);
	void ReplaceDeadEscorts(std::vector<engine::GameObject*>& butterflies);

private:
	enum class BombinRunState
	{
		moveToLoopPoint,
		divingLoop
	};
	enum class TractorBeamState
	{
		moveIntoPosition,
		tractorBeam
	};

	void InitData(const engine::Window window);
	void LoopAndDive(const float deltaTime);
	void UpdateAttack(const float deltaTime, const glm::vec3& currentPos);
	void LeaveLevel(const float deltaTime, float galagaYPos, const engine::Window& window);

	// Tractor beam behaviour
	void moveIntoPosition(const float deltaTime, const glm::vec3& currentPos);
	void UpdateTractorBeam(const float deltaTime);

	engine::GameObject* m_pTractorBeam;
	engine::CollisionComponent* m_pTractorBeamCollisionCP;

	// Bombing run behaviour
	void UpdateBombingRun(const float deltaTime, const glm::vec3& currentPos);

	AttackState m_AttackState;
	BombinRunState m_BombingRunState;
	TractorBeamState m_TractorBeamState;
	bool m_DoTractorBeam;							// To know which behavior do

	MoveComponent* m_pMoveCP;
	engine::TransformComponent* m_pGalagaTransfCP;
	RotatorComponent* m_pRotatorCP;
	EnemyCP* m_pEnemyCP;
	bool m_DoRotateLeft;							// On which direction to rotate when breaking formation

	// LOOP data
	const float ROTATION_TIME;
	float m_RotationRadius;

	glm::vec2 m_TractorBeamPos;
	glm::vec3 m_Direction;

	const float MAX_TRACTORBEAM_TIME;
	float m_ElapsedTime;

	bool m_IsAIActive;
	bool m_FormationOnly;
	bool m_IsVersusMode;
	bool m_IsAttacking;

	bool m_PlayerHit;								// To avoid from the player being instakilled by Tractorbeam, it's too OP
	bool m_IsRetracting;							// To track if the tractor beam is retracting

	bool m_IsTractorBeamRun;						// To track is the current attack is the tractor beam run

	int m_StartFrame;

	AI_ButterflyCP* m_pButterflyLeft;               // Left escort butterfly
	AI_ButterflyCP* m_pButterflyRight;              // Right escort butterfly
};
#endif // DEBUG
