#ifndef MOVEHORRORSCENE_H_INCLUDED
#define MOVEHORRORSCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* HorrorScene�ɔ�΂�������o���I�u�W�F�N�g
* player���G�ꂽ��V�[���ɔ�΂�����͔C���Ă���
* @param MoveHorrorSceneState��Inside��Outside
*/
class MoveHorrorScene : public Component
{
public:
	MoveHorrorScene() = default;
	virtual ~MoveHorrorScene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveHorrorSceneState
	{
		Inside,		// �������Ă���
		Outside,	// �o�Ă�����
	};

	MoveHorrorSceneState GetState() const { return mhsState; }

	void SetState(MoveHorrorSceneState _state) {
		mhsState = _state;
	}

private:
	MoveHorrorSceneState mhsState = MoveHorrorSceneState::Outside;
};

#endif // MOVEHORRORSCENE_H_INCLUDED