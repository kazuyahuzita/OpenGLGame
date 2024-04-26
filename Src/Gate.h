#ifndef GATE_H_INCLUDED
#define GATE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
#include "Engine/BoxCollider.h"
#include "Engine/Engine.h"

/**
* 門
* 下から出現するタイプかそのまま配置できるタイプか選べるようにしてある
* 練習用のシーンに配置しているのでmoveTimerはそれに合わせている
* @param moveTimerによって門が上に上がる
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