#ifndef MOVEATHLETICSCENE_H_INCLUDED
#define MOVEATHLETICSCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* AthleticScene�ɔ�΂�������o���I�u�W�F�N�g
* player���G�ꂽ��V�[���ɔ�΂�����͔C���Ă���
* @param MoveAthleticSceneState��Inside��Outside
*/
class MoveAthleticScene : public Component
{
public:
	MoveAthleticScene() = default;
	virtual ~MoveAthleticScene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveAthleticSceneState
	{
		Inside,		// �������Ă���
		Outside,	// �o�Ă�����
	};

	MoveAthleticSceneState GetState() const { return masState; }

	void SetState(MoveAthleticSceneState _state) {
		masState = _state;
	}

private:
	MoveAthleticSceneState masState = MoveAthleticSceneState::Outside;
};

#endif // MOVEATHLETICSCENE_H_INCLUDED