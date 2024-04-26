#include "MoveHorrorScene.h"

void MoveHorrorScene::Awake()
{
	auto owner = GetOwner();

	// コライダーを設定
	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1.0f, 2.0f, 0.25f };
	collider->box.position.y = 0.5f;
	collider->isStatic = true;
}

void MoveHorrorScene::Update(float deltaTime)
{
	//状態がInsideになったら何もしないようにする
	if (mhsState == MoveHorrorSceneState::Inside)
		return;
}

void MoveHorrorScene::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	//プレイヤーが触れたら状態をInsideに変更
	if (name == "player") {
		mhsState = MoveHorrorSceneState::Inside;
	}

}
