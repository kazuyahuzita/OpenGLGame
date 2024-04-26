#ifndef MOVESTAGE2SCENE_H_INCLUDED
#define MOVESTAGE2SCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* Stage2Scene�ɔ�΂�������o���I�u�W�F�N�g
* player���G�ꂽ��V�[���ɔ�΂�����͔C���Ă���
* @param MoveStage2Scene��Inside��Outside
*/
class MoveStage2Scene : public Component
{
public:
	MoveStage2Scene() = default;
	virtual ~MoveStage2Scene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveStage2SceneState
	{
		Inside,		// �������Ă���
		Outside,	// �o�Ă�����
	};

	MoveStage2SceneState GetState() const { return ms2sState; }

	void SetState(MoveStage2SceneState _state) {
		ms2sState = _state;
	}

private:
	MoveStage2SceneState ms2sState = MoveStage2SceneState::Outside;
};

#endif // MOVESTAGE2SCENE_H_INCLUDED