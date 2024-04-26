#ifndef SLIDINGMOVEMENT_H_INCLUDED
 #define SLIDINGMOVEMENT_H_INCLUDED
 #include "Component.h"
 #include "GameObject.h"
#include <random>

/**
* ���Ɗ��鏰���
*/
class SlidingMovement : public Component
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	SlidingMovement() = default;
	virtual ~SlidingMovement() = default;

	// ���t���[��1��Ăяo�����
	virtual void Update(float deltaTime) override
	{
		auto owner = GetOwner();
		if (owner->isSlided ==false) {
			return;
		}
		if (owner->isSlided) {
			
			SlideMove(deltaTime);
			// �A�C�X���̐ڒn�t���O�������Ă�����A�����Ă���
			//std::max�͗����̒l�̑傫������Ԃ��֐�
			velocity.x = std::max(velocity.x, 0.0f);
			
		}
		// ���x�����W�ɔ��f����
		owner->position += velocity * deltaTime;
	}
	void SlideMove(float deltaTime) {

		//�ȒP�Ȑ��l���Ɗ��鏰�ɐU��񂳂�銴��
		//�ɂȂ�̂ŁA���G�Ȋ����ɂ��Ă���.

		std::random_device rd;
		std::mt19937 gen(rd());
		//-1.0f����1.0�͈͓̔��Ŏ�������l���z�Ő���;
		std::uniform_real_distribution<float> dis(-2.0f, 2.0f);

		float moveX = dis(gen);
		float moveZ = dis(gen);

		// �ړ��ʂ����W�ɉ��Z
		velocity.x += moveX* deltaTime;
		velocity.z += moveZ* deltaTime;
	}
	vec3 velocity = { 0, 0, 0 }; // ���x
public:
};
//�N���X�̃R���X�g���N�^��֐��̌p��
using SlidingMovementPtr = std::shared_ptr<SlidingMovement>;

#endif // SLIDINGMOVEMENT_H_INCLUDED