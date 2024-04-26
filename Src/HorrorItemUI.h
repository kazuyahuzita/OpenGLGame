#ifndef HORRORITEMUI_H_INCLUDED
#define HORRORITEMUI_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
#include "Engine/BoxCollider.h"
#include "Engine/Engine.h"

/**
* ホラーゲームシーンでのアイテムのUIを表示しているUI専用のオブジェクト
* 　アイテムをゲットしたら指定のisHitPlayerがtrueになり
* 　しっかり見えるようになる(alpha値を1にする)
*
*/
class HorrorItemUI : public Component
{
public:
	HorrorItemUI() = default;
	virtual ~HorrorItemUI() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	//それぞれのゲームオブジェクトの設定をBoolで管理できるようにする

	//ホラーゲームシーンでのバイクのバッテリーアイテムのGetter
	bool GetIsHitBattery() const { return isHitBattery; }
	//ホラーゲームシーンでのバイクのバッテリーアイテムのSetter
	void SetIsHitBattery(bool _isHitBattery)
	{
		isHitBattery = _isHitBattery;
	}
	//ホラーゲームシーンでのガス管アイテムのGetter
	bool GetIsHitGasCan() const { return isHitGasCan; }
	//ホラーゲームシーンでのガス管アイテムのSetter
	void SetIsHitGasCan(bool _isHitGasCan)
	{
		isHitGasCan = _isHitGasCan;
	}
	//ホラーゲームシーンでのバイクのシートアイテムのGetter
	bool GetIsHitSeat() const { return isHitSeat; }
	//ホラーゲームシーンでのバイクのシートアイテムのSetter
	void SetIsHitSeat(bool _isHitSeat)
	{
		isHitSeat = _isHitSeat;
	}
	//ホラーゲームシーンでのバイクのタイヤアイテムのGetter
	bool GetIsHitTyre() const { return isHitTyre; }
	//ホラーゲームシーンでのバイクのタイヤアイテムのSetter
	void SetIsHitTyre(bool _isHitTyre)
	{
		isHitTyre = _isHitTyre;
	}

private:


	//アイテム用のゲームオブジェクト
	GameObjectPtr Battery;
	GameObjectPtr GasCan;
	GameObjectPtr Seat;
	GameObjectPtr Tyre;
	GameObjectPtr BackShadow;

	bool isHitBattery	= false;
	bool isHitGasCan	= false;
	bool isHitSeat		= false;
	bool isHitTyre		= false;
};

#endif // HORRORITEMUI_H_INCLUDED