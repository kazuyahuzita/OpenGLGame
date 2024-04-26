#ifndef BILLBOARD_H_INCLUDED
 #define BILLBOARD_H_INCLUDED
 #include "Component.h"
 #include "Engine.h"

/**
* �r���{�[�h���R���|�[�l���g
*/
class Billboard : public Component
{
public:
	Billboard() = default;
	virtual ~Billboard() = default;

	virtual void Update(float deltaTime) override {
		// ��ɃJ�����̕��������悤�Ƀ��b�Z�[�W�I�u�W�F�N�g�̌����𒲐�
		GameObject* owner = GetOwner();
		const Engine* engine = owner->GetEngine();
		const GameObject& camera = engine->GetMainCamera();
		owner->rotation.y = camera.rotation.y;

	}
};

#endif // BILLBOARD_H_INCLUDED