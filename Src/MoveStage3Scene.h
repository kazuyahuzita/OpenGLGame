#ifndef MOVESTAGE3SCENE_H_INCLUDED
#define MOVESTAGE3SCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* Stage3Scene�ɔ�΂�������o���I�u�W�F�N�g
* player���G�ꂽ��V�[���ɔ�΂�����͔C���Ă���
* @param MoveStage3Scene��Inside��Outside
*/
class MoveStage3Scene : public Component
{
public:
	MoveStage3Scene() = default;
	virtual ~MoveStage3Scene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveStage3SceneState
	{
		Inside,		// �������Ă���
		Outside,	// �o�Ă�����
	};

	MoveStage3SceneState GetState() const { return ms3sState; }

	void SetState(MoveStage3SceneState _state) {
		ms3sState = _state;
	}

private:
	MoveStage3SceneState ms3sState = MoveStage3SceneState::Outside;
};

#endif // MOVESTAGE2SCENE_H_INCLUDED