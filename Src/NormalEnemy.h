#ifndef NORMALENEMY_H_INCLUDED
#define NORMALENEMY_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
#include <functional>
/**
* そこら辺を歩いているエネミー
* 

*/
class NormalEnemy : public Component
{
public:
	NormalEnemy() = default;
	virtual ~NormalEnemy() = default;
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);
	enum class NormalEnemyState
	{
		alive,	//動いている時
		stop,	//動きのサイクルの一つ
		hit,	//プレイヤーに当たった時
		dead,	//死んだとき
	};
	NormalEnemyState GetState() const { return normalEnemyState; }
	void SetState(NormalEnemyState _state) {
		normalEnemyState = _state;
	}


private:
	NormalEnemyState normalEnemyState = NormalEnemyState::alive;
	float moveTimer = 0; // 動作制御用タイマー	
	float timer = 0; // 動作制御用タイマー	
	float timerCount = 0; // 動作制御用タイマーカウント
	const float speed = 0.5f;
	float baseY = 0.6f;    // 基準の高さ(m)
	float range = 0.1f; // 左右の範囲(m)
	float cycle = 2;    // 左右を行き来する間隔(秒)

};

#endif // NORMALENEMY_H_INCLUDED