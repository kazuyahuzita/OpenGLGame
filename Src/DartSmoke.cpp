#include "DartSmoke.h"

void DartSmoke::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	owner->AddComponent<Billboard>(); // �r���{�[�h��
	owner->staticMesh = engine->GetStaticMesh("plane_xy");
	owner->materials = CloneMaterialList(owner->staticMesh);
	owner->materials[0]->texBaseColor = engine->GetTexture("Res/Effect/effect_smoke.tga");
	owner->materials[0]->baseColor = { 1, 1, 1, 2 }; // ���C�g�̉e�����Ȃ���
	owner->materials[0]->emission = { 1.0f, 1.0f,1.0f }; // 
	owner->renderQueue = RenderQueue_transparent; // �������L���[�ŕ`��
	owner->scale = vec3(0.1f);
}

void DartSmoke::Update(float deltaTime)
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
	owner->position.y += front.y * 2 * deltaTime; // ��Ɉړ�
	owner->scale += vec3(2.0f * deltaTime); // ���X�Ɋg��
	lifespan -= deltaTime; // �������Ԃ����炷
}



void DartSmokeEffect::Update(float deltaTime)
{
	//alive�ȊO���Ɖ������Ȃ�
	if (dartSmokeEffectState != DartSmokeEffectState::alive)return;

	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// ���q�̔���������ʒu�������_���ɑI��
	vec3 pos = owner->position;
	pos.x += Random::Range(-range.x, range.x);
	pos.y += 0.1f; // �������񂶂ɕ\�������悤�ɍ����𒲐�
	pos.z += Random::Range(-range.y, range.y);

	// ��莞�Ԃ��Ƃɗ��q�𔭐�������
	timer += deltaTime;
	if (timer >= 0.2f) {
		timer -= 0.2f;
		auto dartSmoke = 
			engine->Create<GameObject>("dartSmoke", "dartSmoke", pos);
		dartSmoke->AddComponent<DartSmoke>();
	} // if timer

	if (moveTimerCount >= 5.0f) SetState(DartSmokeEffectState::stop);

	moveTimer += deltaTime;
	if (moveTimer >= 4.0f)
	{
		moveTimerCount++;
		moveTimer = 0;
	}

}

