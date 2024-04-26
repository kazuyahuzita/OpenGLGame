#include "MoveEnemy.h"

void MoveEnemy::Awake()
{

	// 敵モデルを設定
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	//owner->scale = vec3{3,3,3};
	
	// コライダーを設定
	// 球体コライダーを上下に並べて置くことで、縦長の衝突形状を再現
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
* positionで位置を指定し追跡する
*/
void MoveEnemy::Tracking(vec3 Position, float deltaTime)
{
	if (moveEnemyState != MoveEnemyState::alive) return;

	auto owner = GetOwner();
	const Engine* engine = owner->GetEngine();
	const GameObject& camera = engine->GetMainCamera();
	//playerの方を見ているようにする
	owner->rotation.y = camera.rotation.y;

	//  positionで設定したオブジェクトの方向ベクトルを計算し、正規化する
	//分けずに計算するとうまく計算されないのでしっかり分ける
	float directionX = Position.x - owner->position.x;
	float directionY = Position.y - owner->position.y;
	float directionZ = Position.z - owner->position.z;
	float magnitude = std::sqrt(directionX * directionX+ directionY * directionY + directionZ * directionZ);
	if (magnitude != 0) {
		directionX /= magnitude;
		directionY /= magnitude;
		directionZ /= magnitude;
	}
	// positionで設定したオブジェクトに向かって移動させる
	owner->position.x += directionX * speed * deltaTime;
	owner->position.y += directionY * speed * deltaTime;
	owner->position.z += directionZ * speed * deltaTime;

}
/*
* positionX,Zにプレイヤーの位置を指定し、
* rangeで指定した範囲に入ったかどうかを教えてくれる
* true:入った	false :入ってない
*/
bool MoveEnemy::IsPlayerInRange(float positionX, float positionZ, float range) {
	auto owner = GetOwner();

	float deltaX = positionX - owner->position.x;
	float deltaY = positionZ - owner->position.z;
	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	return distance <= range;
}
