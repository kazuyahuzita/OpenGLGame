#ifndef FLUORESCENTLIGHT_H_INCLUDED
#define FLUORESCENTLIGHT_H_INCLUDED
#include "Engine/Billboard.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/Light.h"

/**
* �u����(�t���[���Z���g�E���C�g)�R���|�[�l���g
*/
class FluorescentLight : public Component
{
public:
	FluorescentLight() = default;
	virtual ~FluorescentLight() = default;

	// �Q�[���I�u�W�F�N�g�ɒǉ����ꂽ���ɌĂяo�����
	virtual void Awake() override
	{
		// �u�������f����ݒ�
		auto owner = GetOwner();
		auto engine = owner->GetEngine();
		owner->staticMesh = engine->GetStaticMesh(
			"Res/MeshData/DeathForest/Light/Light.obj");

		// �ڂ���Ƃ�������ǉ�
		//�r���{�[�h��z�u���āu��C���̃z�R���Ȃǂɂ���āA
		// �����t�߂��ڂ���Ɩ��邭�Ȃ�v�\����ǉ�
		//�r���{�[�h���u���E�̂͂����肵�Ȃ����≌�A���Ȃǂ�\�������i�v�Ƃ��Ĉ�����悤�ɂ���
		auto glare = engine->Create<GameObject>("glare","glare", owner->position);
		glare->position.y -= 0.25f; // �������񂶌�����悤�Ɉʒu�𒲐�
		glare->renderQueue = RenderQueue_transparent; // �������Ƃ��Ĉ���
		glare->staticMesh = engine->GetStaticMesh("plane_xy");
		glare->materials = CloneMaterialList(glare->staticMesh);
		glare->materials[0]->texBaseColor = engine->GetTexture("Res/glare.tga");
		glare->AddComponent<Billboard>();
		
		// ������ǉ�
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