#include "Item.h"
#include "AudioSettings.h"
#include "Engine/AabbCollider.h"
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/Engine.h"

void GemItem::Awake()
{
	GameObject* owner = GetOwner();
	// �R���C�_�[��ݒ�
	auto collider = owner->AddComponent<AabbCollider>();
	collider->aabb = { { -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f } };
	collider->isStatic = true;

}

void GemItem::Update(float deltaTime)
{
	auto owner = GetOwner();

	// alive�ȊO�̏�Ԃ͈ړ��ł��Ȃ�
	if (gemItemState != GemItemState::alive)
	{
		return;
	}

	// ���쐧��p�^�C�}�[���X�V
	timer += deltaTime;

	//// �^�C�}�[�ɂ���đ̂������E�ɗh�炷
	constexpr float baseX = 0;    // ��̍���(m)
	constexpr float range = 0.1f; // �h�炷�����͈̔�(m)
	constexpr float cycle = 5;    // �h�炷�Ԋu(�b)

	owner->rotation.x = baseX + sin(timer * cycle) * range;

}

void GemItem::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	auto owner = GetOwner();

	// alive�ȊO�̏�Ԃ͏Փ˂ɔ������Ȃ�
	if (gemItemState != GemItemState::alive) {
		owner->Destroy();
		return;
	}

	const auto& name = other->GetOwner()->name;
	if (name == "player") {
		gemItemState = GemItemState::dead;
		EasyAudio::PlayOneShot(SE::GetGem);
	}

}
