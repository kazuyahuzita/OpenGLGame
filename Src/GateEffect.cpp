#include "GateEffect.h"

void GateEffectParticle::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	owner->AddComponent<Billboard>(); // �r���{�[�h��
	owner->staticMesh = engine->GetStaticMesh("plane_xy");
	owner->materials = CloneMaterialList(owner->staticMesh);
	owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/particle_gate.tga");
	owner->materials[0]->baseColor = { 1, 0, 0, 1 }; // ���C�g�̉e�����Ȃ���
	owner->materials[0]->emission = { 1.0f, 1.0f, 1.0f }; // 
	owner->renderQueue = RenderQueue_transparent; // �������L���[�ŕ`��
	owner->scale = vec3(1.0f);
}

void GateEffectParticle::Update(float deltaTime)
{
	// �������Ԃ��߂����玩�g���폜
	GameObject* owner = GetOwner();
	if (lifespan <= 0) {
		owner->Destroy();
		return;

	}
	//�L�����N�^�[�̑O
	const vec3 front = { cos(owner->rotation.y), 0, -sin(owner->rotation.y) };

	// �o�ߎ��Ԃɉ����Ĉʒu�Ƃ���ύX
	owner->position.z += front.z * 2 * deltaTime; // �v���C���[���Ɉړ�
	owner->scale += vec3(10.0f * deltaTime); // ���X�Ɋg��
	owner->color.w -= deltaTime; // ���X�ɓ�����
	lifespan -= deltaTime; // �������Ԃ����炷
}

void GateEffectSmoke::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	owner->AddComponent<Billboard>(); // �r���{�[�h��
	owner->staticMesh = engine->GetStaticMesh("plane_xy");
	owner->materials = CloneMaterialList(owner->staticMesh);
	owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/effect_smoke.tga");
	owner->materials[0]->baseColor = { 1, 0, 0, 0.5 }; // ���C�g�̉e�����Ȃ���
	owner->materials[0]->emission = { 1.0f, 1.0f, 1.0f }; // 
	owner->renderQueue = RenderQueue_transparent; // �������L���[�ŕ`��
	owner->scale = vec3(0.5f);

}

void GateEffectSmoke::Update(float deltaTime)
{
	timer += deltaTime;

	if (timer > interval) {
		return;

	}
	timer -= interval;

	GameObject* owner = GetOwner();
	if (lifespan <= 0) {
		owner->Destroy();
		return;

	}

	// �o�ߎ��Ԃɉ����Ĉʒu�Ƃ���ύX
	owner->rotation.y +=  2 * deltaTime; // ��]
	lifespan -= deltaTime; // �������Ԃ����炷

}

void GateEffect::Update(float deltaTime)
{
	//alive�ȊO���Ɖ������Ȃ��悤�ɂ���
	if (gateEffectState != GateEffectState::alive)return;

	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// �Q�[�g�X���[�N�𔭐�������ʒu�������_���ɑI��
	vec3 pos = owner->position;
	pos.x += Random::Range(-range.x, range.x);
	pos.y += Random::Range(-range.y, range.y); // �������񂶂ɕ\�������悤�ɍ����𒲐�
	pos.z += 0.1f;

	auto gateEffectSmoke =
		engine->Create<GameObject>("gateEffectSmoke", "gateEffectSmoke", pos);
	gateEffectSmoke->AddComponent<GateEffectSmoke>();

	// ��莞�Ԃ��Ƃɗ��q�𔭐�������
	timer += deltaTime;
	if (timer >= 0.2f) {
		timer -= 0.2f;
		auto gateEffectParticle = 
			engine->Create<GameObject>("gateEffectParticle", "gateEffectParticle", owner->position);
		gateEffectParticle->AddComponent<GateEffectParticle>();
	} // if timer

}

