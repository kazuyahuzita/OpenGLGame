#include "GoalObject.h"

void GoalObject::Awake()
{
	// �S�[���u���b�N���f����ݒ�
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	//�w��̃|�W�V�����Ɉړ������Ă���
	owner->position = vec3{ 18, 1.8f, -105 };
	owner->staticMesh = engine->GetStaticMesh("Res/MeshData/DeathForest/GoalBlock/GoalBlock.obj");

	// �R���C�_�[��ݒ�
	// BOX�R���C�_�[��z�u
	for (float i = 0; i < 2; i++)
	{
		auto collider = owner->AddComponent<BoxCollider>();
		collider->box.scale = { 3,3,3 };
		collider->box.position.x = 3.0f * i;
	}
}


	