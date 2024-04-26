#ifndef FLUORESCENTLIGHT_H_INCLUDED
#define FLUORESCENTLIGHT_H_INCLUDED
#include "Engine/Billboard.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/Light.h"

/**
* 蛍光灯(フルーレセント・ライト)コンポーネント
*/
class FluorescentLight : public Component
{
public:
	FluorescentLight() = default;
	virtual ~FluorescentLight() = default;

	// ゲームオブジェクトに追加された時に呼び出される
	virtual void Awake() override
	{
		// 蛍光灯モデルを設定
		auto owner = GetOwner();
		auto engine = owner->GetEngine();
		owner->staticMesh = engine->GetStaticMesh(
			"Res/MeshData/DeathForest/Light/Light.obj");

		// ぼんやりとした光を追加
		//ビルボードを配置して「空気中のホコリなどによって、
		// 光源付近がぼんやりと明るくなる」表現を追加
		//ビルボードを「境界のはっきりしない光や煙、炎などを表現する手段」として扱えるようにした
		auto glare = engine->Create<GameObject>("glare","glare", owner->position);
		glare->position.y -= 0.25f; // いいかんじ見えるように位置を調整
		glare->renderQueue = RenderQueue_transparent; // 半透明として扱う
		glare->staticMesh = engine->GetStaticMesh("plane_xy");
		glare->materials = CloneMaterialList(glare->staticMesh);
		glare->materials[0]->texBaseColor = engine->GetTexture("Res/glare.tga");
		glare->AddComponent<Billboard>();
		
		// 光源を追加
		auto lightObject = engine->Create<GameObject>("light","light",
								owner->position, owner->rotation);
		lightObject->rotation.x = radians(-90);
		auto light = lightObject->AddComponent<Light>();
		light->color = { 0.95f, 1.0f, 0.98f };
		light->intensity = 3;
		light->radius = 4;
		light->type = Light::Type::spotLight;
	}
};

#endif // FLUORESCENTLIGHT_H_INCLUDED