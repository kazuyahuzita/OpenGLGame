/**
* @file Particle.cpp
*/
#include "Particle.h"
 #include "Engine/Debug.h"
 #include <algorithm>
#include "Engine/Engine.cpp"


// 頂点データ構造体
struct ParticleVertex
{	float x, y; // 座標(XYのみ)	
	float u, v; // テクスチャ座標	
};

// 画像表示用データを格納する構造体(float*16個=64バイト)
struct ParticleData
 {
	vec4 position; // 座標(w要素は未使用)
	vec2 scale;    // 拡大縮小
	float cos;     // Z軸回転のcos
	float sin;     // Z軸回転のsin
	vec4 color;    // 色と不透明度
	vec4 texcoord; // テクスチャ座標
};
/**
* コンストラクタ
*
* @param ep       エミッターの初期化パラメータ
* @param pp       パーティクルの初期化パラメータ
* @param pManager 管理オブジェクトのアドレス
*/
ParticleEmitter::ParticleEmitter(const ParticleEmitterParameter& ep,
	const ParticleParameter& pp, ParticleManager* pManager) :
	ep(ep), pp(pp), pManager(pManager), interval(1.0f / ep.emissionsPerSecond)
{
	//texture = Texture::Create(ep.imagePath.c_str(), GL_LINEAR);
}


/**
* エミッターの管理下にあるパーティクルの状態を更新する
*
* @param matView    描画に使用するビュー行列
* @param deltaTime  前回の更新からの経過時間(秒)
*/
void ParticleEmitter::Update(const mat4& matView, float deltaTime)
{
	// 管理オブジェクトが未設定なら何もしない
	if (!pManager) {
		return;

	}

	// 時間を経過させる
	timer += deltaTime;
	if (timer >= ep.duration) {
		if (ep.loop) {
			timer -= ep.duration;
			emissionTimer -= ep.duration;

		}
		else {
			timer = ep.duration;

		}

	}
	// ソートに使う「視点からの深度値」を計算
	viewDepth = (matView * vec4(ep.position, 1)).z;


	// パーティクルの放出
	for (; timer - emissionTimer >= interval; emissionTimer += interval) {
		AddParticle();

	}

	// パーティクルの更新
	for (auto e : particles) {
		if (e->lifetime <= 0) {
			continue;

		}
		e->lifetime -= deltaTime;

		// 速度を計算
		const float t = e->GetLifetimeRatio();
		vec3 velocity = e->velocity.Interporate(t);

		// 半径方向の速度を計算
		velocity += e->radialDirection * e->pp->radial.Interporate(t);

		// 重力による加速度と速度を更新
		e->gravityAccel += ep.gravity * deltaTime; // 加速度
		e->gravitySpeed += e->gravityAccel * deltaTime; // 速度
		velocity.z += e->gravitySpeed;

		// 座標を更新
		e->position += velocity * deltaTime;

		// ソートに使う「視点からの深度値」を計算
		e->viewDepth = (matView * vec4(e->position, 1)).z;

	}

	// 死んでいるパーティクルを削除
	auto itr = std::partition(particles.begin(), particles.end(),
		[](const Particle* p) { return !p->IsDead(); });

	for (auto i = itr; i != particles.end(); ++i) {
		pManager->DeallocateParticle(*i); // パーティクルを返却

	}
	particles.erase(itr, particles.end());
	
	  // パーティクルをソート
	std::sort(particles.begin(), particles.end(),
		[](const Particle* a, const Particle* b)
				{ return a->viewDepth < b->viewDepth; });


}


