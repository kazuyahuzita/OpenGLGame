#ifndef RIDER_H_INCLUDED
#define RIDER_H_INCLUDED
#include "../FluorescentLight.h"
#include "CharacterMovement.h"
#include "Component.h"
#include "Engine.h"
#include "Light.h"
#include "SphereCollider.h"
#include "UILayout.h"
/**
* RiderSceneの時のプレイヤー
*/
class Rider : public Component
{
public:	Rider() = default;
	  virtual ~Rider() = default;
	  virtual void Awake()override;
	  virtual void Update(float deltaTime) override;
	  // 衝突時の処理
	  virtual void OnCollision(
		  const ComponentPtr& self, const ComponentPtr& other) override;

	  // プレイヤーの状態を表す列挙型
	  enum class RiderState
	  {
		  drive, // 生きている
		  goal,  // ゴールした
		  dead,  // 死んでいる
		  stop,	//止めたいときに
	  };
	  RiderState GetState() const { return riderState; }

	  void SetState(RiderState _state) {
		  riderState = _state;
	  }

	  bool GetESC()const { return isESC; }
	  void SetESC(bool _esc) {
		  isESC = _esc;
	  }

private:
	GLFWgamepadstate gamepadstate;	//ゲームパッドの情報を持ってくる
	GameObjectPtr DashEffectObject;	//ダッシュ時のエフェクト1
	GameObjectPtr DashEffectObject1;//ダッシュ時のエフェクト2

	RiderState riderState = RiderState::drive; // Riderシーンのプレイヤーの状態
	float timer = 0; // 動作制御用タイマー	
	// カメラの移動
	//十字キーとWSADキーで分けるために二つある
	const float cameraSpeed = 2.0f;
	const float addcameraMoveSpeed = 2.0f;

	const float addJumpPower = 2.0f;//ジャンプの威力
	float bikerotate = 0.3f;		//左右に移動するときの力

	bool isESC = false;		//escキーを押したときに反応するフラグ

};

#endif // RIDER_H_INCLUDED