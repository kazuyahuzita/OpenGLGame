/**
* @file Particle.h
*/
#ifndef PARTICLE_H_INCLUDED
 #define PARTICLE_H_INCLUDED
 #include "glad/glad.h"
 #include "Engine/Texture.h"
 #include "Engine/Mesh.h"
 #include "Engine/VecMath.h"
 #include <vector>
 #include <memory>
 #include <random>

// 先行宣言
class Particle;
class ParticleEmitter;
using ParticleEmitterPtr = std::shared_ptr<ParticleEmitter>;
class ParticleManager;
using ParticleManagerPtr = std::shared_ptr<ParticleManager>;
class VertexArrayObject;
using VertexArrayObjectPtr = std::shared_ptr<VertexArrayObject>;
/**
* 値の補間方法
*/
enum class InterporationType
 {	
	linear,    // 線形補間		
	easeIn,    // 徐々に加速		
	easeOut,   // 徐々に減速		
	easeInOut, // 徐々に加速し、徐々に減速する		
	none,      // 補間しない		
};

//
/**
* 値を補間する機能を持つクラス
*/
template<typename T>
class Interporator
{
public:
	Interporator() = default;
	~Interporator() = default;
	explicit Interporator(const T& start) : start(start), end(start) {}
	Interporator(const T& start, const T& end) : start(start), end(end) {}

	// 値を設定する
	void Set(const T& start, const T& end) {
		this->start = start;
		this->end = end;

	}

	// 引数t(範囲0.0～1.0)で補間した値を返す
	T Interporate(float t) const {
		switch (type) {
		default:
		case InterporationType::linear: return start + t * (end - start);
		case InterporationType::none:   return start;

		}

	}

	T start; // 補間開始時の値
	T end;   // 補間終了時の値
	InterporationType type = InterporationType::linear; // 補間方法
};
using Interporator1f = Interporator<float>;
using Interporator2f = Interporator<vec2>;
using Interporator3f = Interporator<vec3>;
using Interporator4f = Interporator<vec4>;

/**
* パーティクルの動作を定義するパラメータ
*/
struct ParticleParameter
 {
	float lifetime = 1;           // 生存期間
	vec4 tc = { 0, 0, 1, 1 }; // テクスチャ座標
	Interporator1f radial = Interporator1f(10);             // 半径方向の速度
	Interporator3f velocity = Interporator3f({ 0, 0, 0 });    // 速度
	Interporator2f scale = Interporator2f({ 1, 1 });       // 大きさ
	Interporator1f rotation = Interporator1f(0);              // 回転
	Interporator4f color = Interporator4f({ 1, 1, 1, 1 }); // 色と不透明度
};


/**
* エミッターの形状
*/
enum class ParticleEmitterShape
{
	sphere, // 球
	box,    // 直方体
};


/**
* パーティクル・エミッターのパラメーター
*/
struct ParticleEmitterParameter
{
	std::string name;      // エミッター名
	float gravity = -9.8f; // 重力

	// 画像とブレンド係数
	std::string imagePath;          // テクスチャ・ファイル名
	vec2 tiles = { 1, 1 }; // テクスチャの縦と横の分割数
	GLenum srcFactor = GL_SRC_ALPHA;           // ブレンド係数
	GLenum dstFactor = GL_ONE_MINUS_SRC_ALPHA; // ブレンド係数

	// 放出時間と放出頻度
	float duration = 1.0f; // パーティクルを放出する秒数
	bool loop = false; // duration経過後の動作(true=ループ, false=停止)
	float emissionsPerSecond = 10; // 秒あたりのパーティクル放出数

	// 放出範囲の形状
	//形状を無名共用体として定義
	//共用体のメンバが有効なのかはshapeメンバ変数で指定
	vec3 position = { 0, 0, 0 }; // 座標
	vec3 rotation = { 0, 0, 0 }; // 回転
	ParticleEmitterShape shape = ParticleEmitterShape::sphere; // 形状
	float randomizeDirection = 0; // 放出方向の乱数性(0=なし 1=完全ランダム)
	union {
		struct Sphere {
			float radius = 1;    // 球の半径
			float thickness = 1; // 球殻の厚み(0=最薄 1=中心までぎっしり)

		} sphere;
		struct Box {
			vec3 size = { 1, 1, 1 }; // 直方体のサイズ

		} box;

	};


	// コンストラクタ・デストラクタ
	ParticleEmitterParameter() : sphere({ 1, 1 }) {}
	~ParticleEmitterParameter() = default;

	// 放出範囲に球を指定する
	void SetSphereShape(float radius, float thickness) {
		shape = ParticleEmitterShape::sphere;
		sphere.radius = radius;
		sphere.thickness = thickness;

	}

	// 放出範囲に直方体を指定する
	void SetBoxShape(const vec3& size) {
		shape = ParticleEmitterShape::box;
		box.size = size;

	}
};


