#ifndef ITEMBLOCK_H_INCLUDED
#define ITEMBLOCK_H_INCLUDED
#include "Engine/Engine.h"
#include "Engine/Component.h"
#include "Engine/SphereCollider.h"
/**
* AthleticSceneにて追跡するエネミーを出してくれるアイテムとして配置してある
*/
class ItemBlock : public Component
{
public:	ItemBlock() = default;
	  virtual ~ItemBlock() = default;
	  virtual void Awake()override;
	  virtual void Update(float deltaTime) override;
	  // 衝突時の処理
	  virtual void OnCollision(
		  const ComponentPtr& self, const ComponentPtr& other) override;

	  // プレイヤーの状態を表す列挙型
	  enum class ItemBlockState
	  {
		  Onswitch,		// 押されていない
		  Offswitch,	// 押されている
		  stopswitch	//　何かあった時
	  };
	  ItemBlockState GetState() const { return IBstate; }

	  void SetState(ItemBlockState _state) {
		  IBstate = _state;
	  }

	float Timer = 0.0f;
	bool isActive = false;
private:

	ItemBlockState IBstate = ItemBlockState::Onswitch; // 状態
	float speed = 0.05f;
};

#endif // ITEMBLOCK_H_INCLUDED