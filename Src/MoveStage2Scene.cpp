#include "MoveStage2Scene.h"

void MoveStage2Scene::Awake()
{
	auto owner = GetOwner();

	// �R���C�_�[��ݒ�
	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1.0f, 2.0f, 0.25f };
	collider->box.position.y = 0.5f;
	collider->isStatic = true;
	collider->isTrigger = true;
}

void MoveStage2Scene::Update(float deltaTime)
{
	//��Ԃ�Inside�ɂȂ����牽�����Ȃ��悤�ɂ���
	if (ms2sState == MoveStage2SceneState::Inside)
		return;
}

void MoveStage2Scene::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	//�v���C���[���G�ꂽ���Ԃ�Inside�ɕύX
	if (name == "player") {
		ms2sState = MoveStage2SceneState::Inside;
	}

}
