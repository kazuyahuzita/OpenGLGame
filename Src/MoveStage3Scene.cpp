#include "MoveStage3Scene.h"

void MoveStage3Scene::Awake()
{
	auto owner = GetOwner();
	auto engine = owner ->GetEngine();

	// �R���C�_�[��ݒ�
	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1.0f, 2.0f, 0.25f };
	collider->box.position.y = 0.5f;
	collider->isStatic = true;
	collider->isTrigger = true;

}

void MoveStage3Scene::Update(float deltaTime)
{
	//��Ԃ�Inside�ɂȂ����牽�����Ȃ��悤�ɂ���
	if (ms3sState == MoveStage3SceneState::Inside)
		return;
}

void MoveStage3Scene::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	//�v���C���[���G�ꂽ���Ԃ�Inside�ɕύX
	if (name == "player") {
		ms3sState = MoveStage3SceneState::Inside;
	}

}
