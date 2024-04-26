#include "GateEffect.h"

void GateEffectParticle::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	owner->AddComponent<Billboard>(); // ビルボード化
	owner->staticMesh = engine->GetStaticMesh("plane_xy");
	owner->materials = CloneMaterialList(owner->staticMesh);
	owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/particle_gate.tga");
	owner->materials[0]->baseColor = { 1, 0, 0, 1 }; // ライトの影響をなくす
	owner->materials[0]->emission = { 1.0f, 1.0f, 1.0f }; // 
	owner->renderQueue = RenderQueue_transparent; // 半透明キューで描画
	owner->scale = vec3(1.0f);
}

void GateEffectParticle::Update(float deltaTime)
{
	// 生存期間を過ぎたら自身を削除
	GameObject* owner = GetOwner();
	if (lifespan <= 0) {
		owner->Destroy();
		return;

	}
	//キャラクターの前
	const vec3 front = { cos(owner->rotation.y), 0, -sin(owner->rotation.y) };

	// 経過時間に応じて位置とかを変更
	owner->position.z += front.z * 2 * deltaTime; // プレイヤー側に移動
	owner->scale += vec3(10.0f * deltaTime); // 徐々に拡大
	owner->color.w -= deltaTime; // 徐々に透明化
	lifespan -= deltaTime; // 生存期間を減らす
}

void GateEffectSmoke::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	owner->AddComponent<Billboard>(); // ビルボード化
	owner->staticMesh = engine->GetStaticMesh("plane_xy");
	owner->materials = CloneMaterialList(owner->staticMesh);
	owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/effect_smoke.tga");
	owner->materials[0]->baseColor = { 1, 0, 0, 0.5 }; // ライトの影響をなくす
	owner->materials[0]->emission = { 1.0f, 1.0f, 1.0f }; // 
	owner->renderQueue = RenderQueue_transparent; // 半透明キューで描画
	owner->scale = vec3(0.5f);

}

void GateEffectSmoke::Update(float deltaTime)
{
	timer += deltaTime;

	if (timer > interval) {
		return;

	}
	timer -= interval;

	GameObject* owner = GetOwner();
	if (lifespan <= 0) {
		owner->Destroy();
		return;

	}

	// 経過時間に応じて位置とかを変更
	owner->rotation.y +=  2 * deltaTime; // 回転
	lifespan -= deltaTime; // 生存期間を減らす

}

void GateEffect::Update(float deltaTime)
{
	//alive以外だと何もしないようにする
	if (gateEffectState != GateEffectState::alive)return;

	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// ゲートスモークを発生させる位置をランダムに選択
	vec3 pos = owner->position;
	pos.x += Random::Range(-range.x, range.x);
	pos.y += Random::Range(-range.y, range.y); // いいかんじに表示されるように高さを調整
	pos.z += 0.1f;

	auto gateEffectSmoke =
		engine->Create<GameObject>("gateEffectSmoke", "gateEffectSmoke", pos);
	gateEffectSmoke->AddComponent<GateEffectSmoke>();

	// 一定時間ごとに粒子を発生させる
	timer += deltaTime;
	if (timer >= 0.2f) {
		timer -= 0.2f;
		auto gateEffectParticle = 
			engine->Create<GameObject>("gateEffectParticle", "gateEffectParticle", owner->position);
		gateEffectParticle->AddComponent<GateEffectParticle>();
	} // if timer

}

