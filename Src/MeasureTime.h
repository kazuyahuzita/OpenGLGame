#ifndef MEASURETIME_H_INCLUDED
#define MEASURETIME_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/UILayout.h"
#include "Engine/VecMath.h"

//#include <functional>
//#include<random>
//#include<iostream>
//#include <vector>
//#include <memory>
//数字関連のオブジェクト管理をしていきたい
//どんな桁数でも対応できるようにする

/**
* 
*/
class MeasureTime : public Component
{
public:
	MeasureTime() = default;
	virtual ~MeasureTime() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	
	enum class MeasureTimeState
	{
		alive,
		dead,
		none
	};

	MeasureTimeState GetState() const{return measureTimeState;}

	void SetState(MeasureTimeState _measureTimeState)
	{
		measureTimeState = _measureTimeState;
	}
	//時間の調整をシーンでいじられるようにしたい
	//1の桁
	float GetCountTime() const { return countTime; }
	void SetCountTime(float _countTime)
	{
		countTime = _countTime;
	}
	//10の桁
	int GetCountTime1() const { return countTime1; }
	void SetCountTime1(int _countTime1)
	{
		countTime1 = _countTime1;
	}
	//時間の終わりを知らせてくれる。
	bool GetIsCount()const { return isCount; }
	void SetIsCount(bool _isCount)
	{
		isCount = _isCount;
	}
	
private:

	MeasureTimeState measureTimeState = MeasureTimeState::dead;
	//GameObjectPtr(shared_ptr)をvector型に格納するための変数
	//shared_ptrを格納することで、柔軟性があがる
	//1,shared_ptrのコピーとかムーブがおきたときに内部の
	//		参照カウントを更新することで、効率的な共有が
	//2,動的なサイズ変更の際に便利
	//3,要素へのアクセスが簡単に

	std::vector<GameObjectPtr> SingledigitVector = std::vector<GameObjectPtr>();
	std::vector<GameObjectPtr> DoubledigitVector = std::vector<GameObjectPtr>();

	//1の桁用のtextObject
	GameObjectPtr singledigit0;
	GameObjectPtr singledigit1;
	GameObjectPtr singledigit2;
	GameObjectPtr singledigit3;
	GameObjectPtr singledigit4;
	GameObjectPtr singledigit5;
	GameObjectPtr singledigit6;
	GameObjectPtr singledigit7;
	GameObjectPtr singledigit8;
	GameObjectPtr singledigit9;


	//10の桁用のtextObject
	GameObjectPtr doubledigit0;
	GameObjectPtr doubledigit1;
	GameObjectPtr doubledigit2;
	GameObjectPtr doubledigit3;
	GameObjectPtr doubledigit4;
	GameObjectPtr doubledigit5;
	GameObjectPtr doubledigit6;
	GameObjectPtr doubledigit7;
	GameObjectPtr doubledigit8;
	GameObjectPtr doubledigit9;

	GameObjectPtr clockObject;
	GameObjectPtr backObject;

	float countTime =10.0f;
	int countTime1 =9;
	bool isCount = false;
};

#endif // TEXTBLOCK_H_INCLUDED