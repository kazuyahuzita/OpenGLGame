#include "MoveStage3Scene.h"

void MoveStage3Scene::Awake()
{
	auto owner = GetOwner();
	auto engine = owner ->GetEngine();

	// コライダーを設定
	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1.0f, 2.0f, 0.25f };
	collider->box.position.y = 0.5f;
	collider->isStatic = true;
	collider->isTrigger = true;

}

void MoveStage3Scene::Update(float deltaTime)
{
	//状態がInsideになったら何もしないようにする
	if (ms3sState == MoveStage3SceneState::Inside)
		return;
}

void MoveStage3Scene::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	//プレイヤーが触れたら状態をInsideに変更
	if (name == "player") {
		ms3sState = MoveStage3SceneState::Inside;
	}

}
