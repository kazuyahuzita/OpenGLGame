/**
* @file DartSmoke.h
*/
#ifndef DARTSMOKE_H_INCLUDED
 #define DARTSMOKE_H_INCLUDED
 #include "Engine/Billboard.h"
 #include "Engine/Component.h"
 #include "Engine/Engine.h"
 #include "Engine/GameObject.h"
 #include "Engine/Random.h"
 #include "Engine/SphereCollider.h"

/**
* �S�[����\�����鉺���痱�q���o�Ă���I�u�W�F�N�g
*/

class Tornado : public Component
{
public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float lifespan = 1.0; // ��������
	float timer = 0.0f;
	vec2 range = { 2.0f, 2.0f }; // XZ�����̔������a

};

/**
* ���q�̔������u
*/
class TornadoEffect : public Component
{
public:
	virtual void Update(float deltaTime) override;

	enum class TornadoEffectState
	{
		alive,
		dead,
		stop,

	};

	TornadoEffectState GetState() const { return tornadoEffectState; }

	void SetState(TornadoEffectState _state) {
		tornadoEffectState = _state;
	}

private:
	TornadoEffectState tornadoEffectState = TornadoEffectState::stop;
	float timer = 0; // ���q�����^�C�}�[
	vec2 range = { 2.0f, 2.0f }; // XZ�����̔������a

};

#endif // DARTSMOKE_H_INCLUDED