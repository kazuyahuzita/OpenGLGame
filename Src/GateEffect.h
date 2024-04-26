#ifndef GATEEFFECT_H_INCLUDED
#define GATEEFFECT_H_INCLUDED
#include "Engine/Billboard.h"
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/GameObject.h"
#include "Engine/Random.h"

/**
* ゲートの周りを舞っている粒子
*/
class GateEffectParticle : public Component
{
public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float lifespan = 1; // 生存期間
};

class GateEffectSmoke : public Component
{
public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float lifespan = 1.0; // 生存期間
	float interval = 150.0f;    // 霧を発成する間隔
	float timer = 0.0f;
};

/**
* 粒子の発生装置
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
	float timer = 0; // 粒子発生タイマー
	vec2 range = { 0.8f, 1.0f }; // XZ方向の発生半径

};

#endif // GATEEFFECT_H_INCLUDED