/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED
#include "glad/glad.h"
#include "GameObject.h"
#include "Scene.h"
#include "Mesh.h"
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>

class FramebufferObject;
using FramebufferObjectPtr = std::shared_ptr<FramebufferObject>;

// シェーダのロケーション番号
constexpr GLint locTransformMatrix = 0;
constexpr GLint locNormalMatrix = 1;
constexpr GLint locViewProjectionMatrix = 2;
constexpr GLint locShadowTextureMatrix = 10;
constexpr GLint locShadowNormalOffset = 11;

// シェーダで使えるライトの数
constexpr size_t maxShaderLightCount = 16;

// ユニフォーム変数のロケーション番号
constexpr GLint locCloudColor = 100;

// シェーダのライト配列のロケーション番号
constexpr GLint locAlphaCutoff = 102;
//環境光
constexpr GLint locAmbientLight = 107;
//平行光源用のロケーション番号
constexpr GLint locDirectionalLightColor = 108;
constexpr GLint locDirectionalLightDirection = 109;

constexpr GLint locLightCount = 110;
constexpr GLint locLightColorAndFalloffAngle = 111;

constexpr GLint locLightPositionAndRadius =
locLightColorAndFalloffAngle + maxShaderLightCount;

constexpr GLint locLightDirectionAndConeAngle =
locLightColorAndFalloffAngle + maxShaderLightCount * 2;

// 点光源
struct LightData
{
	vec3 color;      // 色
	float intensity; // 明るさ
	vec3 position;   // 位置
	float radius;    // ライトが届く最大半径

	
	  // スポットライト用の変数
	vec3 direction;     // ライトの向き
	float coneAngle;    // スポットライトが照らす角度
	float falloffAngle; // スポットライトの減衰開始角度

	bool used = false; // 使用中かどうか
};

// 平行光源
struct DirectionalLight
{
	vec3 color = { 1.00f, 0.98f, 0.95f }; // 色
	float intensity = 1.0f;   // 明るさ
	vec3 direction = { 0.58f, -0.58f, 0.58f }; // 向き
};

// メッシュ番号
// Initializeメンバ関数にあるmeshes配列と順番を合わせること
// meshes配列に変更を加えた場合、Meshidの順序も同様に変更すること。
/**
* ゲームエンジン
*/
class Engine
{
public:
	Engine() = default;
	~Engine() = default;

	int Run();

