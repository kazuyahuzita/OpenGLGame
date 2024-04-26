#ifndef GEMSUI_H_INCLUDED
#define GEMSUI_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

#include <functional>
#include<random>

/**
* �W�F���A�C�e����UI��\�����Ă���UI��p�̃I�u�W�F�N�g
* �@�W�F�����Q�b�g������w���isHitPlayer��true�ɂȂ�
* �@�������茩����悤�ɂȂ�(alpha�l��1�ɂ���)
* 
*/
class GemsUI : public Component
{
public:
	GemsUI() = default;
	virtual ~GemsUI() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	//���ꂼ��̃Q�[���I�u�W�F�N�g�̐ݒ��Bool�ŊǗ��ł���悤�ɂ���

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


	//�X�e�[�W�̐����p�̃Q�[���I�u�W�F�N�g
	GameObjectPtr gemObject;
	bool isHitPlayer  = false;
	bool isHitPlayer1 = false;
	bool isHitPlayer2 = false;
	bool isHitPlayer3 = false;
	bool isHitPlayer4 = false;
	//5�̐ݒ�ɂ��Ă���̂Ō���5��
	int number = 5;
	GameObjectPtr num[6];
};

#endif // GEMSUI_H_INCLUDED