#include "MoveStage1Scene.h"

void MoveStage1Scene::Awake()
{
	auto owner = GetOwner();

	// �R���C�_�[��ݒ�
	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1.0f, 2.0f, 0.25f };
	collider->box.position.y = 0.5f;
	collider->isStatic = true;
	collider->isTrigger = true;
}

void MoveStage1Scene::Update(float deltaTime)
{
	//��Ԃ�Inside�ɂȂ����牽�����Ȃ��悤�ɂ���
	if (ms1sState == MoveStage1SceneState::Inside)
		return;
}

void MoveStage1Scene::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	//�v���C���[���G�ꂽ���Ԃ�Inside�ɕύX
	if (name == "player") {
		ms1sState = MoveStage1SceneState::Inside;
	}

}
