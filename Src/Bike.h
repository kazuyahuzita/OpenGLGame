#ifndef BIKE_H_INCLUDED
#define BIKE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
#include "Engine/BoxCollider.h"
#include "Engine/Engine.h"
/**
* バイクのオブジェクトを表示するだけのオブジェクト
* 置いてあるだけの状態
*/
class Bike : public Component
{
public:
	Bike() = default;
	virtual ~Bike() = default;

	virtual void Awake() override;
};

#endif // BIKE_H_INCLUDED