	/**
	* ゲームオブジェクトを作成する
	*
	* @param name     オブジェクトの名前
	* @param tag      オブジェクトのtag名
	* @param position オブジェクトを配置する座標
	* @param rotation オブジェクトの回転角度
	* @param rotation オブジェクトの色
	*/
	template<typename T>
	std::shared_ptr<T> Create(const std::string& name,
		const std::string& tag,
		const vec3& position = { 0, 0, 0 },
		const vec3& rotation = { 0, 0, 0 })
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->tag = tag;
		p->position = position;
		p->rotation = rotation;
		gameObjects.push_back(p); // エンジンに登録
		return p;
	}


	/**
	* UIオブジェクトを作成する
	*
	* @tparam T ゲームオブジェクトに割り当てるUILayoutまたはその派生クラス
	*
	* @param filename UIオブジェクトに表示する画像
	* @param position UIオブジェクトの座標
	* @param scale    UIオブジェクトの大きさ
	*
	* @return 作成したUIオブジェクト
	*/
	template<typename T>
	std::pair<GameObjectPtr, std::shared_ptr<T>> CreateUIObject(
		const char* filename, const char* tagname, const vec2& position, float scale)
	{
		auto object = Create<GameObject>(filename,tagname, { position.x, position.y, 0 });
		object->renderQueue = RenderQueue_overlay;
		object->staticMesh = GetStaticMesh("plane_xy");
		
		// 固有マテリアルを作成し、テクスチャを差し替える
		auto texBaseColor = GetTexture(filename);
		object->materials = CloneMaterialList(object->staticMesh);
		object->materials[0]->texBaseColor = texBaseColor;

		// 画像サイズに応じて拡大率を調整
		const float aspectRatio = texBaseColor->GetAspectRatio();
		object->scale = { scale * aspectRatio, scale, 1 };

		// コンポーネントを追加
		auto component = object->AddComponent<T>();

		return { object, component };
	}

	
	  /**
	  * ゲームオブジェクト配置ファイルに含まれるメッシュ名と実際のファイル名の対応表
	  *
	  * first=メッシュ名, second=実際のファイル名
	  */
	using FilepathMap = std::unordered_map<std::string, std::string>;

	
	  /**
	  * ゲームオブジェクト配置ファイルを読み込む
	  *
	  * @param filename ゲームオブジェクト配置ファイル名
	  *
	  * @return ファイルから読み込んだゲームオブジェクトの配列
	  */
	GameObjectList LoadGameObjectMap(const char* filename, const FilepathMap&);

	// すべてのゲームオブジェクトを削除する
	void ClearGameObjectAll();

	// 視野角の管理
	void SetFovY(float fovY);
	float GetFovY() const { return degFovY; }
	float GetFovScale() const { return fovScaleY; }

	// 次のシーンを設定する
	template<typename T>
	void SetNextScene() { nextScene = std::make_shared<T>(); }

	// カメラを取得する
	GameObject& GetMainCamera() { return camera; }
	const GameObject& GetMainCamera() const { return camera; }

	// キーが押されていたらtrue, 押されてなければfalseを返す
	bool GetKey(int key) const
	{
		return glfwGetKey(window, key) == GLFW_PRESS;

	}

	// キーが押されていたらtrue, 押されてなければfalseを返す
	// マウスカーソルの座標を取得する
	vec2 GetMousePosition() const;

	// マウスのボタンが押されていたらtrue, 押されていなければfalseを返す
	bool GetMouseButton(int button) const;
	
	/**
	* マウスボタンのクリック状態を取得する
	*
	* @param button 調べるボタンを示すマクロ定数(GLFW_MOUSE_BUTTON_LEFT等)
	*
	* @retval true  押されている
	* @retval false 押されていない
	*/
	bool GetMouseClick(int button) const;

	double& GetPreviousTime() { return previousTime; }
	// フレームバッファの大きさを取得する
	vec2 GetFramebufferSize() const;
	// フレームバッファのアスペクト比を取得する
	float GetAspectRatio() const;

	/**
	* スタティックメッシュの取得
	*
	* @param name メッシュ名
	*
	* @return 名前がnameと一致するスタティックメッシュ
	*/
	StaticMeshPtr GetStaticMesh(const char* name)
	{
		return meshBuffer->GetStaticMesh(name);
	}
	
	//スケルタルメッシュとスタティックメッシュの描画の違いは
	//シェーダーストレージブロックの有無で、描画パラメータを描画する部分に違いはない
	/**
	* スケルタルメッシュの取得
	*
	* @param name メッシュ名
	*
	* @return 名前がnameと一致するスケルタルメッシュ
	*/
	SkeletalMeshPtr GetSkeletalMesh(const char* name)
	{
		return meshBuffer->GetSkeletalMesh(name);
	}
	
	/**
	* テクスチャの取得
	*
	* @param name テクスチャファイル名
	*
	* @return 名前がnameと一致するテクスチャ
	*/
	TexturePtr GetTexture(const char* name);

	/**
	* ライト配列を初期化する
	*/
	void InitializeLight();
	
	/**
	* 新しいライトを取得する
	*
	* @return ライトのインデックス
	*/
	int AllocateLight();
	
	/**
	* ライトを解放する
	*
	* @param index 解放するライトのインデックス
	*/
	void DeallocateLight(int index);
	
	/**
	* インデックスに対応するライトデータを取得する
	*
	* @param index ライトのインデックス
	*
	* @retval nullptr以外 indexに対応するライトデータのアドレス
	* @retval nullptr     対応するライトデータが存在しない
	*/
	LightData* GetLight(int index);
	const LightData* GetLight(int index) const;

	/**
	* マウス座標から発射される光線を取得する
	*
	* @return マウス座標から発射される光線
	*/
	Ray GetRayFromMousePosition() const;

	// 光線の交差判定結果
	struct RaycastHit
	{
		ColliderPtr collider;     // 最初に光線と交差したコライダー
		vec3 point;               // 最初の交点の座標
		float distance;           // 最初の交点までの距離
	};

	//光線との交差判定を行うコライダーかどうかを判定する「述語」
	//「述語」は「ある引数が条件を満たすかどうかをboolで返す関数」のこと
	//基本的には別の関数に引数として渡されます。
	//述語が単体の関数として使われることはない
	//std::find_ifやstd::sortに渡すラムダ式等に
	/**
	* 交差判定の対象になるかどうかを調べる述語型
	*
	* @param collider 判定するコライダー
	* @param distance 光線とコライダーの交点までの距離
	*
	* @retval true  交差判定の対象になる
	* @retval false 交差判定の対象にならない
	*/
	using RaycastPredicate =
		std::function<bool(const ColliderPtr& collider, float distance)>;


	/**
	* 光線とコライダーの交差判定
	*
	* @param ray     光線
	* @param hitInfo 光線と最初に交差したコライダーの情報
	* @param pred    交差判定を行うコライダーを選別する述語
	*
	* @retval true  コライダーと交差した
	* @retval false どのコライダーとも交差しなかった
	*/
	bool Raycast(const Ray& ray, RaycastHit& hitInfo,
		const RaycastPredicate& pred) const;



