#include "Item.h"
#include "AudioSettings.h"
#include "Engine/AabbCollider.h"
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/Engine.h"

void GemItem::Awake()
{
	GameObject* owner = GetOwner();
	// コライダーを設定
	auto collider = owner->AddComponent<AabbCollider>();
	collider->aabb = { { -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f } };
	collider->isStatic = true;

}

void GemItem::Update(float deltaTime)
{
	auto owner = GetOwner();

	// alive以外の状態は移動できない
	if (gemItemState != GemItemState::alive)
	{
		return;
	}

	// 動作制御用タイマーを更新
	timer += deltaTime;

	//// タイマーによって体をを左右に揺らす
	constexpr float baseX = 0;    // 基準の高さ(m)
	constexpr float range = 0.1f; // 揺らす高さの範囲(m)
	constexpr float cycle = 5;    // 揺らす間隔(秒)

	owner->rotation.x = baseX + sin(timer * cycle) * range;

}

void GemItem::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	auto owner = GetOwner();

	// alive以外の状態は衝突に反応しない
	if (gemItemState != GemItemState::alive) {
		owner->Destroy();
		return;
	}

	const auto& name = other->GetOwner()->name;
	if (name == "player") {
		gemItemState = GemItemState::dead;
		EasyAudio::PlayOneShot(SE::GetGem);
	}

}
