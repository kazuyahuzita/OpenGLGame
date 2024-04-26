#include "DartSmoke.h"

void DartSmoke::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	owner->AddComponent<Billboard>(); // ビルボード化
	owner->staticMesh = engine->GetStaticMesh("plane_xy");
	owner->materials = CloneMaterialList(owner->staticMesh);
	owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/effect_smoke.tga");
	owner->materials[0]->baseColor = { 1, 1, 1, 2 }; // ライトの影響をなくす
	owner->materials[0]->emission = { 1.0f, 1.0f,1.0f }; // 
	owner->renderQueue = RenderQueue_transparent; // 半透明キューで描画
	owner->scale = vec3(0.1f);
}

void DartSmoke::Update(float deltaTime)
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
	owner->position.y += front.y * 2 * deltaTime; // 上に移動
	owner->scale += vec3(2.0f * deltaTime); // 徐々に拡大
	lifespan -= deltaTime; // 生存期間を減らす
}



void DartSmokeEffect::Update(float deltaTime)
{
	//alive以外だと何もしない
	if (dartSmokeEffectState != DartSmokeEffectState::alive)return;

	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// 粒子の発生させる位置をランダムに選択
	vec3 pos = owner->position;
	pos.x += Random::Range(-range.x, range.x);
	pos.y += 0.1f; // いいかんじに表示されるように高さを調整
	pos.z += Random::Range(-range.y, range.y);

	// 一定時間ごとに粒子を発生させる
	timer += deltaTime;
	if (timer >= 0.2f) {
		timer -= 0.2f;
		auto dartSmoke = 
			engine->Create<GameObject>("dartSmoke", "dartSmoke", pos);
		dartSmoke->AddComponent<DartSmoke>();
	} // if timer

	if (moveTimerCount >= 5.0f) SetState(DartSmokeEffectState::stop);

	moveTimer += deltaTime;
	if (moveTimer >= 4.0f)
	{
		moveTimerCount++;
		moveTimer = 0;
	}

}

