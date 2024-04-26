#ifndef GATEEFFECT_H_INCLUDED
#define GATEEFFECT_H_INCLUDED
#include "Engine/Billboard.h"
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/GameObject.h"
#include "Engine/Random.h"

/**
* �Q�[�g�̎���𕑂��Ă��闱�q
*/
class GateEffectParticle : public Component
{
public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float lifespan = 1; // ��������
};

class GateEffectSmoke : public Component
{
public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float lifespan = 1.0; // ��������
	float interval = 150.0f;    // ���𔭐�����Ԋu
	float timer = 0.0f;
};

/**
* ���q�̔������u
*/
class GateEffect : public Component
{
public:
	virtual void Update(float deltaTime) override;

	enum class GateEffectState
	{
		alive,
		dead,
		stop,

	};

	GateEffectState GetState() const { return gateEffectState; }

	void SetState(GateEffectState _state) {
		gateEffectState = _state;
	}

private:
	GateEffectState gateEffectState = GateEffectState::stop;
	float timer = 0; // ���q�����^�C�}�[
	vec2 range = { 0.8f, 1.0f }; // XZ�����̔������a

};

#endif // GATEEFFECT_H_INCLUDED