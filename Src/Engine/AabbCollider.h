#ifndef AABBCOLLIDER_H_INCLUDED
 #define AABBCOLLIDER_H_INCLUDED
 #include "Collider.h"
 #include "Collision.h"

/**
* AABB�R���C�_�[
*/
class AabbCollider : public Collider
{
public:	AabbCollider() = default;
	  virtual ~AabbCollider() = default;


	  // �}�`�̎��
	  Type GetType() const override { return Type::AABB; }

	  // ���W��ύX����
	  void AddPosition(const vec3& translate) override
	  {
		  aabb.min += translate;
		  aabb.max += translate;
	  }

	  /**
	  * ���W�ϊ������R���C�_�[���擾����
	  *
	  * ��]�p�x��90�x�P�ʂŎw�肷�邱�ƁB����ȊO�̊p�x�ł͐������������肪�s���Ȃ��B
	  */
	  ColliderPtr GetTransformedCollider(const mat4& transform) const override
	  {
		  // ���W�ϊ������R�s�[���쐬
		  auto p = std::make_shared<AabbCollider>();
		  p->aabb.min = vec3(transform * vec4(aabb.min, 1));
		  p->aabb.max = vec3(transform * vec4(aabb.max, 1));

		  // min�̂ق����傫�����������ւ���
		  for (int i = 0; i < 3; ++i) {
			  if (p->aabb.min[i] > p->aabb.max[i]) {
				  const float tmp = p->aabb.min[i];
				  p->aabb.min[i] = p->aabb.max[i];
				  p->aabb.max[i] = tmp;

			  }

		  }

		  return p;
	  }
	  
	  // �}�`���擾����
	  const AABB & GetShape() const { return aabb; }

	  AABB aabb = { { -1, -1, -1 }, { 1, 1, 1 } }; // �}�`(�����s���E�{�b�N�X)
};
using AabbColliderPtr = std::shared_ptr<AabbCollider>;

#endif // AABBCOLLIDER_H_INCLUDED