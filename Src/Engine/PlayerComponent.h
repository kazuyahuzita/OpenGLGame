#ifndef PLAYERCOMPONENT_H_INCLUDED
#define PLAYERCOMPONENT_H_INCLUDED
#include "../AudioSettings.h"
#include "CharacterMovement.h"
#include "Component.h"
#include "EasyAudio/EasyAudio.h"
#include "Engine.h"
#include "Light.h"
#include "SlidingMovement.h"
#include "SphereCollider.h"
#include "UILayout.h"
/**
* プレイヤー
*/
class PlayerComponent : public Component
{
public:	PlayerComponent() = default;
	  virtual ~PlayerComponent() = default;
	  virtual void Awake()override;
	  virtual void Update(float deltaTime) override;
	  // 衝突時の処理
	  virtual void OnCollision(
		  const ComponentPtr& self, const ComponentPtr& other) override;
	  //ジャンプ台に触れた際に吹っ飛ばす処理
	  void Jumping(float deltaTime);
	  //敵に触れた際に吹っ飛ばす処理
	  void Knockback(float deltaTime);


	  // プレイヤーの状態を表す列挙型
	  enum class PlayerState
	  {
		  alive,		// 生きている
		  drive,		// 走っている
		  athletic,		// 走っている
		  jumpinghit,	// 当たった
		  nolmalhit,	// そこらの敵に当たった時
		  dead,			// 死んでいる
		  goal,			// ゴールした
		  stop			// プレイヤーの動きを止めたいとき
	  };
	  PlayerState GetState() const { return state; }

	  void SetState(PlayerState _state) {
		  state = _state;
	  }
	  //プレイヤーのポジション
	  vec3 GetPosition() const { return GetOwner()->position; }
	  //プレイヤーのポジション
	  void SetPosition(vec3 _position)  { 
		   GetOwner()->position = _position;
	  }

	  //エネミーに触れたかどうか
	  void SetEnemy(bool _enemy) {
		  isEnemy = _enemy;
	  }
	  bool GetEnemy()const { return isEnemy; }

	  //エネミーに触れたかどうか
	  void SetJump(bool _jump) {
		  isJump = _jump;
	  }
	  bool GetJump()const { return isJump; }

	  //ポイント管理
	  int GetPoint() const { return ItemPoint; }
	  //キーアイテムを取ったときとかの処理
	  //ホラーゲームシーンで使われているフラグ管理のGetter
	  bool GetKeyItem()const { return isKeyItem; }
	  bool GetTyreItem()const { return isTyre; }
	  bool GetBatteryItem()const { return isBattery; }
	  bool GetSeatItem()const { return isSeat; }

	  //ゲーム終了処理
	  void SetESC(bool esc) {
		  isESC = esc;
	  }
	  bool GetESC()const { return isESC; }


private:
	GLFWgamepadstate gamepadstate;			//ゲームパッドの情報を持ってくる
	PlayerState state = PlayerState::alive; // プレイヤーの状態

	GameObjectPtr DamageEffectObject;//ダッシュ時のエフェクト

	// カメラの移動
	//十字キーとWSADキーで分けるために二つある
	const float cameraSpeed = 2.0f;
	const float cameraMoveSpeed = 3.0f; 
	float addDashPower = 1.0f;			//走る際に変更する走る力
	float JumpPower = 4.0f;				//ジャンプの威力
	float JumpTimer = 0.0f;				//ジャンプの際に音が重ならないようにする為にフラグを制御するタイマー

	int ItemPoint = 0;					//ジェムをゲットした際のポイント加算に使う

	bool isBattery = false;				//HorrorGameSceneでゲットしたアイテムのフラグ
	bool isEnemy = false;				//エネミーに当たった際のフラグ
	bool isESC = false;					//ゲーム終了判定のオブジェクト
	bool isFire = false;				//ファイアオブジェクト(障害物)に触れた際のフラグ
	bool isGoal = false;				//ゴールした際のフラグ
	bool isKeyItem = false;				//HorrorGameSceneでゲットしたアイテムのフラグ
	bool isJump = false;				//ジャンプの際に音が重ならないようにするフラグ
	bool isPlayerSlide = false;			//スライドブロックに触れた際の判定
	bool isSeat = false;				//HorrorGameSceneでゲットしたアイテムのフラグ
	bool isTyre = false;				//HorrorGameSceneでゲットしたアイテムのフラグ

};

#endif // PLAYERCOMPONENT_H_INCLUDED