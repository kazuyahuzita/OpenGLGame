#ifndef CHARACTERMOVEMENT_H_INCLUDED
 #define CHARACTERMOVEMENT_H_INCLUDED
 #include "Component.h"
 #include "GameObject.h"

/**
* �Q�[���I�u�W�F�N�g�̑��x�Əd�͂������R���|�[�l���g
*/
class CharacterMovement : public Component
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CharacterMovement() = default;
	virtual ~CharacterMovement() = default;

	// ���t���[��1��Ăяo�����
	virtual void Update(float deltaTime) override
	{
		auto owner = GetOwner();

		if (owner->isGrounded) {
			      // �ڒn�t���O�������Ă�����A�������ւ̉����x��0�ɂ���
				velocity.y = std::max(velocity.y, 0.0f);
			
		}
		else {

			// �d�͉����x�ɂ���đ��x���X�V����
			velocity.y -= gravity * gravityScale * deltaTime;
		}
		// ���x�����W�ɔ��f����
		owner->position += velocity * deltaTime;
	}

	static constexpr float gravity = 9.8f; // �d�͉����x
	vec3 velocity = { 0, 0, 0 }; // ���x
public:
	float gravityScale = 0.8f; // �d�͂̉e���𐧌䂷��W��
};
using CharacterMovementPtr = std::shared_ptr<CharacterMovement>;

#endif // CHARACTERMOVEMENT_H_INCLUDED