private:
	// コンストラクタ、デストラクタを呼べるようにするための補助クラス
	//空のテクスチャが作成するため
	struct TexHelper : public Texture {
		TexHelper(const char* p) : Texture(p) {}
		TexHelper(const char* p, int w, int h, GLenum f) : Texture(p, w, h, f) {}

	};
	// ワールド座標系のコライダーを表す構造体
	struct WorldCollider
	{
		//コライダー同士が重ならないようにする
		// 座標を変更する
		void AddPosition(const vec3& v) {
			origin->GetOwner()->position += v;
			world->AddPosition(v);
		}

		ColliderPtr origin;
		ColliderPtr world;
	};
	using WorldColliderList = std::vector<WorldCollider>;
	void ApplyPenetration(WorldColliderList*, GameObject*, const vec3&);

	int Initialize();
	void Update();
	void Render();
	void DrawGameObject(GLuint prog,
		GameObjectList::iterator begin,
		GameObjectList::iterator end);

	void UpdateGameObject(float deltaTime);
	void HandleGameObjectCollision();
	void HandleWorldColliderCollision(
		WorldColliderList* a, WorldColliderList* b);
	void RemoveDestroyedGameObject();
	void UpdateShaderLight();
	void DrawSkySphere();
	void CreateShadowMap(
		GameObjectList::iterator begin,
		GameObjectList::iterator end);


	GLFWwindow* window = nullptr;				//ウィンドウオブジェクト
	const std::string title = "OpenGLGame";		//ウィンドウタイトル
	float degFovY = 60; // 垂直視野角(度数法)
	float radFovY = degFovY * 3.1415926535f / 180;
	float fovScaleY = 1 / tan(radFovY / 2); // 視野角による拡大率の逆数

	
	// アンリット・シェーダ
	GLuint vsUnlit = 0;
	GLuint fsUnlit = 0;
	GLuint progUnlit = 0;

	// スケルタルメッシュ・シェーダ
	GLuint vsSkeletal = 0;
	GLuint fsSkeletal = 0;
	GLuint progSkeletal = 0;

	  // シャドウ・シェーダ
	GLuint vsShadow = 0;
	GLuint fsShadow = 0;
	GLuint progShadow = 0;
	FramebufferObjectPtr fboShadow; // デプスシャドウ用FBO

	GLuint vs = 0;			// 頂点シェーダの管理番号
	GLuint fs = 0;			// フラグメントシェーダの管理番号
	GLuint prog3D = 0;		// シェーダプログラムの管理番号
	MeshBufferPtr meshBuffer;   // 図形データ管理オブジェクト
	
	// 関節行列の配列の最大バイト数
	static constexpr size_t jointMatricesBytes = 1024 * 1024;
	BufferObjectPtr ssboJointMatrices; // 関節行列の配列用のバッファ


	std::unordered_map<std::string, TexturePtr> textureCache;

	GameObjectList gameObjects; // ゲームオブジェクト配列
	double previousTime = 0; // 前回更新時の時刻
	float deltaTime = 0;     // 前回更新からの経過時間

	ScenePtr scene;     // 実行中のシーン
	ScenePtr nextScene; // 次のシーン

	//ボタンが押されてから、一定時間以内にボタンが離された場合は「クリック」として判定する
	// クリックと判定する速度
	static constexpr float mouseClickSpeed = 0.3f;

	// マウスボタンの状態
	struct MouseButton {
		bool current = false;  // 現在フレームのボタンの状態
		bool previous = false; // 前フレームのボタンの状態
		bool click = false;    // クリックの状態
		float timer = 0;       // ボタンが押されている時間

	};
	MouseButton mouseButtons[3];

	//locはlocationの略として使われている
	std::vector<LightData> lights; // ライトデータの配列
	std::vector<int> usedLights;    // 使用中のライトのインデックス配列
	std::vector<int> freeLights;  // 未使用のライトのインデックス配列
	
	// 一度に増やすライト数
	static constexpr size_t lightResizeCount = 100;
	
	// 平行光源
	DirectionalLight directionalLight;

	// スカイスフィア用モデル
	StaticMeshPtr skySphere;
	StaticMeshPtr cloud;

	//カメラオブジェクト
	GameObject camera;
	GLsizei indexCount = 0;
public:
	// 環境光
	 vec3 ambientLight = vec3(0.50f, 0.50f, 0.50f);
	//音量の調整
	float MusicVolume = 0.4f;

private:
	int move = 0;
	int range = 128;

};

#endif // ENGINE_H_INCLUDED