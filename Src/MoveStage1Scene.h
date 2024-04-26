#ifndef MOVESTAGE1SCENE_H_INCLUDED
#define MOVESTAGE1SCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* Stage1Scene�ɔ�΂�������o���I�u�W�F�N�g
* player���G�ꂽ��V�[���ɔ�΂�����͔C���Ă���
* @param MoveStage1Scene��Inside��Outside
*/
class MoveStage1Scene : public Component
{
public:
	MoveStage1Scene() = default;
	virtual ~MoveStage1Scene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveStage1SceneState
	{
		Inside,		// �������Ă���
		Outside,	// �o�Ă�����
	};

	MoveStage1SceneState GetState() const { return ms1sState; }

	void SetState(MoveStage1SceneState _state) {
		ms1sState = _state;
	}

private:
	MoveStage1SceneState ms1sState = MoveStage1SceneState::Outside;
};

#endif // MOVESTAGE2SCENE_H_INCLUDED