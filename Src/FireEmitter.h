#ifndef FIREEMITTER_H_INCLUDED
#define FIREEMITTER_H_INCLUDED
#include "Engine/Billboard.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/GameObject.h"
#include "Engine/SphereCollider.h"

/**
* 炎の粒子
*/
class FireParticle : public Component
{
public:
	virtual void Awake() override
	{
		GameObject* owner = GetOwner();
		Engine* engine = owner->GetEngine();
		owner->AddComponent<Billboard>(); // ビルボード化
		owner->staticMesh = engine->GetStaticMesh("plane_xy");
		owner->materials = CloneMaterialList(owner->staticMesh);
		owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/particle_fire.tga");
		owner->materials[0]->baseColor = { 1, 0, 0, 1 }; // ライトの影響をなくす
		owner->materials[0]->emission = { 15.0f, 0.0f, 0.0f }; // 強い赤
		owner->renderQueue = RenderQueue_transparent; // 半透明キューで描画
		owner->scale = vec3(0.25f);
	}

	virtual void Update(float deltaTime) override
	{
		// 生存期間を過ぎたら自身を削除
		GameObject* owner = GetOwner();
		if (lifespan <= 0) {
			owner->Destroy();
			return;

		}

		// 経過時間に応じて位置とかを変更
		owner->position.y += 2 * deltaTime; // 上に移動
		owner->scale += vec3(0.75f * deltaTime); // 徐々に拡大
		owner->color.w -= deltaTime; // 徐々に透明化
		lifespan -= deltaTime; // 生存期間を減らす
	}

	float lifespan = 1; // 生存期間
};

/**
* 炎粒子の発生装置
*/
class FireEmitter : public Component
{
public:


	virtual void Update(float deltaTime) override
	{
		// 一定時間ごとに炎粒子を発生させる
		timer += deltaTime;
		if (timer >= 0.2f) {
			timer -= 0.2f;
			GameObject* owner = GetOwner();
			Engine* engine = owner->GetEngine();
			auto fire = engine->Create<GameObject>("fire", "fire", owner->position);
			fire->AddComponent<FireParticle>();

		} // if timer
	}

	float timer = 0; // 粒子発生タイマー
};

#endif // FIREEMITTER_H_INCLUDED