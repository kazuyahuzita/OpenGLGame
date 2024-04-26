#ifndef MOVEEXPLORATIONSCENE_H_INCLUDED
#define MOVEEXPLORATIONSCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* ExplorationScene�ɔ�΂�������o���I�u�W�F�N�g
* player���G�ꂽ��V�[���ɔ�΂�����͔C���Ă���
* @param MoveExplorationSceneState��Inside��Outside
*/
class MoveExplorationScene : public Component
{
public:
	MoveExplorationScene() = default;
	virtual ~MoveExplorationScene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveExplorationSceneState
	{
		Inside,		// �������Ă���
		Outside,	// �o�Ă�����
	};

	MoveExplorationSceneState GetState() const { return mesState; }

	void SetState(MoveExplorationSceneState _state) {
		mesState = _state;
	}

private:
	MoveExplorationSceneState mesState = MoveExplorationSceneState::Outside;
};

#endif // MOVEEXPLORATIONSCENE_H_INCLUDED