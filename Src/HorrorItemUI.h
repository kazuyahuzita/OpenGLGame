#ifndef HORRORITEMUI_H_INCLUDED
#define HORRORITEMUI_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
#include "Engine/BoxCollider.h"
#include "Engine/Engine.h"

/**
* �z���[�Q�[���V�[���ł̃A�C�e����UI��\�����Ă���UI��p�̃I�u�W�F�N�g
* �@�A�C�e�����Q�b�g������w���isHitPlayer��true�ɂȂ�
* �@�������茩����悤�ɂȂ�(alpha�l��1�ɂ���)
*
*/
class HorrorItemUI : public Component
{
public:
	HorrorItemUI() = default;
	virtual ~HorrorItemUI() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	//���ꂼ��̃Q�[���I�u�W�F�N�g�̐ݒ��Bool�ŊǗ��ł���悤�ɂ���

	//�z���[�Q�[���V�[���ł̃o�C�N�̃o�b�e���[�A�C�e����Getter
	bool GetIsHitBattery() const { return isHitBattery; }
	//�z���[�Q�[���V�[���ł̃o�C�N�̃o�b�e���[�A�C�e����Setter
	void SetIsHitBattery(bool _isHitBattery)
	{
		isHitBattery = _isHitBattery;
	}
	//�z���[�Q�[���V�[���ł̃K�X�ǃA�C�e����Getter
	bool GetIsHitGasCan() const { return isHitGasCan; }
	//�z���[�Q�[���V�[���ł̃K�X�ǃA�C�e����Setter
	void SetIsHitGasCan(bool _isHitGasCan)
	{
		isHitGasCan = _isHitGasCan;
	}
	//�z���[�Q�[���V�[���ł̃o�C�N�̃V�[�g�A�C�e����Getter
	bool GetIsHitSeat() const { return isHitSeat; }
	//�z���[�Q�[���V�[���ł̃o�C�N�̃V�[�g�A�C�e����Setter
	void SetIsHitSeat(bool _isHitSeat)
	{
		isHitSeat = _isHitSeat;
	}
	//�z���[�Q�[���V�[���ł̃o�C�N�̃^�C���A�C�e����Getter
	bool GetIsHitTyre() const { return isHitTyre; }
	//�z���[�Q�[���V�[���ł̃o�C�N�̃^�C���A�C�e����Setter
	void SetIsHitTyre(bool _isHitTyre)
	{
		isHitTyre = _isHitTyre;
	}

private:


	//�A�C�e���p�̃Q�[���I�u�W�F�N�g
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