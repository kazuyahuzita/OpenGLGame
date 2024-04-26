#include "MoveEnemy.h"

void MoveEnemy::Awake()
{

	// �G���f����ݒ�
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	//owner->scale = vec3{3,3,3};
	
	// �R���C�_�[��ݒ�
	// ���̃R���C�_�[���㉺�ɕ��ׂĒu�����ƂŁA�c���̏Փˌ`����Č�
		auto collider = owner->AddComponent<SphereCollider>();
		collider->sphere.radius = 0.5f;
		collider->sphere.position.y = 0.5f;

}

void MoveEnemy::Update(float deltaTime)
{
	if (moveEnemyState != MoveEnemyState::alive) return;

	auto owner = GetOwner();
}

void MoveEnemy::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	if (name == "player") {
		moveEnemyState = MoveEnemyState::hit;

	}
	else if (name =="bullet")
	{
		moveEnemyState = MoveEnemyState::dead;
		self->Destroy();
		self->OnDestroy();
	}


}
/*
* position�ňʒu���w�肵�ǐՂ���
*/
void MoveEnemy::Tracking(vec3 Position, float deltaTime)
{
	if (moveEnemyState != MoveEnemyState::alive) return;

	auto owner = GetOwner();
	const Engine* engine = owner->GetEngine();
	const GameObject& camera = engine->GetMainCamera();
	//player�̕������Ă���悤�ɂ���
	owner->rotation.y = camera.rotation.y;

	//  position�Őݒ肵���I�u�W�F�N�g�̕����x�N�g�����v�Z���A���K������
	//�������Ɍv�Z����Ƃ��܂��v�Z����Ȃ��̂ł������蕪����
	float directionX = Position.x - owner->position.x;
	float directionY = Position.y - owner->position.y;
	float directionZ = Position.z - owner->position.z;
	float magnitude = std::sqrt(directionX * directionX+ directionY * directionY + directionZ * directionZ);
	if (magnitude != 0) {
		directionX /= magnitude;
		directionY /= magnitude;
		directionZ /= magnitude;
	}
	// position�Őݒ肵���I�u�W�F�N�g�Ɍ������Ĉړ�������
	owner->position.x += directionX * speed * deltaTime;
	owner->position.y += directionY * speed * deltaTime;
	owner->position.z += directionZ * speed * deltaTime;

}
/*
* positionX,Z�Ƀv���C���[�̈ʒu���w�肵�A
* range�Ŏw�肵���͈͂ɓ��������ǂ����������Ă����
* true:������	false :�����ĂȂ�
*/
bool MoveEnemy::IsPlayerInRange(float positionX, float positionZ, float range) {
	auto owner = GetOwner();

	float deltaX = positionX - owner->position.x;
	float deltaY = positionZ - owner->position.z;
	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	return distance <= range;
}
