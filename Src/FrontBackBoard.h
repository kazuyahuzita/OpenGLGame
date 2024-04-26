#ifndef FRONTBACKBOARD_H_INCLUDED
 #define FRONTBACKBOARD_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Engine.h"

/**
* 前後に180度ずつ回転する板と真ん中にある棒のオブジェクト
* 
*/
class FrontBackBoard : public Component
{
public:
	FrontBackBoard() = default;
	virtual ~FrontBackBoard() = default;
	// ゲームオブジェクトに追加された時に呼び出される
	virtual void Awake() override;
	// 状態の更新
	virtual void Update(float deltaTime) override;
private:
	// ドアの開閉状態
	enum class Boardstate {
		left,  // 左側に回転させている状態
		right, // 右側に回転させている状態
		alive, // 何もしなくてもいいけど置きたいときの状態
		dead,  // 壊れたとき用や止めたいときの状態

	};
	Boardstate boardstate = Boardstate::left;
	GameObjectPtr frontBackBoard;
	float timer = 0.0f;
	float timerCount = 0.0f;
	const float speed = 3.0f;
};

#endif // FRONTBACKBOARD_H_INCLUDED 