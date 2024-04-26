/**
* @file Collider.h
*/
#ifndef COLLIDER_H_INCLUDED
 #define COLLIDER_H_INCLUDED
 #include "Component.h"
 #include "VecMath.h"
 #include <memory>

// ��s�錾
class Collider;
using ColliderPtr = std::shared_ptr<Collider>;

/**
* �R���C�_�[�̊��N���X
*/
class Collider : public Component
{
public:
	Collider() = default;
	virtual ~Collider() = default;

	//�R���C�_�[�̎��ۂ̐}�`�̎�ނ��ƂɕԂ�
	//�}�`�̎��
	enum class Type {
		AABB,   // �����s���E�{�b�N�X
		Sphere, // ����
		Box,	//�L�����E�{�b�N�X
	};
	virtual Type GetType() const = 0;

	// ���W��ύX����
	virtual void AddPosition(const vec3& translate) = 0;

	// ���W�ϊ������R���C�_�[���擾����
	virtual ColliderPtr GetTransformedCollider(const mat4& transform) const = 0;

	bool isTrigger = false; // true=�d��������, false=�d�����֎~
	bool isStatic = false;  // true=�����Ȃ�����, false=��������
};

#endif // COLLIDER_H_INCLUDED