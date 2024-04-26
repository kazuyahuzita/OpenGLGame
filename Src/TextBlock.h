#ifndef TEXTBLOCK_H_INCLUDED
#define TEXTBLOCK_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

#include <functional>
#include<random>

/**
* それぞれのステージでの固有のテキストを管理する
* 当たり判定がなく、距離に応じて反応するようにしている
*/
class TextBlock : public Component
{
public:
	TextBlock() = default;
	virtual ~TextBlock() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	bool IsPlayerInRange(float positionX, float positionZ, float range);

	//それぞれのゲームオブジェクトの設定をBoolで管理できるようにする
	/*
	* ExplorationSceneで使われているtextObjectのGettar
	* @param Stage1のテキスト
	*/
	bool GetIsHitPlayer() { return isHitPlayer; }
	/*
	* ExplorationSceneで使われているtextObjectのSettar
	* @param Stage1のテキスト
	*/
	void SetIsHitPlayer(bool _isHitPlayer)
	{
		isHitPlayer = _isHitPlayer;
	}
	/*
	* ExplorationSceneで使われているtextObjectのGettar
	* @param Stage2のテキスト
	*/
	bool GetIsHitPlayer1() { return isHitPlayer1; }
	/*
	* ExplorationSceneで使われているtextObjectのSettar
	* @param Stage2のテキスト
	*/
	void SetIsHitPlayer1(bool _isHitPlayer)
	{
		isHitPlayer1 = _isHitPlayer;
	}
	/*
	* ExplorationSceneで使われているtextObjectのGettar
	* @param Stage3のテキスト
	*/
	bool GetIsHitPlayer2() { return isHitPlayer2; }
	/*
	* ExplorationSceneで使われているtextObjectのSettar
	* @param Stage3のテキスト
	*/
	void SetIsHitPlayer2(bool _isHitPlayer)
	{
		isHitPlayer2 = _isHitPlayer;
	}
	/*
	* ExplorationSceneで使われているtextObjectのGettar
	* @param Stage4のテキスト
	*/
	bool GetIsHitPlayer3() { return isHitPlayer3; }
	/*
	* ExplorationSceneで使われているtextObjectのSettar
	* @param Stage4のテキスト
	*/
	void SetIsHitPlayer3(bool _isHitPlayer)
	{
		isHitPlayer3 = _isHitPlayer;
	}

	/*
	* AthleticStageSceneで使われているtextObjectのGettar
	* @param NextStageのテキスト
	*/
	bool GetIsHitPlayer4() { return isHitPlayer4; }
	/*
	* AthleticStageSceneで使われているtextObjectのSettar
	* @param NextStageのテキスト
	*/
	void SetIsHitPlayer4(bool _isHitPlayer)
	{
		isHitPlayer4 = _isHitPlayer;
	}
	/*
	* AthleticStageSceneで使われているtextObjectのGettar
	* @param BackStageのテキスト
	*/
	bool GetIsHitPlayer5() { return isHitPlayer5; }

	/*
	* AthleticStageSceneで使われているtextObjectのSettar
	* @param BackStageのテキスト
	*/
	void SetIsHitPlayer5(bool _isHitPlayer)
	{
		isHitPlayer5 = _isHitPlayer;
	}

private:


	//ステージの説明用のゲームオブジェクト

	GameObjectPtr textObject;
	GameObjectPtr textObject1;
	GameObjectPtr textObject2;
	GameObjectPtr textObject3;
	GameObjectPtr nextSceneTextObject;
	GameObjectPtr backSceneTextObject;

	bool isHitPlayer = false;
	bool isHitPlayer1 = false;
	bool isHitPlayer2 = false;
	bool isHitPlayer3 = false;
	bool isHitPlayer4 = false;
	bool isHitPlayer5 = false;

};

#endif // TEXTBLOCK_H_INCLUDED