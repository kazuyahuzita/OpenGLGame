#ifndef GEMSUI_H_INCLUDED
#define GEMSUI_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

#include <functional>
#include<random>

/**
* ジェムアイテムのUIを表示しているUI専用のオブジェクト
* 　ジェムをゲットしたら指定のisHitPlayerがtrueになり
* 　しっかり見えるようになる(alpha値を1にする)
* 
*/
class GemsUI : public Component
{
public:
	GemsUI() = default;
	virtual ~GemsUI() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	//それぞれのゲームオブジェクトの設定をBoolで管理できるようにする

	bool GetIsHitPlayer() { return isHitPlayer; }
	void SetIsHitPlayer(bool _isHitPlayer)
	{
		isHitPlayer = _isHitPlayer;
	}
	bool GetIsHitPlayer1() { return isHitPlayer1; }
	void SetIsHitPlayer1(bool _isHitPlayer)
	{
		isHitPlayer1 = _isHitPlayer;
	}
	bool GetIsHitPlayer2() { return isHitPlayer2; }
	void SetIsHitPlayer2(bool _isHitPlayer)
	{
		isHitPlayer2 = _isHitPlayer;
	}
	bool GetIsHitPlayer3() { return isHitPlayer3; }
	void SetIsHitPlayer3(bool _isHitPlayer)
	{
		isHitPlayer3 = _isHitPlayer;
	}
	bool GetIsHitPlayer4() { return isHitPlayer4; }
	void SetIsHitPlayer4(bool _isHitPlayer)
	{
		isHitPlayer4 = _isHitPlayer;
	}

private:


	//ステージの説明用のゲームオブジェクト
	GameObjectPtr gemObject;
	bool isHitPlayer  = false;
	bool isHitPlayer1 = false;
	bool isHitPlayer2 = false;
	bool isHitPlayer3 = false;
	bool isHitPlayer4 = false;
	//5個の設定にしているので個数は5個に
	int number = 5;
	GameObjectPtr num[6];
};

#endif // GEMSUI_H_INCLUDED