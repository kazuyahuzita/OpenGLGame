#ifndef BIKE_H_INCLUDED
#define BIKE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
#include "Engine/BoxCollider.h"
#include "Engine/Engine.h"
/**
* �o�C�N�̃I�u�W�F�N�g��\�����邾���̃I�u�W�F�N�g
* �u���Ă��邾���̏��
*/
class Bike : public Component
{
public:
	Bike() = default;
	virtual ~Bike() = default;

	virtual void Awake() override;
};

#endif // BIKE_H_INCLUDED