/**
* パーティクルを追加する
*/
void ParticleEmitter::AddParticle()
{
	// パーティクルを確保できなければ何もしない
	Particle* particle = pManager->AllocateParticle();
	if (!particle) {
		return;

	}

	
	  // ローカル放出座標を計算
	vec4 position = { 0, 0, 0, 1 };
	switch (ep.shape) {
	case ParticleEmitterShape::sphere: {
		const float cosTheta = pManager->RandomFloat(-1, 1);
		const float sinTheta = std::sqrt(1 - cosTheta * cosTheta);
		const float phi = pManager->RandomFloat(0, 2 * pi);
		// 球の体積の公式4/3πr^3から、半径ごとの放出頻度は半径の3乗根で求められる
		// thicknessを3乗することで、3乗根を取ったとき最小値がthicknessと一致する
		const float t = ep.sphere.thickness;
		const float r0 = pManager->RandomFloat(1 - t * t * t, 1);
		//三乗根関数のcbrtによって、中心からの距離rを変形している
		const float r = std::cbrt(r0) * ep.sphere.radius;
		position.x = r * sinTheta * std::cos(phi);
		position.y = r * sinTheta * std::sin(phi);
		position.z = r * cosTheta;
		break;
		}
	case ParticleEmitterShape::box:
		position.x = pManager->RandomFloat(-ep.box.size.x, ep.box.size.x);
		position.y = pManager->RandomFloat(-ep.box.size.y, ep.box.size.y);
		position.z = pManager->RandomFloat(-ep.box.size.z, ep.box.size.z);
		break;
		
	}
	
	  // Z->X->Yの順で回転を適用し、放出方向へ向ける行列を計算
	const mat4 matRot = RotateY(ep.rotation.y) *
	RotateX(ep.rotation.x) * RotateZ(ep.rotation.z);
	
	// パーティクルのメンバ変数を初期化
	particle->pp = &pp;
	particle->lifetime = pp.lifetime;
	particle->position = ep.position + vec3(matRot * position);
	particle->radialDirection = normalize(particle->position - ep.position);
	particle->gravityAccel = 0;
	particle->gravitySpeed = 0;
	particle->viewDepth = 0;

	
	  // 方向ランダム係数が0でなければ、移動方向にランダムを加える
	if (ep.randomizeDirection) {
	    // 移動方向をランダム化する行列を計算
		const float randomRange = pi * ep.randomizeDirection;
	const mat4 matRandom =
		RotateY(pManager->RandomFloat(-randomRange, randomRange)) *
		RotateX(pManager->RandomFloat(-randomRange, randomRange)) *
		RotateZ(pManager->RandomFloat(-randomRange, randomRange));
		matRot * matRandom; // 回転とランダムを合成
	
		    // 半径方向をランダム化
		particle->radialDirection = vec3(matRandom * vec4(particle->radialDirection, 1));
	
	}
	// 速度ベクトルを回転
	particle->velocity.start = vec3(matRot * vec4(pp.velocity.start, 1));
	particle->velocity.end = vec3(matRot * vec4(pp.velocity.end, 1));

		
	//  // タイル数が1より多い場合、ランダムにタイルを選択する
	//if (ep.tiles.x > 1 || ep.tiles.y > 1)
	//{
	//const vec2 invTiles = 1.0f / ep.tiles;
	//const int u = pManager->RandomInt(0, static_cast<int>(ep.tiles.x) - 1);
	//const int v = pManager->RandomInt(0, static_cast<int>(ep.tiles.y) - 1);
	//particle->tc.u = static_cast<float>(u) * invTiles.x;
	//particle->tc.v = static_cast<float>(v) * invTiles.y;
	//particle->tc.sx = invTiles.x;
	//particle->tc.sy = invTiles.y;
	//
	//}
	// エミッターの管理リストにパーティクルを追加
	particles.push_back(particle);

}


/**
* パーティクル管理オブジェクトを作成する
*
* @param maxParticleCount 表示可能なパーティクルの最大数
*/
ParticleManagerPtr ParticleManager::Create(size_t maxParticleCount)
 {
	return std::make_shared<ParticleManager>(maxParticleCount);
}


/**
* コンストラクタ
*
* @param maxParticleCount 表示可能なパーティクルの最大数
*/
ParticleManager::ParticleManager(size_t maxParticleCount) :
	maxParticleCount(maxParticleCount)
{


	const ParticleVertex vertexData[] = {
	{-0.5f,-0.5f, 0, 1},
	{ 0.5f,-0.5f, 1, 1},
	{ 0.5f, 0.5f, 1, 0},
	{-0.5f, 0.5f, 0, 0},
	};
	vbo = BufferObject::Create(sizeof(vertexData), vertexData);

	const GLushort indexData[] = { 0, 1, 2, 2, 3, 0 };
	ibo = BufferObject::Create(sizeof(indexData), indexData);

	glBindVertexArray(*meshBuffer->GetVAO());
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex),
		(const void*)offsetof(ParticleVertex, u));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// シェーダを読み込んでコンパイル
	vsParticle = CompileShader(GL_VERTEX_SHADER, "Res/standard.vert");
	fsParticle = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");

	glProgramUniform1f(maxParticleCount * sizeof(ParticleData),
		GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

	// 必要なメモリ領域を確保
	emitters.reserve(100);
	particles.resize(maxParticleCount);

	// 未使用パーティクルリストを作成
	freeParticles.resize(maxParticleCount);
	auto itr = particles.end();
	for (auto& e : freeParticles) {
		--itr;
		e = &*itr;

	}

	// 乱数を初期化
	std::random_device rd;
	randomEngine.seed(rd());
}


/**
* デストラクタ
*/
ParticleManager::~ParticleManager()
{
	// すべてのエミッターからマネージャのアドレスを消去する
	for (auto& e : emitters) {
		e->pManager = nullptr;

	}
}


/**
* パーティクルの状態を更新する
*
* @param  matView 描画に使用するビュー行列
* @param deltaTime  前回の更新からの経過時間(秒)
*/
void ParticleManager::Update(const mat4& matView, float deltaTime)
{
	if (emitters.empty()) {
		return;

	}

	// エミッターの更新
	for (auto& e : emitters) {
		e->Update(matView, deltaTime);

	}

	// 死んでいるエミッターを削除
	auto itr = std::stable_partition(emitters.begin(), emitters.end(),
		[](const ParticleEmitterPtr& e) { return !e->IsDead(); });
	for (auto i = itr; i != emitters.end(); ++i) { (*i)->pManager = nullptr; }
	emitters.erase(itr, emitters.end());

	// エミッターをソート
	std::stable_sort(emitters.begin(), emitters.end(),
		[](const ParticleEmitterPtr& a, const ParticleEmitterPtr& b) {
			return a->viewDepth < b->viewDepth; });
}