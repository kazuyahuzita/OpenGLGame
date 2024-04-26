/**
* @file Collider.h
*/
#ifndef COLLIDER_H_INCLUDED
 #define COLLIDER_H_INCLUDED
 #include "Component.h"
 #include "VecMath.h"
 #include <memory>

// 先行宣言
class Collider;
using ColliderPtr = std::shared_ptr<Collider>;

/**
* コライダーの基底クラス
*/
class Collider : public Component
{
public:
	Collider() = default;
	virtual ~Collider() = default;

	//コライダーの実際の図形の種類ごとに返す
	//図形の種類
	enum class Type {
		AABB,   // 軸平行境界ボックス
		Sphere, // 球体
		Box,	//有向境界ボックス
	};
	virtual Type GetType() const = 0;

	// 座標を変更する
	virtual void AddPosition(const vec3& translate) = 0;

	// 座標変換したコライダーを取得する
	virtual ColliderPtr GetTransformedCollider(const mat4& transform) const = 0;

	bool isTrigger = false; // true=重複を許可, false=重複を禁止
	bool isStatic = false;  // true=動かない物体, false=動く物体
};

#endif // COLLIDER_H_INCLUDED