#include "SpawnEnemy.h"


void SpawnEnemy::Awake()
{

	// 敵モデルを設定
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();

	owner->position = vec3{ 0,-2.0f,0 };
	// コライダーを設定
	// 球体コライダーを上下に並べて置くことで、縦長の衝突形状を再現
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

	// プレイヤーへの方向ベクトルを計算し、正規化する
	float directionX = positionX - owner->position.x;
	float directionZ = positionZ - owner->position.z;
	float magnitude = std::sqrt(directionX * directionX + directionZ * directionZ);
	if (magnitude != 0) {
		directionX /= magnitude;
		directionZ /= magnitude;
	}
	// 敵をプレイヤーに向かって移動させる
	owner->position.x += directionX * speed* deltaTime;
	owner->position.z += directionZ * speed* deltaTime;

}
/*
* positionX,Zにプレイヤーの位置を指定し、
* rangeで指定した範囲に入ったかどうかを教えてくれる
* true:入った	false :入ってない
*/
bool SpawnEnemy::IsPlayerInRange(float positionX, float positionZ, float range)
{
	auto owner = GetOwner();

	float deltaX = positionX - owner->position.x;
	float deltaZ = positionZ - owner->position.z;
	float distance = sqrt(deltaX * deltaX + deltaZ * deltaZ);

	return distance <= range;
}
