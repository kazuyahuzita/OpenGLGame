/**
* @file Collision.h
*/
#ifndef COLLISION_H_INCLUDED
 #define COLLISION_H_INCLUDED
 #include "VecMath.h"

/**
* 軸平行境界ボックス
* 
* 全ての面が軸と平行な交差判定が高速な奴
* 
*/
struct AABB
 {
  vec3 min;
  vec3 max;
  };


/**
* 球体
*/
struct Sphere
{
	vec3 position; // 中心座標
	float radius;  // 半径
};

/**
* 有向境界ボックス(OBB)
* 
* 「好きな向きで配置できる直方体」
*/
struct Box
{
	vec3 position; // 中心座標
	vec3 axis[3];  // 軸の向きベクトル
	vec3 scale;    // 拡大率
};
/**
* 光線
*/
struct Ray
{
	vec3 start;     // 光線の始点
	vec3 direction; // 光線の向き
	//光線の向きベクトルは「光線が一定時間にX, Y, Zの各軸方向に移動する比率」
};


bool Intersect(const AABB & a, const AABB & b, vec3 & penetration);
bool Intersect(const Sphere & a, const Sphere & b, vec3 & penetration);
bool Intersect(const AABB & a, const Sphere & b, vec3 & penetration);
bool Intersect(const Box& box, const Sphere& sphere, vec3& penetration);


bool Intersect(const AABB& a, const Ray& b, float& distance);
bool Intersect(const Sphere& a, const Ray& b, float& distance);
bool Intersect(const Box& a, const Ray& b, float& distance);


bool IntersectSlab(float min, float max, float start, float direction,float& tmin, float& tmax);

#endif // COLLISION_H_INCLUDED



