#include "GoalObject.h"

void GoalObject::Awake()
{
	// ゴールブロックモデルを設定
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	//指定のポジションに移動させておく
	owner->position = vec3{ 18, 1.8f, -105 };
	owner->staticMesh = engine->GetStaticMesh("Res/MeshData/DeathForest/GoalBlock/GoalBlock.obj");

	// コライダーを設定
	// BOXコライダーを配置
	for (float i = 0; i < 2; i++)
	{
		auto collider = owner->AddComponent<BoxCollider>();
		collider->box.scale = { 3,3,3 };
		collider->box.position.x = 3.0f * i;
	}
}


	