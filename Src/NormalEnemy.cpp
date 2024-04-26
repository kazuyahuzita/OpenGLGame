#include "NormalEnemy.h"
 #include "Engine/AabbCollider.h"
 #include "Engine/Engine.h"

/**
* コンポーネントの初期化
*/
void NormalEnemy::Awake()
{
	// 敵モデルを設定
	auto owner = GetOwner();
	owner->staticMesh = 
		owner->GetEngine()->GetStaticMesh("Res/MeshData/Athletic/Enemy/Enemy.obj");
	owner->rotation.y += radians(90);

	// コライダーを設定
	auto collider = owner->AddComponent<AabbCollider>();
	collider->aabb = { { -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f } };
	collider->isStatic = true;
}


/**
* コンポーネントの更新
*
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void NormalEnemy::Update(float deltaTime)
{
	//hitかdeadのステートになったら止まるように設定
	if (normalEnemyState == NormalEnemyState::hit 
		|| normalEnemyState == NormalEnemyState::dead) return;

	auto owner = GetOwner();
	moveTimer += deltaTime;
	// タイマーによって座標を上下に揺らす
	owner->position.y = baseY + sin(moveTimer * cycle) * range;

	//キャラクターの前
	const vec3 front = { cos(owner->rotation.y), 0, -sin(owner->rotation.y) };

	//normalEnemyStateのaliveとstopを交互に繰り返して
	//一定の範囲を常に動く敵にしている
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
			// 左に旋回
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

