#include "MoveStage1Scene.h"

void MoveStage1Scene::Awake()
{
	auto owner = GetOwner();

	// コライダーを設定
	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1.0f, 2.0f, 0.25f };
	collider->box.position.y = 0.5f;
	collider->isStatic = true;
	collider->isTrigger = true;
}

void MoveStage1Scene::Update(float deltaTime)
{
	//状態がInsideになったら何もしないようにする
	if (ms1sState == MoveStage1SceneState::Inside)
		return;
}

void MoveStage1Scene::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	//プレイヤーが触れたら状態をInsideに変更
	if (name == "player") {
		ms1sState = MoveStage1SceneState::Inside;
	}

}
