#include "MoveExplorationScene.h"

void MoveExplorationScene::Awake()
{
	auto owner = GetOwner();
	auto engine = owner->GetEngine();

	// コライダーを設定
	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1.0f, 2.0f, 0.25f };
	collider->box.position.y = 0.5f;
	collider->isStatic = true;
	collider->isTrigger = true;
}

void MoveExplorationScene::Update(float deltaTime)
{
	//状態がInsideになったら何もしないようにする
	if (mesState == MoveExplorationSceneState::Inside)
		return;
}

void MoveExplorationScene::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	if (name == "player") {
		mesState = MoveExplorationSceneState::Inside;
	}

}
