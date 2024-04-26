#include "Bike.h"

void Bike::Awake()
{
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	//‰ó‚ê‚Ä‚¢‚éŠ´‚ðo‚·‚½‚ß‚É­‚µŒX‚¯‚Ä‚¢‚é
	owner->rotation.x = radians(20);
	owner->rotation.y = radians(15);
	owner->staticMesh = engine->GetStaticMesh("Res/MeshData/DeathForest/Motor/Motor.obj");

	auto collider = owner->AddComponent<BoxCollider>();
	collider->isStatic = true;

}
