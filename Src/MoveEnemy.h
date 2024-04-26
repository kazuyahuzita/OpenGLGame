#ifndef MOVEENEMY_H_INCLUDED
#define MOVEENEMY_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/SphereCollider.h"
#include "Engine/VecMath.h"

/**
* playerを追いかけるエネミー
* 
* vec3型のpositionを設定できるようにしている
* @param void Tracking : Position,deltaTime
* ポジションのXとZ軸と距離を設定できる
* 距離範囲内でtrue、範囲外でfalseを返す。
* @param bool IsPlayerInRange　:positionX,positionZ,range

*/
class MoveEnemy : public Component
{
public:
	MoveEnemy() = default;
	virtual ~MoveEnemy() = default;
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	// 衝突時の処理
	virtual void OnCollision(
		const ComponentPtr& self, const ComponentPtr& other) override;

	//MoveEnemyのステート
	enum class MoveEnemyState
	{
		alive, // 生きている
		hit,   //当たった
		stop,  //止まる
		dead,  //死んだ
	};
	MoveEnemyState GetState() const { return moveEnemyState; }
	void SetState(MoveEnemyState _state) {
		moveEnemyState = _state;
	}
	
	void Tracking(vec3 Position, float deltaTime) ;
	bool IsPlayerInRange(float positionX, float positionZ, float range) ;

private:
	MoveEnemyState moveEnemyState = MoveEnemyState::alive;
	const float speed = 2.0f;
};

#endif // MOVEENEMY_H_INCLUDED