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
* ゴールを表示する下から粒子が出ているオブジェクト
*/

class Tornado : public Component
{
public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float lifespan = 1.0; // 生存期間
	float timer = 0.0f;
	vec2 range = { 2.0f, 2.0f }; // XZ方向の発生半径

};

/**
* 粒子の発生装置
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
	float timer = 0; // 粒子発生タイマー
	vec2 range = { 2.0f, 2.0f }; // XZ方向の発生半径

};

#endif // DARTSMOKE_H_INCLUDED