#ifndef SPAWNENEMY_H_INCLUDED
#define SPAWNENEMY_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/SphereCollider.h"
#include "Engine/VecMath.h"

/**
* 後々生成される予定の敵
* 
* 特別な関数を持っているクラス
* ポジションのXとZ軸を設定できるだけにしている
* @param void Tracking : positionX,positionZ,deltaTime
* ポジションのXとZ軸と距離を設定できる
* 距離範囲内でtrue、範囲外でfalseを返す。
* @param bool IsPlayerInRange　:positionX,positionZ,range
*/
class SpawnEnemy : public Component
{
public:
	SpawnEnemy() = default;
	virtual ~SpawnEnemy() = default;
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	virtual void Tracking(float positionX ,float positionZ,float deltaTime) override;
	bool IsPlayerInRange(float positionX, float positionZ, float range);

private:
	const float speed = 30.0f;
};

#endif // SPAWNENEMY_H_INCLUDED