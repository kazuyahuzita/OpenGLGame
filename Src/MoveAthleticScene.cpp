#include "MoveAthleticScene.h"

void MoveAthleticScene::Awake()
{
	auto owner = GetOwner();

	// �R���C�_�[��ݒ�
	auto collider = owner->AddComponent<BoxCollider>();
	collider->box.scale = vec3{ 1.0f, 2.0f, 0.25f };
	collider->box.position.y = 0.5f;
	collider->isStatic = true;
}

void MoveAthleticScene::Update(float deltaTime)
{
	//��Ԃ�Inside�ɂȂ����牽�����Ȃ��悤�ɂ���
	if (masState == MoveAthleticSceneState::Inside)
		return;
}

void MoveAthleticScene::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	//�v���C���[���G�ꂽ���Ԃ�Inside�ɕύX
	if (name == "player") {
		masState = MoveAthleticSceneState::Inside;
	}

}
