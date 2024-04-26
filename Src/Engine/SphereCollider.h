/**
* @file SphereCollider.h
*/
#ifndef SPHERECOLLIDER_H_INCLUDED
 #define SPHERECOLLIDER_H_INCLUDED
 #include "Collider.h"
 #include "Collision.h"

/**
* 球体コライダー
*/
class SphereCollider : public Collider
{
public:
	SphereCollider() = default;
	virtual ~SphereCollider() = default;

	// 図形の種類
	Type GetType() const override { return Type::Sphere; }

	// 座標を変更する
	void AddPosition(const vec3& translate) override
	{
		sphere.position += translate;
	}

	// 座標変換したコライダーを取得する
	ColliderPtr GetTransformedCollider(const mat4& transform) const override
	{
		// 中心座標を座標変換する
		auto p = std::make_shared<SphereCollider>();
		p->sphere.position = vec3(transform * vec4(sphere.position, 1));

		// 球体の拡大率はオブジェクトのXYZ拡大率のうち最大のものとする(Unity準拠)
		const vec3 scale = ExtractScale(transform);
		const float maxScale = std::max({ scale.x, scale.y, scale.z });
		p->sphere.radius = sphere.radius * maxScale;
		return p;
	}

	// 図形を取得する
	const Sphere& GetShape() const { return sphere; }

	Sphere sphere = { vec3(0), 1 }; // 図形(球体)
};
#endif// SPHERECOLLIDER_H_INCLUDED