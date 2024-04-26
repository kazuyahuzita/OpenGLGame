#include "NormalEnemy.h"
 #include "Engine/AabbCollider.h"
 #include "Engine/Engine.h"

/**
* �R���|�[�l���g�̏�����
*/
void NormalEnemy::Awake()
{
	// �G���f����ݒ�
	auto owner = GetOwner();
	owner->staticMesh = 
		owner->GetEngine()->GetStaticMesh("Res/MeshData/Athletic/Enemy/Enemy.obj");
	owner->rotation.y += radians(90);

	// �R���C�_�[��ݒ�
	auto collider = owner->AddComponent<AabbCollider>();
	collider->aabb = { { -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f } };
	collider->isStatic = true;
}


/**
* �R���|�[�l���g�̍X�V
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void NormalEnemy::Update(float deltaTime)
{
	//hit��dead�̃X�e�[�g�ɂȂ�����~�܂�悤�ɐݒ�
	if (normalEnemyState == NormalEnemyState::hit 
		|| normalEnemyState == NormalEnemyState::dead) return;

	auto owner = GetOwner();
	moveTimer += deltaTime;
	// �^�C�}�[�ɂ���č��W���㉺�ɗh�炷
	owner->position.y = baseY + sin(moveTimer * cycle) * range;

	//�L�����N�^�[�̑O
	const vec3 front = { cos(owner->rotation.y), 0, -sin(owner->rotation.y) };

	//normalEnemyState��alive��stop�����݂ɌJ��Ԃ���
	//���͈̔͂���ɓ����G�ɂ��Ă���
	switch (normalEnemyState)
	{
	case NormalEnemyState::alive:
		owner->position.x += front.x * speed * deltaTime;
		owner->position.z += front.z * speed * deltaTime;
		if (timerCount > 300)
		{
			normalEnemyState = NormalEnemyState::stop;
			timerCount = 0;
		}
		break;
	case NormalEnemyState::stop:
		if (timerCount > 200)
		{
			// ���ɐ���
			owner->rotation.y += radians(90);
			normalEnemyState = NormalEnemyState::alive;
			timerCount = 0;
		}
		break;
	case NormalEnemyState::hit:

		break;
	case NormalEnemyState::dead:
		break;

	}

	timer += deltaTime;
	if (timer < 100)
	{
		timerCount++;
		timer = 0;
	}

}

void NormalEnemy::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	if (name == "player") {
		normalEnemyState = NormalEnemyState::hit;

	}

}

