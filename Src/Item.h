#ifndef GEMITEM_H_INCLUDED
#define GEMITEM_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
/**
* ジェムアイテムの判定を作ってくれる
*/
class GemItem : public Component
{
public:
	GemItem() = default;
	virtual ~GemItem() = default;
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	// 衝突時の処理
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other) override;

	// ジェムアイテムの状態を表す列挙型
	enum class GemItemState
	{
		alive, // 生きている
		get,   // ゲットされた
		dead,  // 死んでいる

	};
	GemItemState GetState() const { return gemItemState; }

private:
	GemItemState gemItemState = GemItemState::alive; // プレイヤーの状態

	float timer = 0; // 動作制御用タイマー	
};

#endif // GEMITEM_H_INCLUDED