/**
* @file Collision.h
*/
#ifndef COLLISION_H_INCLUDED
 #define COLLISION_H_INCLUDED
 #include "VecMath.h"

/**
* �����s���E�{�b�N�X
* 
* �S�Ă̖ʂ����ƕ��s�Ȍ������肪�����ȓz
* 
*/
struct AABB
 {
  vec3 min;
  vec3 max;
  };


/**
* ����
*/
struct Sphere
{
	vec3 position; // ���S���W
	float radius;  // ���a
};

/**
* �L�����E�{�b�N�X(OBB)
* 
* �u�D���Ȍ����Ŕz�u�ł��钼���́v
*/
struct Box
{
	vec3 position; // ���S���W
	vec3 axis[3];  // ���̌����x�N�g��
	vec3 scale;    // �g�嗦
};
/**
* ����
*/
struct Ray
{
	vec3 start;     // �����̎n�_
	vec3 direction; // �����̌���
	//�����̌����x�N�g���́u��������莞�Ԃ�X, Y, Z�̊e�������Ɉړ�����䗦�v
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



