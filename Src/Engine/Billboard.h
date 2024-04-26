#ifndef BILLBOARD_H_INCLUDED
 #define BILLBOARD_H_INCLUDED
 #include "Component.h"
 #include "Engine.h"

/**
* ビルボード化コンポーネント
*/
class Billboard : public Component
{
public:
	Billboard() = default;
	virtual ~Billboard() = default;

	virtual void Update(float deltaTime) override {
		// 常にカメラの方を向くようにメッセージオブジェクトの向きを調整
		GameObject* owner = GetOwner();
		const Engine* engine = owner->GetEngine();
		const GameObject& camera = engine->GetMainCamera();
		owner->rotation.y = camera.rotation.y;

	}
};

#endif // BILLBOARD_H_INCLUDED