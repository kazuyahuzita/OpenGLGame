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
//�����֘A�̃I�u�W�F�N�g�Ǘ������Ă�������
//�ǂ�Ȍ����ł��Ή��ł���悤�ɂ���

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
	//���Ԃ̒������V�[���ł�������悤�ɂ�����
	//1�̌�
	float GetCountTime() const { return countTime; }
	void SetCountTime(float _countTime)
	{
		countTime = _countTime;
	}
	//10�̌�
	int GetCountTime1() const { return countTime1; }
	void SetCountTime1(int _countTime1)
	{
		countTime1 = _countTime1;
	}
	//���Ԃ̏I����m�点�Ă����B
	bool GetIsCount()const { return isCount; }
	void SetIsCount(bool _isCount)
	{
		isCount = _isCount;
	}
	
private:

	MeasureTimeState measureTimeState = MeasureTimeState::dead;
	//GameObjectPtr(shared_ptr)��vector�^�Ɋi�[���邽�߂̕ϐ�
	//shared_ptr���i�[���邱�ƂŁA�_���������
	//1,shared_ptr�̃R�s�[�Ƃ����[�u���������Ƃ��ɓ�����
	//		�Q�ƃJ�E���g���X�V���邱�ƂŁA�����I�ȋ��L��
	//2,���I�ȃT�C�Y�ύX�̍ۂɕ֗�
	//3,�v�f�ւ̃A�N�Z�X���ȒP��

	std::vector<GameObjectPtr> SingledigitVector = std::vector<GameObjectPtr>();
	std::vector<GameObjectPtr> DoubledigitVector = std::vector<GameObjectPtr>();

	//1�̌��p��textObject
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


	//10�̌��p��textObject
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