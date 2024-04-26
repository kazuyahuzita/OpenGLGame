#ifndef SLIDINGMOVEMENT_H_INCLUDED
 #define SLIDINGMOVEMENT_H_INCLUDED
 #include "Component.h"
 #include "GameObject.h"
#include <random>

/**
* つるつると滑る床作り
*/
class SlidingMovement : public Component
{
public:
	// コンストラクタ・デストラクタ
	SlidingMovement() = default;
	virtual ~SlidingMovement() = default;

	// 毎フレーム1回呼び出される
	virtual void Update(float deltaTime) override
	{
		auto owner = GetOwner();
		if (owner->isSlided ==false) {
			return;
		}
		if (owner->isSlided) {
			
			SlideMove(deltaTime);
			// アイス床の接地フラグが立っていたら、滑っていく
			//std::maxは両方の値の大きい方を返す関数
			velocity.x = std::max(velocity.x, 0.0f);
			
		}
		// 速度を座標に反映する
		owner->position += velocity * deltaTime;
	}
	void SlideMove(float deltaTime) {

		//簡単な数値だと滑る床に振り回される感じ
		//になるので、複雑な感じにしている.

		std::random_device rd;
		std::mt19937 gen(rd());
		//-1.0fから1.0の範囲内で実数を一様分布で生成;
		std::uniform_real_distribution<float> dis(-2.0f, 2.0f);

		float moveX = dis(gen);
		float moveZ = dis(gen);

		// 移動量を座標に加算
		velocity.x += moveX* deltaTime;
		velocity.z += moveZ* deltaTime;
	}
	vec3 velocity = { 0, 0, 0 }; // 速度
public:
};
//クラスのコンストラクタや関数の継承
using SlidingMovementPtr = std::shared_ptr<SlidingMovement>;

#endif // SLIDINGMOVEMENT_H_INCLUDED