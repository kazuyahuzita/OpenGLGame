#ifndef FIREEMITTER_H_INCLUDED
#define FIREEMITTER_H_INCLUDED
#include "Engine/Billboard.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/GameObject.h"
#include "Engine/SphereCollider.h"

/**
* ���̗��q
*/
class FireParticle : public Component
{
public:
	virtual void Awake() override
	{
		GameObject* owner = GetOwner();
		Engine* engine = owner->GetEngine();
		owner->AddComponent<Billboard>(); // �r���{�[�h��
		owner->staticMesh = engine->GetStaticMesh("plane_xy");
		owner->materials = CloneMaterialList(owner->staticMesh);
		owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/particle_fire.tga");
		owner->materials[0]->baseColor = { 1, 0, 0, 1 }; // ���C�g�̉e�����Ȃ���
		owner->materials[0]->emission = { 15.0f, 0.0f, 0.0f }; // ������
		owner->renderQueue = RenderQueue_transparent; // �������L���[�ŕ`��
		owner->scale = vec3(0.25f);
	}

	virtual void Update(float deltaTime) override
	{
		// �������Ԃ��߂����玩�g���폜
		GameObject* owner = GetOwner();
		if (lifespan <= 0) {
			owner->Destroy();
			return;

		}

		// �o�ߎ��Ԃɉ����Ĉʒu�Ƃ���ύX
		owner->position.y += 2 * deltaTime; // ��Ɉړ�
		owner->scale += vec3(0.75f * deltaTime); // ���X�Ɋg��
		owner->color.w -= deltaTime; // ���X�ɓ�����
		lifespan -= deltaTime; // �������Ԃ����炷
	}

	float lifespan = 1; // ��������
};

/**
* �����q�̔������u
*/
class FireEmitter : public Component
{
public:


	virtual void Update(float deltaTime) override
	{
		// ��莞�Ԃ��Ƃɉ����q�𔭐�������
		timer += deltaTime;
		if (timer >= 0.2f) {
			timer -= 0.2f;
			GameObject* owner = GetOwner();
			Engine* engine = owner->GetEngine();
			auto fire = engine->Create<GameObject>("fire", "fire", owner->position);
			fire->AddComponent<FireParticle>();

		} // if timer
	}

	float timer = 0; // ���q�����^�C�}�[
};

#endif // FIREEMITTER_H_INCLUDED