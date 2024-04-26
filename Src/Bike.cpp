#include "Bike.h"

void Bike::Awake()
{
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	//���Ă��銴���o�����߂ɏ����X���Ă���
	owner->rotation.x = radians(20);
	owner->rotation.y = radians(15);
	owner->staticMesh = engine->GetStaticMesh("Res/MeshData/DeathForest/Motor/Motor.obj");

	auto collider = owner->AddComponent<BoxCollider>();
	collider->isStatic = true;

}
