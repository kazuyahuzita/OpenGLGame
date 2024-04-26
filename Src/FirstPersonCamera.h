#ifndef FIRSTPERSONCAMERA_H_INCLUDED
#define FIRSTPERSONCAMERA_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include <functional>
/**
* �J�������v���C���[�ɒǏ]������R���|�[�l���g
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
		//�J�����̈ʒu�v���C���[�̈ʒu�𓯂��ɂ���FPS�̂悤�ɂ��Ă���
		camera.position = owner->position;
		camera.rotation = owner->rotation;

	}
};
#endif //FIRSTPERSONCAMERA_H_INCLUDED
