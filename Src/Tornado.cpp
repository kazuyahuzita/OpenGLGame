#include "Tornado.h"

void Tornado::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	owner->AddComponent<Billboard>(); // �r���{�[�h��
	owner->staticMesh = engine->GetStaticMesh("plane_xy");
	owner->materials = CloneMaterialList(owner->staticMesh);
	owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/particle_gate.tga");
	owner->materials[0]->baseColor = { 1, 1, 1, 2 }; // ���C�g�̉e�����Ȃ���
	owner->materials[0]->emission = { 15.0f, 1.0f,15.0f }; // 
	owner->renderQueue = RenderQueue_transparent; // �������L���[�ŕ`��
	owner->scale = vec3(1.5f);
}

void Tornado::Update(float deltaTime)
{
	// �������Ԃ��߂����玩�g���폜
	GameObject* owner = GetOwner();
	if (lifespan <= 0) {
		owner->Destroy();
		return;

	}

	// �o�ߎ��Ԃɉ����Ĉʒu�Ƃ���ύX
	owner->position.y += 2 * deltaTime; // ��Ɉړ�
	owner->scale += vec3(1.25f * deltaTime); // ���X�Ɋg��
	owner->color.w -= deltaTime; // ���X�ɓ�����
	lifespan -= deltaTime; // �������Ԃ����炷
}



void TornadoEffect::Update(float deltaTime)
{
	if (tornadoEffectState != TornadoEffectState::alive)return;

	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// ��莞�Ԃ��Ƃɗ��q�𔭐�������
	timer += deltaTime;
	if (timer >= 0.2f) {
		timer -= 0.2f;
		auto tornado =
			engine->Create<GameObject>("tornado", "tornado" ,owner->position);
		tornado->AddComponent<Tornado>();
	} // if timer

}

