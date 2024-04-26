#include "SpawnEnemy.h"


void SpawnEnemy::Awake()
{

	// �G���f����ݒ�
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();

	owner->position = vec3{ 0,-2.0f,0 };
	// �R���C�_�[��ݒ�
	// ���̃R���C�_�[���㉺�ɕ��ׂĒu�����ƂŁA�c���̏Փˌ`����Č�
	for (float i = 0; i < 1; ++i) {
		auto collider = owner->AddComponent<SphereCollider>();
		collider->sphere.radius = 0.5f;
		collider->sphere.position.y = -0.5f * i;

	}
}

void SpawnEnemy::Update(float deltaTime)
{

}

void SpawnEnemy::Tracking(float positionX, float positionZ,float deltaTime)
{
	auto owner = GetOwner();
	const Engine * engine = owner->GetEngine();
	const GameObject & camera = engine->GetMainCamera();
	owner->rotation.y = camera.rotation.y;

	// �v���C���[�ւ̕����x�N�g�����v�Z���A���K������
	float directionX = positionX - owner->position.x;
	float directionZ = positionZ - owner->position.z;
	float magnitude = std::sqrt(directionX * directionX + directionZ * directionZ);
	if (magnitude != 0) {
		directionX /= magnitude;
		directionZ /= magnitude;
	}
	// �G���v���C���[�Ɍ������Ĉړ�������
	owner->position.x += directionX * speed* deltaTime;
	owner->position.z += directionZ * speed* deltaTime;

}
/*
* positionX,Z�Ƀv���C���[�̈ʒu���w�肵�A
* range�Ŏw�肵���͈͂ɓ��������ǂ����������Ă����
* true:������	false :�����ĂȂ�
*/
bool SpawnEnemy::IsPlayerInRange(float positionX, float positionZ, float range)
{
	auto owner = GetOwner();

	float deltaX = positionX - owner->position.x;
	float deltaZ = positionZ - owner->position.z;
	float distance = sqrt(deltaX * deltaX + deltaZ * deltaZ);

	return distance <= range;
}