/**
* パーティクル
*/
class Particle
 {
public:
	friend ParticleManager;
	friend ParticleEmitter;
	
	Particle() = default;
	~Particle() = default;
	
		  // 生存期間の比率
		float GetLifetimeRatio() const {
		const float t = 1 - lifetime / pp->lifetime;
		return std::clamp(t, 0.0f, 1.0f);
		
	}
	
		  // true=死亡, false=生存
		bool IsDead() const { return lifetime <= 0; }
	
private:
	  // 変化しないパラメータ
	const ParticleParameter * pp;          // 基本パラメータ
	vec3 radialDirection = { 1, 0, 0 }; // 中心からの向き
	Interporator3f velocity = Interporator3f({ 0, 0, 0 }); // 速度
	vec4 tc = { 0, 0, 1, 1 };         // テクスチャ座標
	
	  // 変化するパラメータ
	float lifetime = 0;                   // 生存期間
	vec3 position = { 0, 0, 0 }; // 座標
	float gravityAccel = 0;               // 重力による加速度の合計
	float gravitySpeed = 0;               // 重力による速度の合計
	//半透明描画をする時は、「奥から手前」の順で描画するため
	//viewDepthをつかってソートします
	float viewDepth = 0;                  // 視点からの深度値
};


/**
* パーティクル放出クラス
*/
class ParticleEmitter
{
public:
	friend ParticleManager;

	ParticleEmitter(const ParticleEmitterParameter& ep,
		const ParticleParameter& pp, ParticleManager* pManager);
	~ParticleEmitter() = default;
	ParticleEmitter(const ParticleEmitter&) = delete;
	ParticleEmitter& operator=(const ParticleEmitter&) = delete;

	void Update(const mat4& matView, float deltatime);
	void Die() { ep.loop = false; ep.duration = 0; }
	bool IsDead() const { return !ep.loop && timer >= ep.duration && particles.empty(); }

	// パラメータの取得・設定
	ParticleEmitterParameter& GetEmitterParameter() { return ep; }
	const ParticleEmitterParameter& GetEmitterParameter() const { return ep; }
	ParticleParameter& GetParticleParameter() { return pp; }
	const ParticleParameter& GetParticleParameter() const { return pp; }

private:
	void AddParticle();

	ParticleEmitterParameter ep; // エミッターのパラメーター
	ParticleParameter pp;        // パーティクルのパラメーター
	ParticleManager* pManager = nullptr; // 管理クラスのアドレス

	TexturePtr texture;      // パーティクル用テクスチャ
	float interval = 0;      // パーティクルの発生間隔(秒)
	float timer = 0;         // 総経過時間(秒)
	float emissionTimer = 0; // パーティクル発生タイマー(秒)
	float viewDepth = 0;     // 視点からの深度値

	std::vector<Particle*> particles; // パーティクルリスト
	GLsizei dataCount = 0; // 描画するパーティクル数
	size_t dataOffset = 0; // 描画の基準となる頂点のオフセット
};


/**
* パーティクル管理クラス
*/
class ParticleManager
 {
public:
	static ParticleManagerPtr Create(size_t maxParticleCount);
	
	explicit ParticleManager(size_t maxParticleCount);
	~ParticleManager();
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager & operator=(const ParticleManager&) = delete;
	
	void Update(const mat4 & matView, float deltatime);
	void UpdateSSBO();
	void Draw(const mat4 & matProj, const mat4 & matView);
	
	  // エミッターの管理
	ParticleEmitterPtr AddEmitter(
		const ParticleEmitterParameter & ep, const ParticleParameter & pp);
	ParticleEmitterPtr FindEmitter(const std::string & name) const;
	void RemoveEmitter(const ParticleEmitterPtr&);
	void RemoveEmitterAll();
	
	  // パーティクルの管理
	Particle * AllocateParticle();
	void DeallocateParticle(Particle * p);
	
		  // 乱数の生成
		int RandomInt(int min, int max) {
		return std::uniform_int_distribution<>(min, max)(randomEngine);
		
	}
	 float RandomFloat(float min, float max) {
		return std::uniform_real_distribution<float>(min, max)(randomEngine);
		
	}
	
private:
	std::vector<ParticleEmitterPtr> emitters; // エミッターのリスト
	//パーティクルは一度に大量に、発生するのでスマートポインタ
	//を使うのは避けたい
	//今回は、パーティクルシステムの内部で管理が行われるので
	//スマートポインタなしでも、比較的簡単に処理でき、かつ高速化につながる
	std::vector<Particle> particles; // パーティクルのリスト
	std::vector<Particle*> freeParticles; // 未使用パーティクルのリスト
	
	size_t maxParticleCount = 0; // 生成可能な最大パーティクル数
	BufferObjectPtr vbo;
	BufferObjectPtr ibo;
	VertexArrayObjectPtr vao;
	GLuint vsParticle = 0;			// 頂点シェーダの管理番号
	GLuint fsParticle = 0;			// フラグメントシェーダの管理番号
	GLuint prog3D = 0;		// シェーダプログラムの管理番号
	BufferObjectPtr ssbo;
	MeshBufferPtr meshBuffer;   // 図形データ管理オブジェクト
	std::mt19937 randomEngine;
};

#endif // PARTICLE_H_INCLUDED