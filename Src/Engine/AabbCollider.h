#ifndef AABBCOLLIDER_H_INCLUDED
 #define AABBCOLLIDER_H_INCLUDED
 #include "Collider.h"
 #include "Collision.h"

/**
* AABBコライダー
*/
class AabbCollider : public Collider
{
public:	AabbCollider() = default;
	  virtual ~AabbCollider() = default;


	  // 図形の種類
	  Type GetType() const override { return Type::AABB; }

	  // 座標を変更する
	  void AddPosition(const vec3& translate) override
	  {
		  aabb.min += translate;
		  aabb.max += translate;
	  }

	  /**
	  * 座標変換したコライダーを取得する
	  *
	  * 回転角度は90度単位で指定すること。それ以外の角度では正しい交差判定が行えない。
	  */
	  ColliderPtr GetTransformedCollider(const mat4& transform) const override
	  {
		  // 座標変換したコピーを作成
		  auto p = std::make_shared<AabbCollider>();
		  p->aabb.min = vec3(transform * vec4(aabb.min, 1));
		  p->aabb.max = vec3(transform * vec4(aabb.max, 1));

		  // minのほうが大きかったら入れ替える
		  for (int i = 0; i < 3; ++i) {
			  if (p->aabb.min[i] > p->aabb.max[i]) {
				  const float tmp = p->aabb.min[i];
				  p->aabb.min[i] = p->aabb.max[i];
				  p->aabb.max[i] = tmp;

			  }

		  }

		  return p;
	  }
	  
	  // 図形を取得する
	  const AABB & GetShape() const { return aabb; }

	  AABB aabb = { { -1, -1, -1 }, { 1, 1, 1 } }; // 図形(軸平行境界ボックス)
};
using AabbColliderPtr = std::shared_ptr<AabbCollider>;

#endif // AABBCOLLIDER_H_INCLUDED