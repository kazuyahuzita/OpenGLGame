#include "Gate.h"

void Gate::Awake()
{
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	owner->staticMesh = engine->GetStaticMesh("Res/MeshData/Athletic/Gate/Gate.obj");

	// コライダーを設定
	auto GateCollider = owner->AddComponent<BoxCollider>();
	GateCollider->box.scale = vec3{ 2.0f, 2.0f, 0.5f };
	GateCollider->box.position.y = 0.5f;
	GateCollider->isStatic = true;
}

void Gate::Update(float deltaTime)
{
	//ゲート自身の状態がalive以外だと反応しない
	if (gateState != GateState::alive)return;

	auto owner = GetOwner();

	//moveTimerが一定の値になったら止まるようにしている
	//今のところ練習シーン以外で使う予定がないのでそれに合わせた仕様にしている
	if (moveTimer >= 20.0f)
	{
		return;
	}
	owner->position.y += 0.2f * deltaTime;
	moveTimer+=deltaTime;
}
