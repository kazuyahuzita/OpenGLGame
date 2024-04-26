/**
* @file Component.h
*/
#ifndef COMOPNENT_H_INCLUDED
 #define COMOPNENT_H_INCLUDED
 #include <memory>

// 先行宣言
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;
class Component;
using ComponentPtr = std::shared_ptr<Component>; // コンポーネントのポインタ

/**
* コンポーネントの基底クラス
*/
class Component
 {
	friend GameObject;
public:
	Component() = default;
	virtual ~Component() = default;
	
	  // コンポーネントの所有者を取得
	GameObject * GetOwner() const { return owner; }
	
	  // コンポーネントをゲームオブジェクトから削除する
	void Destroy() { isDestroyed = true; }
	
	  // コンポーネントが破壊されていたらtrueを返す
	bool IsDestroyed() const { return isDestroyed; }
	
	  // ゲームオブジェクトに追加された時に呼び出される
	virtual void Awake() {}
	
	  // 最初のUpdateの直前で呼び出される
	virtual void Start() {}
	
	  // 毎フレーム1回呼び出される
	virtual void Update(float deltaTime) {}
	
	//敵がプレイヤーを追跡する
	virtual void Tracking(float positionX, float positionZ, float deltaTime) {}

	  // 衝突が起きたときに呼び出される
	virtual void OnCollision(
	const ComponentPtr & self, const ComponentPtr & other) {}
	
	  // ゲームオブジェクトがエンジンから削除されるときに呼び出される
	virtual void OnDestroy() {}
	
private:
	GameObject * owner = nullptr; // このコンポーネントの所有者
	bool isStarted = false;      // Startが実行されたらtrueになる
	bool isDestroyed = false;    // Destroyが実行されたらtrueになる
	bool isTouched = false;    // Destroyが実行されたらtrueになる
};

#endif // COMOPNENT_H_INCLUDED