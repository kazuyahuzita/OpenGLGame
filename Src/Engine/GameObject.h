/**
* @file GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include"Component.h"
#include "Texture.h"
#include"VecMath.h"
#include "AabbCollider.h"
#include<string>
#include<vector>
#include<memory>
#include <type_traits>

// 先行宣言
class Engine;
class Controller;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>; // ゲームオブジェクトのポインタ
using GameObjectList = std::vector<GameObjectPtr>; // ゲームオブジェクトポインタの配列
class GameObjectSettingsJson;
struct StaticMesh;
using StaticMeshPtr = std::shared_ptr<StaticMesh>; // スタティックメッシュのポインタ
struct SkeletalMesh;
using SkeletalMeshPtr = std::shared_ptr<SkeletalMesh>; // スケルタルメッシュのポインタ
struct Material;
using MaterialPtr = std::shared_ptr<Material>; // マテリアルのポインタ
using MaterialList = std::vector<MaterialPtr>; // マテリアル配列

/**
* 描画の順序
*/
enum RenderQueue
{
	RenderQueue_geometry = 2000, // 一般的な図形
	/*
	*描画順の失敗で、完全な透明の物でもカラーバッファと深度バッファに書き込まれてしまうため、
	* 先に完全に不透明なモデルを描画し、その後で半透明な部分を持つモデルを描画する。
	*/
	RenderQueue_transparent = 3000, // 半透明な図形
	RenderQueue_overlay = 4000, // UI, 全画面エフェクトなど
	RenderQueue_max = 5000, // キューの最大値
};

/**
* ゲームに登場するさまざまなオブジェクトを表す基本クラス
*/
class GameObject
{
	friend Engine;
	friend GameObjectSettingsJson;
public:
	GameObject() = default;
	virtual ~GameObject();

	//デストラクタ側で、親子階層の解除を行っているため、
	// ポインタの所有権を他の人に持たれるとまずいので,
	  // コピーと代入を禁止
	GameObject(GameObject&) = delete;
	GameObject& operator=(GameObject&) = delete;

	// ゲームエンジンを取得
	Engine* GetEngine() const { return engine; }
	// コントローラーヘッダーを取得
	Controller* GetController() const { return controller; }

	// ゲームオブジェクトをエンジンから削除する
	void Destroy() { isDestroyed = true; }
	// ゲームオブジェクトが破壊されていたらtrueを返す
	bool IsDestroyed() const { return isDestroyed; }

	// ゲームオブジェクトにコンポーネントを追加する
	template<typename T>
	std::shared_ptr<T> AddComponent()
	{
		auto p = std::make_shared<T>();
		p->owner = this;
		if constexpr (std::is_base_of_v<Collider, T>) {
			colliders.push_back(p);
		}
		components.push_back(p);
		p->Awake();
		return p;
	}


	/**
	* コンポーネントを検索する
	*
	* 多くのコンポーネントを持つゲームオブジェクトに対して使用すると、
	*  検索に時間がかかる場合があるので注意。
	*
	* @tparam T 検索するコンポーネントの型
	*
	* @retval nullptr以外 T型の最初のコンポーネントへのポインタ
	* @retval nullptr    T型のコンポーネントを持っていない
	*/
	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		for (auto& e : components) {
			// shared_ptrの場合はdynamic_castではなくdynamic_pointer_castを使う
			//shared_ptrは通常のポインタではなく、クラスなのでdynamic_castでは呼べない
			auto p = std::dynamic_pointer_cast<T>(e);
			if (p) {
				return p; // 見つかったコンポーネントを返す

			}

		}
		//ポインタが見つからなかったら、空のポインタを返す
		return nullptr; // 見つからなかったのでnullptrを返す
	}
	//動的キャストは時間がかかるため避けるべきだが、オブジェクトのポインタから、
	//そのオブジェクトが特定のクラスを継承しているかどうかを,知りたいときには仕方がない


	// ゲームオブジェクトからコンポーネントを削除する
	void RemoveDestroyedComponent();

	// イベント制御
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void OnCollision(
		const ComponentPtr& self, const ComponentPtr& other);
	virtual void OnDestroy();

	/*
	* 親オブジェクトを取得する
	*
	* @retval nullptr     親がいない
	* @retval nullptr以外 親オブジェクトのアドレス
	*/
	GameObject* GetParent() const { return parent; }

	/**
	* 親オブジェクトを設定する
	*
	* @param parent 親にするゲームオブジェクト
	*               nullptrを指定すると親子関係を解消する
	*/
	void SetParent(GameObject* parent);
	void SetParent(const GameObjectPtr& parent);

	/**
	* 子オブジェクトの数を取得する
	*
	* @return 子オブジェクトの数
	*/
	size_t GetChildCount() const { return children.size(); }

	/**
	* 子オブジェクトを取得する
	*
	* @param index 子オブジェクト配列の添字
	*              GetChildCountが返す以上の値を渡すとエラーになるので注意
	*
	* @return index番目の子オブジェクト
	*/
	GameObject* GetChild(size_t index) const { return children[index]; }
	
	// 座標変換行列を取得する
	const mat4 & GetTransformMatrix() const { return transformMatrix; }
	
	  // 法線変換行列を取得する
	const mat3 & GetNormalMatrix() const { return normalMatrix; }

	std::string name;                // オブジェクト名
	std::string tag;				 // オブジェクト名
	vec3 position = { 0, 0, 0 };     // 物体の位置
	vec3 rotation = { 0, 0, 0 };     // 物体の回転角度
	vec3 scale = { 1, 1, 1 };		 // 物体の拡大率
	vec4 color = { 1, 1, 1, 1 };	 // 物体の色
	StaticMeshPtr staticMesh;        // 表示するスタティックメッシュ
	SkeletalMeshPtr skeletalMesh;    // 表示するスケルタルメッシュ
	MaterialList materials;          // ゲームオブジェクト固有のマテリアル配列
	int renderQueue = RenderQueue_geometry; // 描画順
	bool isGrounded = false;  // 地面についているかどうか
	bool isSlided = false;  // 滑る床にいるかどうか

private:
	Engine* engine = nullptr;				// エンジンのアドレス
	Controller* controller = nullptr;        // コントローラーヘッダーのアドレス
	bool isDestroyed = false;        // 死亡フラグ
	//行列を管理するメンバ変数ではあるが、
	//計算用とかではなく、一度計算した値を、覚えてもらうために使う
	mat4 transformMatrix = mat4(1);  // 座標変換行列
	mat3 normalMatrix = mat3(1);     // 法線変換行列

	GameObject* parent = nullptr;      // 親オブジェクト
	std::vector<GameObject*> children; // 子オブジェクト

	std::vector<ComponentPtr> components;	// コンポーネント配列
	std::vector<ColliderPtr> colliders;   // コライダー配列
};

#endif // GAMEOBJECT_H_INCLUDED