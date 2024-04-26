#ifndef FRONTBACKBOARD_H_INCLUDED
 #define FRONTBACKBOARD_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Engine.h"

/**
* �O���180�x����]����Ɛ^�񒆂ɂ���_�̃I�u�W�F�N�g
* 
*/
class FrontBackBoard : public Component
{
public:
	FrontBackBoard() = default;
	virtual ~FrontBackBoard() = default;
	// �Q�[���I�u�W�F�N�g�ɒǉ����ꂽ���ɌĂяo�����
	virtual void Awake() override;
	// ��Ԃ̍X�V
	virtual void Update(float deltaTime) override;
private:
	// �h�A�̊J���
	enum class Boardstate {
		left,  // �����ɉ�]�����Ă�����
		right, // �E���ɉ�]�����Ă�����
		alive, // �������Ȃ��Ă��������ǒu�������Ƃ��̏��
		dead,  // ��ꂽ�Ƃ��p��~�߂����Ƃ��̏��

	};
	Boardstate boardstate = Boardstate::left;
	GameObjectPtr frontBackBoard;
	float timer = 0.0f;
	float timerCount = 0.0f;
	const float speed = 3.0f;
};

#endif // FRONTBACKBOARD_H_INCLUDED 