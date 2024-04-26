#ifndef TEXTBLOCK_H_INCLUDED
#define TEXTBLOCK_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

#include <functional>
#include<random>

/**
* ���ꂼ��̃X�e�[�W�ł̌ŗL�̃e�L�X�g���Ǘ�����
* �����蔻�肪�Ȃ��A�����ɉ����Ĕ�������悤�ɂ��Ă���
*/
class TextBlock : public Component
{
public:
	TextBlock() = default;
	virtual ~TextBlock() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	bool IsPlayerInRange(float positionX, float positionZ, float range);

	//���ꂼ��̃Q�[���I�u�W�F�N�g�̐ݒ��Bool�ŊǗ��ł���悤�ɂ���
	/*
	* ExplorationScene�Ŏg���Ă���textObject��Gettar
	* @param Stage1�̃e�L�X�g
	*/
	bool GetIsHitPlayer() { return isHitPlayer; }
	/*
	* ExplorationScene�Ŏg���Ă���textObject��Settar
	* @param Stage1�̃e�L�X�g
	*/
	void SetIsHitPlayer(bool _isHitPlayer)
	{
		isHitPlayer = _isHitPlayer;
	}
	/*
	* ExplorationScene�Ŏg���Ă���textObject��Gettar
	* @param Stage2�̃e�L�X�g
	*/
	bool GetIsHitPlayer1() { return isHitPlayer1; }
	/*
	* ExplorationScene�Ŏg���Ă���textObject��Settar
	* @param Stage2�̃e�L�X�g
	*/
	void SetIsHitPlayer1(bool _isHitPlayer)
	{
		isHitPlayer1 = _isHitPlayer;
	}
	/*
	* ExplorationScene�Ŏg���Ă���textObject��Gettar
	* @param Stage3�̃e�L�X�g
	*/
	bool GetIsHitPlayer2() { return isHitPlayer2; }
	/*
	* ExplorationScene�Ŏg���Ă���textObject��Settar
	* @param Stage3�̃e�L�X�g
	*/
	void SetIsHitPlayer2(bool _isHitPlayer)
	{
		isHitPlayer2 = _isHitPlayer;
	}
	/*
	* ExplorationScene�Ŏg���Ă���textObject��Gettar
	* @param Stage4�̃e�L�X�g
	*/
	bool GetIsHitPlayer3() { return isHitPlayer3; }
	/*
	* ExplorationScene�Ŏg���Ă���textObject��Settar
	* @param Stage4�̃e�L�X�g
	*/
	void SetIsHitPlayer3(bool _isHitPlayer)
	{
		isHitPlayer3 = _isHitPlayer;
	}

	/*
	* AthleticStageScene�Ŏg���Ă���textObject��Gettar
	* @param NextStage�̃e�L�X�g
	*/
	bool GetIsHitPlayer4() { return isHitPlayer4; }
	/*
	* AthleticStageScene�Ŏg���Ă���textObject��Settar
	* @param NextStage�̃e�L�X�g
	*/
	void SetIsHitPlayer4(bool _isHitPlayer)
	{
		isHitPlayer4 = _isHitPlayer;
	}
	/*
	* AthleticStageScene�Ŏg���Ă���textObject��Gettar
	* @param BackStage�̃e�L�X�g
	*/
	bool GetIsHitPlayer5() { return isHitPlayer5; }

	/*
	* AthleticStageScene�Ŏg���Ă���textObject��Settar
	* @param BackStage�̃e�L�X�g
	*/
	void SetIsHitPlayer5(bool _isHitPlayer)
	{
		isHitPlayer5 = _isHitPlayer;
	}

private:


	//�X�e�[�W�̐����p�̃Q�[���I�u�W�F�N�g

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