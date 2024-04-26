#ifndef FIRSTPERSONCAMERA_H_INCLUDED
#define FIRSTPERSONCAMERA_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include <functional>
/**
* カメラをプレイヤーに追従させるコンポーネント
*/
class FirstPersonCamera : public Component
{
public:
	FirstPersonCamera() = default;
	virtual ~FirstPersonCamera() = default;

	virtual void Update(float deltaTime) override {
		GameObject* owner = GetOwner();
		Engine* engine = owner->GetEngine();
		GameObject& camera = engine->GetMainCamera();
		//カメラの位置プレイヤーの位置を同じにしてFPSのようにしている
		camera.position = owner->position;
		camera.rotation = owner->rotation;

	}
};
#endif //FIRSTPERSONCAMERA_H_INCLUDED
