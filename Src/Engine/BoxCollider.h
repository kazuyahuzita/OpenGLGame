/**
* @file BoxCollider.h
*/
#ifndef BOXCOLLIDER_H_INCLUDED
 #define BOXCOLLIDER_H_INCLUDED
 #include "Collider.h"
 #include "Collision.h"

/**
* BOX�R���C�_�[
*/
class BoxCollider : public Collider
{
public:
	BoxCollider() = default;
	virtual ~BoxCollider() = default;

	// �}�`�̎��
	Type GetType() const override { return Type::Box; }

	// ���W��ύX����
	void AddPosition(const vec3& translate) override
	{
		box.position += translate;
	}

	// ���W�ϊ������R���C�_�[���擾����
	ColliderPtr GetTransformedCollider(const mat4& transform) const override
	{
		// ���W�ϊ��s��𕪉�
		vec3 translate; // ���g�p
		vec3 scale;
		mat3 rotation;
		Decompose(transform, translate, scale, rotation);

		// ���W�ϊ������R�s�[���쐬
		auto p = std::make_shared<BoxCollider>();

		p->box.position = vec3(transform * vec4(box.position, 1));
		for (int i = 0; i < 3; ++i) {
			p->box.axis[i] = rotation * box.axis[i]; // ���x�N�g��
			p->box.scale[i] = box.scale[i] * scale[i]; // �g�嗦
			
		}
		return p;
	}

	// �}�`���擾����
	const Box& GetShape() const { return box; }

	Box box =
	{
	 { 0, 0, 0 },
	 { { 1, 0, 0}, { 0, 1, 0 }, { 0, 0, 1} },
	 { 1, 1, 1 }
	};
};
using BoxColliderPtr = std::shared_ptr<BoxCollider>;

#endif // BOXCOLLIDER_H_INCLUDED