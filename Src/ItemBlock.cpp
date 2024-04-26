#include "ItemBlock.h"

void ItemBlock::Awake()
{
	auto owner = GetOwner();
	auto engine = owner->GetEngine();
	owner->staticMesh = engine->GetStaticMesh("Res/MeshData/Athletic/Block/Block.obj");

	// �R���C�_�[��ݒ�
	auto collider = owner->AddComponent<SphereCollider>();
	collider->sphere.radius = 0.5f;
}

void ItemBlock::Update(float deltaTime)
{
	GameObject* owner = GetOwner();
	auto engine = owner->GetEngine();
	// alive�ȊO�̏�Ԃ͈ړ��ł��Ȃ�
	if (IBstate == ItemBlockState::Offswitch)
	{
		//�v���C���[���G�ꂽ������ĂȂ��Ȃ�A
		// �ǐՃG�l�~�[���o������t���O��On�ɂ���
		owner->Destroy();
		isActive = true;
	}
}


void ItemBlock::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	// dead��goal,stop�ȊO�̏�Ԃ͏Փ˂ɔ������Ȃ�
	if (IBstate == ItemBlockState::Offswitch)
	{
		return;
	}

	// player�ɐG�ꂽ��X�C�b�`���I�t�ɂ��钆�g��
	const auto& name = other->GetOwner()->name;
	if (name == "player") {
		IBstate = ItemBlockState::Offswitch;
	}

}
