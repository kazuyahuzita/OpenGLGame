#ifndef SPAWNENEMY_H_INCLUDED
#define SPAWNENEMY_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/SphereCollider.h"
#include "Engine/VecMath.h"

/**
* ��X���������\��̓G
* 
* ���ʂȊ֐��������Ă���N���X
* �|�W�V������X��Z����ݒ�ł��邾���ɂ��Ă���
* @param void Tracking : positionX,positionZ,deltaTime
* �|�W�V������X��Z���Ƌ�����ݒ�ł���
* �����͈͓���true�A�͈͊O��false��Ԃ��B
* @param bool IsPlayerInRange�@:positionX,positionZ,range
*/
class SpawnEnemy : public Component
{
public:
	SpawnEnemy() = default;
	virtual ~SpawnEnemy() = default;
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	virtual void Tracking(float positionX ,float positionZ,float deltaTime) override;
	bool IsPlayerInRange(float positionX, float positionZ, float range);

private:
	const float speed = 30.0f;
};

#endif // SPAWNENEMY_H_INCLUDED