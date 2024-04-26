#include "MoveBlock.h"

///<summary>
/// ヘッダーに書いてあるのと同義でcppも合わせています
/// 用途に合わせていけたら良いと考えてます
///</summary>


void XaxisMoveBlock::Awake()
{
	auto owner = GetOwner();

	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1,0.5f,1 };
	collider->isStatic = true;
}

void XaxisMoveBlock::Update(float deltaTime)
{
	auto owner = GetOwner();
	timer += deltaTime;
	owner->position.x += sin(timer * cycle) * range * deltaTime;

}
void ZaxisMoveBlock::Awake()
{
	auto owner = GetOwner();

	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1,0.5f,1 };
	collider->isStatic = true;
}

void ZaxisMoveBlock::Update(float deltaTime)
{
	auto owner = GetOwner();
	timer += deltaTime;

	owner->position.z += sin(timer * cycle) * range * deltaTime;

}

void VerticalMoveBlock::Awake()
{
	auto owner = GetOwner();

	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1,0.5f,1 };
	collider->isStatic = true;

}

void VerticalMoveBlock::Update(float deltaTime)
{
	auto owner = GetOwner();
	timer += deltaTime;
	owner->position.y += sin(timer * cycle) * range *deltaTime;

}
