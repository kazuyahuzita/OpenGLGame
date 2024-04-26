#include "Tornado.h"

void Tornado::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	owner->AddComponent<Billboard>(); // ビルボード化
	owner->staticMesh = engine->GetStaticMesh("plane_xy");
	owner->materials = CloneMaterialList(owner->staticMesh);
	owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/particle_gate.tga");
	owner->materials[0]->baseColor = { 1, 1, 1, 2 }; // ライトの影響をなくす
	owner->materials[0]->emission = { 15.0f, 1.0f,15.0f }; // 
	owner->renderQueue = RenderQueue_transparent; // 半透明キューで描画
	owner->scale = vec3(1.5f);
}

void Tornado::Update(float deltaTime)
{
	// 生存期間を過ぎたら自身を削除
	GameObject* owner = GetOwner();
	if (lifespan <= 0) {
		owner->Destroy();
		return;

	}

	// 経過時間に応じて位置とかを変更
	owner->position.y += 2 * deltaTime; // 上に移動
	owner->scale += vec3(1.25f * deltaTime); // 徐々に拡大
	owner->color.w -= deltaTime; // 徐々に透明化
	lifespan -= deltaTime; // 生存期間を減らす
}



void TornadoEffect::Update(float deltaTime)
{
	if (tornadoEffectState != TornadoEffectState::alive)return;

	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// 一定時間ごとに粒子を発生させる
	timer += deltaTime;
	if (timer >= 0.2f) {
		timer -= 0.2f;
		auto tornado =
			engine->Create<GameObject>("tornado", "tornado" ,owner->position);
		tornado->AddComponent<Tornado>();
	} // if timer

}

