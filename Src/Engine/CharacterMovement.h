#ifndef CHARACTERMOVEMENT_H_INCLUDED
 #define CHARACTERMOVEMENT_H_INCLUDED
 #include "Component.h"
 #include "GameObject.h"

/**
* ゲームオブジェクトの速度と重力を扱うコンポーネント
*/
class CharacterMovement : public Component
{
public:
	// コンストラクタ・デストラクタ
	CharacterMovement() = default;
	virtual ~CharacterMovement() = default;

	// 毎フレーム1回呼び出される
	virtual void Update(float deltaTime) override
	{
		auto owner = GetOwner();

		if (owner->isGrounded) {
			      // 接地フラグが立っていたら、下方向への加速度を0にする
				velocity.y = std::max(velocity.y, 0.0f);
			
		}
		else {

			// 重力加速度によって速度を更新する
			velocity.y -= gravity * gravityScale * deltaTime;
		}
		// 速度を座標に反映する
		owner->position += velocity * deltaTime;
	}

	static constexpr float gravity = 9.8f; // 重力加速度
	vec3 velocity = { 0, 0, 0 }; // 速度
public:
	float gravityScale = 0.8f; // 重力の影響を制御する係数
};
using CharacterMovementPtr = std::shared_ptr<CharacterMovement>;

#endif // CHARACTERMOVEMENT_H_INCLUDED