#ifndef DARTSMOKE_H_INCLUDED
#define DARTSMOKE_H_INCLUDED
#include "Engine/Billboard.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/GameObject.h"
#include "Engine/Random.h"
#include "Engine/SphereCollider.h"

/**
* �y���̗��q
*/

class DartSmoke : public Component
{
public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float lifespan = 1.0; // ��������
	float timer = 0.0f;
};

/**
* ���q�̔������u
*/
class DartSmokeEffect : public Component
{
public:
	virtual void Update(float deltaTime) override;

	enum class DartSmokeEffectState
	{
		alive,
		dead,
		stop,

	};

	DartSmokeEffectState GetState() const { return dartSmokeEffectState; }

	void SetState(DartSmokeEffectState _state) {
		dartSmokeEffectState = _state;
	}

private:
	DartSmokeEffectState dartSmokeEffectState = DartSmokeEffectState::stop;
	float timer = 0; // ���q�����^�C�}�[
	float moveTimer = 0.0f;
	float moveTimerCount = 0.0f;
	vec2 range = { 2.0f, 2.0f }; // XZ�����̔������a

};

#endif // DARTSMOKE_H_INCLUDED