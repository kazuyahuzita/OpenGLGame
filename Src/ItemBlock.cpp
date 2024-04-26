#include "ItemBlock.h"

void ItemBlock::Awake()
{
	auto owner = GetOwner();
	auto engine = owner->GetEngine();
	owner->staticMesh = engine->GetStaticMesh("Res/MeshData/Athletic/Block/Block.obj");

	// コライダーを設定
	auto collider = owner->AddComponent<SphereCollider>();
	collider->sphere.radius = 0.5f;
}

void ItemBlock::Update(float deltaTime)
{
	GameObject* owner = GetOwner();
	auto engine = owner->GetEngine();
	// alive以外の状態は移動できない
	if (IBstate == ItemBlockState::Offswitch)
	{
		//プレイヤーが触れたら消えてなくなり、
		// 追跡エネミーが出現するフラグをOnにする
		owner->Destroy();
		isActive = true;
	}
}


void ItemBlock::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	// deadかgoal,stop以外の状態は衝突に反応しない
	if (IBstate == ItemBlockState::Offswitch)
	{
		return;
	}

	// playerに触れたらスイッチをオフにする中身に
	const auto& name = other->GetOwner()->name;
	if (name == "player") {
		IBstate = ItemBlockState::Offswitch;
	}

}
