#ifndef MOVEBLOCK_H_INCLUDED
#define MOVEBLOCK_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

///<summary>
///MoveBlockという名前で横か縦に動く物体のヘッダー名として利用しています
///一つ一つのコードが少なかったのでひとまとめにしているだけなので
///これから増やす時はヘッダーをばらばらにする方が良い
///</summary>

/**
* 横に動く床
* Meshの設定をしていないので後々つけるか
* 動かしたいものにこれをAddComponentするように
* 横にのみ動く物体
* Y軸の値が0か180度の時、X軸に動く奴
*/
class XaxisMoveBlock: public Component
{
public:

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float timer = 0;
	//// タイマーによって座標を上下に揺らす
	float range = 5.0f;// 揺らす高さの範囲(m)
	float cycle = 1.0f;// 揺らす間隔(秒)


};//class XaxisMoveBlock

/**
* 横に動く床
* Meshの設定をしていないので後々つけるか
* 動かしたいものにこれをAddComponentするように
* 横にのみ動く物体
* Y軸の値が90か270度の時、Z軸に動く奴
*/
class ZaxisMoveBlock: public Component
{
public:

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float timer = 0;
	//// タイマーによって座標を上下に揺らす
	float range = 5.0f;// 揺らす高さの範囲(m)
	float cycle = 1.0f;// 揺らす間隔(秒)


};//class ZaxisMoveBlock

/**
* 縦に動く床
* Meshの設定をしていないので後々つけるか
* 動かしたいものにこれをAddComponentするように
* 縦にのみ動く物体
*/
class VerticalMoveBlock :public Component
{
public:

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float timer = 0;
	//// タイマーによって座標を上下に揺らす
	float range = 4.0f;// 揺らす高さの範囲(m)
	float cycle = 1.0f;// 揺らす間隔(秒)

};//class VerticalMoveBlock

#endif // MOVEBLOCK_H_INCLUDED