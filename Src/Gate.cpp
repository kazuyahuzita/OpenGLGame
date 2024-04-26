#include "Gate.h"

void Gate::Awake()
{
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	owner->staticMesh = engine->GetStaticMesh("Res/MeshData/Athletic/Gate/Gate.obj");

	// �R���C�_�[��ݒ�
	auto GateCollider = owner->AddComponent<BoxCollider>();
	GateCollider->box.scale = vec3{ 2.0f, 2.0f, 0.5f };
	GateCollider->box.position.y = 0.5f;
	GateCollider->isStatic = true;
}

void Gate::Update(float deltaTime)
{
	//�Q�[�g���g�̏�Ԃ�alive�ȊO���Ɣ������Ȃ�
	if (gateState != GateState::alive)return;

	auto owner = GetOwner();

	//moveTimer�����̒l�ɂȂ�����~�܂�悤�ɂ��Ă���
	//���̂Ƃ�����K�V�[���ȊO�Ŏg���\�肪�Ȃ��̂ł���ɍ��킹���d�l�ɂ��Ă���
	if (moveTimer >= 20.0f)
	{
		return;
	}
	owner->position.y += 0.2f * deltaTime;
	moveTimer+=deltaTime;
}
