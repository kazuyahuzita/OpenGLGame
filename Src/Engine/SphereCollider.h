/**
* @file SphereCollider.h
*/
#ifndef SPHERECOLLIDER_H_INCLUDED
 #define SPHERECOLLIDER_H_INCLUDED
 #include "Collider.h"
 #include "Collision.h"

/**
* ���̃R���C�_�[
*/
class SphereCollider : public Collider
{
public:
	SphereCollider() = default;
	virtual ~SphereCollider() = default;

	// �}�`�̎��
	Type GetType() const override { return Type::Sphere; }

	// ���W��ύX����
	void AddPosition(const vec3& translate) override
	{
		sphere.position += translate;
	}

	// ���W�ϊ������R���C�_�[���擾����
	ColliderPtr GetTransformedCollider(const mat4& transform) const override
	{
		// ���S���W�����W�ϊ�����
		auto p = std::make_shared<SphereCollider>();
		p->sphere.position = vec3(transform * vec4(sphere.position, 1));

		// ���̂̊g�嗦�̓I�u�W�F�N�g��XYZ�g�嗦�̂����ő�̂��̂Ƃ���(Unity����)
		const vec3 scale = ExtractScale(transform);
		const float maxScale = std::max({ scale.x, scale.y, scale.z });
		p->sphere.radius = sphere.radius * maxScale;
		return p;
	}

	// �}�`���擾����
	const Sphere& GetShape() const { return sphere; }

	Sphere sphere = { vec3(0), 1 }; // �}�`(����)
};
#endif// SPHERECOLLIDER_H_INCLUDED