#ifndef GATE_H_INCLUDED
#define GATE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
#include "Engine/BoxCollider.h"
#include "Engine/Engine.h"

/**
* ��
* ������o������^�C�v�����̂܂ܔz�u�ł���^�C�v���I�ׂ�悤�ɂ��Ă���
* ���K�p�̃V�[���ɔz�u���Ă���̂�moveTimer�͂���ɍ��킹�Ă���
* @param moveTimer�ɂ���Ė傪��ɏオ��
*/
class Gate : public Component
{
public:
	Gate() = default;
	virtual ~Gate() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	enum class GateState
	{
		alive,
		dead,
		stop,

	};

	GateState GetState() const { return gateState; }

	void SetState(GateState _state) {
		gateState = _state;
	}

	float GetMoveTimer()const { return moveTimer; }

	void SetMoveTimer(float _moveTimer)
	{
		moveTimer = _moveTimer;
	}
private:
	float moveTimer = 0.0f;
	GateState gateState = GateState::stop;
};

#endif // GATE_H_INCLUDED