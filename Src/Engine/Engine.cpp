
/**
* @file Engine.cpp
*/

#define _CRT_SECURE_NO_WARNINGS

#include "Engine.h"
#include "FramebufferObject.h"
#include "EasyAudio/EasyAudio.h"
#include "AabbCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Debug.h"
#include <fstream>
#include <filesystem>

// 図形データ
#include "../../Res/MeshData/crystal_mesh.h"
#include "../../Res/MeshData/barrel_mesh.h"
#include "../../Res/MeshData/wall_mesh.h"
#include "../../Res/MeshData/plane_xy_mesh.h"
#include "../../Res/MeshData/skull_mesh.h"

#include "../../Res/MeshData/chest_open_mesh.h"
#include "../../Res/MeshData/chest_close_mesh.h"

#include "../../Res/MeshData/door_open_mesh.h"
#include "../../Res/MeshData/door_close_mesh.h"

#include "../../Res/MeshData/pillar_mesh.h"




/**
* ゲームエンジンを実行する
*
* @retval 0     正常に実行が完了した
* @retval 0以外 エラーが発生した
*/

/**
* OpenGLからのメッセージを処理するコールバック関数
*
* @param source    メッセージの発信者(OpenGL、Windows、シェーダなど)
* @param type      メッセージの種類(エラー、警告など)
* @param id        メッセージを一位に識別する値
* @param severity  メッセージの重要度(高、中、低、最低)
* @param length    メッセージの文字数. 負数ならメッセージは0終端されている
* @param message   メッセージ本体
* @param userParam コールバック設定時に指定したポインタ
*/
void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string s;
	if (length < 0) {
		s = message;

	}
	else {
		s.assign(message, message + length);

	}
	s += '\n'; // メッセージには改行がないので追加する
	LOG(s.c_str());
}

/**
* シェーダファイルを読み込んでコンパイルする
*
* @param type     シェーダの種類
* @param filename シェーダファイル名
*
* @return シェーダの管理番号
*/
GLuint CompileShader(GLenum type, const char* filename)
{
	std::ifstream file(filename, std::ios::binary); // ファイルを開く
	if (!file) {
		LOG_ERROR("%sを開けません", filename);
		return 0;

	}

	// ファイルを読み込む
	const size_t filesize = std::filesystem::file_size(filename);
	std::vector<char> buffer(filesize);
	file.read(buffer.data(), filesize);
	file.close();

	// ソースコードを設定してコンパイル
	const char* source[] = { buffer.data() };
	const GLint length[] = { int(buffer.size()) };
	const GLuint object = glCreateShader(type);
	glShaderSource(object, 1, source, length);
	glCompileShader(object);
	return object;
}

int Engine::Run()
{

	const int result = Initialize();
	if (result) {
		return result;

	}
	
	  // 音声ライブラリを初期化
	if (!EasyAudio::Initialize()) {
		return 1; // 初期化失敗

	}
	while (!glfwWindowShouldClose(window))
	{
		Update();
		Render();
		RemoveDestroyedGameObject();
		// 音声ライブラリを更新
		EasyAudio::Update();
	}
	
	// 音声ライブラリを終了
	EasyAudio::Finalize();

	glfwTerminate();
	return 0;
}


/**
* ゲームオブジェクト配置ファイルを読み込む
*/
GameObjectList Engine::LoadGameObjectMap(
	const char* filename, const FilepathMap& filepathMap)
{
	// ファイルを開く
	std::ifstream ifs(filename);
	if (!ifs) {
		LOG_WARNING("%sを開けません", filename);
		return {};

	}

	GameObjectList gameObjectList;
	gameObjectList.reserve(1000); // 適当な数を予約
	
	  // ファイルを読み込む
	while (!ifs.eof()) {
		std::string line;
		std::getline(ifs, line);
		const char* p = line.c_str();
		int readByte = 0; // 読み込んだバイト数

		// ゲームオブジェクト定義の開始判定(先頭には必ずname要素があるはず)
		char name[256] = { 0 }; // ゲームオブジェクト名
		char tag[256] = { 0 }; // ゲームオブジェクトのタグ名
		//生文字列リテラルを読み込むための中身に変わっている
		// Rから始まり、”（）”の区切り文字になっている
		//scanfk系の関数で、％の直後の数値はバイト数になる
		if (sscanf(p, R"( { "name" : "%255[^"]" %n)", name, &readByte) != 1) {
			continue; // 定義ではない

		}
		p += readByte; // 読み取り位置を更新

		// 名前以外の要素を読み込む

		char parentName[256] = { 0 }; // 親オブジェクト名
		if (sscanf(p, R"(, "parent" : "%255[^"]" %n)", parentName, &readByte) == 1) {
			p += readByte;

		}

		char meshName[256] = { 0 }; // メッシュ名
		if (sscanf(p, R"(, "mesh" : "%255[^"]" %n)", meshName, &readByte) == 1) {
			p += readByte;

		}

		char renderType[256] = { 0 }; // 描画タイプ
		if (sscanf(p, R"(, "renderType" : "%255[^"]" %n)", renderType, &readByte) == 1) {
			p += readByte;

		}

		vec3 t(0); // 平行移動
		if (sscanf(p, R"(, "translate" : [ %f, %f, %f ] %n)",
			&t.x, &t.y, &t.z, &readByte) == 3) {
			p += readByte;

		}

		vec3 r(0); // 回転
		if (sscanf(p, R"(, "rotate" : [ %f, %f, %f ] %n)",
			&r.x, &r.y, &r.z, &readByte) == 3) {
			p += readByte;

		}

		vec3 s(1); // 拡大率
		if (sscanf(p, R"(, "scale" : [ %f, %f, %f ] %n)",
			&s.x, &s.y, &s.z, &readByte) == 3) {
			p += readByte;

		}

		// ゲームオブジェクトを作成
		// 右手座標は奥方向がーZ、左手座標は奥方向が＋Z
		// Z軸の符号反転と、X軸、Y軸の符号反転が必要
		// Unityは左手座標系なので、平行移動と回転方向を右手座標系に変換する
		auto gameObject = Create<GameObject>(name,tag);
		gameObject->position = vec3(t.x, t.y, -t.z);
		gameObject->rotation =
			vec3(radians(-r.x), radians(-r.y), radians(r.z));
		gameObject->scale = s;

		// メッシュを読み込む
		if (meshName[0]) {
			// メッシュ名に対応するファイルパスを検索
			// 対応するファイルパスが見つかればメッシュを読み込む
			const auto itr = filepathMap.find(meshName);
			if (itr != filepathMap.end()) {
				const auto& path = itr->second.c_str();
				gameObject->staticMesh = meshBuffer->LoadOBJ(path);
			}

		} // if meshName[0]
		
		// ボックスコライダーを作成
		//Unityのところで作っておいた当たり判定を
		//JSON形式だと読み込めるので、読み込む

		const char strBoxCollider[] = R"(, "BoxCollider" : [)";
		if (strncmp(p, strBoxCollider, sizeof(strBoxCollider) - 1) == 0) {
			p += sizeof(strBoxCollider) - 1;
			Box box = { vec3(0), { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } }, vec3(1) };
			for (;;) {
				if (sscanf(p, R"( { "center" : [ %f, %f, %f ], "size" : [ %f, %f, %f ] } %n)",
					&box.position.x, &box.position.y, &box.position.z,
					&box.scale.x, &box.scale.y, &box.scale.z, &readByte) != 6)
				{
					break;
				}
				auto collider = gameObject->AddComponent<BoxCollider>();
				collider->box = box;
				collider->box.scale *= 0.5f; // UnityのBoxColliderは1x1m(0.5m四方)
				collider->isStatic = true;
				p += readByte;

				// 末尾にカンマがある場合は飛ばす
				if (*p == ',') {
					++p;

				}

			}

		} // if strBoxCollider		

		// ゲームオブジェクトをリストに追加
		gameObjectList.push_back(gameObject);

	} // while


	// 作成したゲームオブジェクト配列を返す
	LOG("ゲームオブジェクト配置ファイル%sを読み込みました", filename);
	return gameObjectList;
}

/**
* ゲームエンジンから全てのゲームオブジェクトを破棄する
*/
void Engine::ClearGameObjectAll()
{
	for (auto& e : gameObjects)
	{
		e->OnDestroy();
	}	 gameObjects.clear();

	

}
/**
* 垂直視野角を設定する
*
* @param fovY 設定する垂直視野角(度数法)
*/
void Engine::SetFovY(float fovY)
{
	degFovY = fovY;
	radFovY = degFovY * 3.1415926535f / 180; // 弧度法に変換
	fovScaleY = 1 / tan(radFovY / 2); // 視野角による拡大率
}

/**
* マウスカーソルの座標を取得する
*
* @return カメラ座標系のカーソル座標
*/
vec2 Engine::GetMousePosition() const
{
	// スクリーン座標系のカーソル座標を取得
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	// 戻り型に合わせるためにfloatにキャスト
	const vec2 pos = { static_cast<float>(x), static_cast<float>(y) };

	// UILayerの座標系に合わせるために、スクリーン座標系からカメラ座標系(Z=-1)に変換する
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	const vec2 framebufferSize = { static_cast<float>(w), static_cast<float>(h) };
	const float aspectRatio = framebufferSize.x / framebufferSize.y;
	return {
	(pos.x / framebufferSize.x * 2 - 1) * aspectRatio,
	(pos.y / framebufferSize.y * 2 - 1) * -1 };
}

/**
* マウスボタンの状態を取得する
*
* @param button 調べるボタンを示すマクロ定数(GLFW_MOUSE_BUTTON_LEFT等)
*
* @retval true  押されている
* @retval false 押されていない
*/
bool Engine::GetMouseButton(int button) const
{
	return glfwGetMouseButton(window, button) == GLFW_PRESS;
}
/**
* マウスボタンのクリック状態を取得する
*/
bool Engine::GetMouseClick(int button) const
{
	// 範囲外のボタン番号は無視
	if (button < GLFW_MOUSE_BUTTON_LEFT || button > GLFW_MOUSE_BUTTON_MIDDLE) {
		return false;

	}
	return mouseButtons[button].click;
}
/**
* フレームバッファの大きさを取得する
*
* @return フレームバッファの縦と横のサイズ
*/
vec2 Engine::GetFramebufferSize() const
 {	
	int w, h;	
	glfwGetFramebufferSize(window, &w, &h);	
	return 
	{ 
		static_cast<float>(w), 
		static_cast<float>(h) 
	};	
}

/**
* フレームバッファのアスペクト比を取得する
*
* @return フレームバッファのアスペクト比
*/
float Engine::GetAspectRatio() const
{
	const vec2 size = GetFramebufferSize();
	return size.x / size.y;
}

/**
* テクスチャの取得
*/
TexturePtr Engine::GetTexture(const char* name)
 {
	  // キャッシュにあれば、キャシュされたテクスチャを返す
	auto itr = textureCache.find(name);
	if (itr != textureCache.end()) {
		return itr->second; // キャッシュされたテクスチャを返す
		
	}
	
	 // キャッシュになければ、テクスチャを作成してキャッシュに登録
	auto tex = std::make_shared<TexHelper>(name);
	textureCache.emplace(name, tex);
	return tex; // 作成したテクスチャを返す
	}
/**
  * ライト配列を初期化する
  */
void Engine::InitializeLight()
{
	// 指定された数のライトを生成
	//usedLights配列はリザーブによって初期化
	lights.resize(lightResizeCount);
	//freeLights配列はリサイズによって初期化
	usedLights.reserve(lightResizeCount);

	// すべてのライトを未使用ライト配列に「逆順」で追加
	//AllocateLight関数で新しいライトを取得するとき、ライト配列の先頭から取得させるため
	freeLights.resize(lightResizeCount);
	for (int i = 0; i < lightResizeCount; ++i) {
		freeLights[i] = static_cast<int>(lightResizeCount - i - 1);

	} // for i
}


/*
* 
* 未使用のライトをひとつ取り出して状態を「使用中」に変更し、そのインデックスを返します
* 
* @return 
*/
int Engine::AllocateLight()
{
	// 未使用のライトがなければライト配列を拡張する
	if (freeLights.empty())
	{
		const int oldSize = lights.size();
		lights.resize(oldSize + lightResizeCount);
		// 拡張したライトを未使用ライト配列に「逆順」で追加
		for (int i = lights.size() - 1; i >= oldSize; --i)
		{
			freeLights.push_back(static_cast<int>(i));

		}

	}
	
	// 未使用ライト配列の末尾からインデックスを取り出す
	const int index = freeLights.back();
	freeLights.pop_back();
	
	//取り出したインデックスを使用中ライト配列に追加
	//配列のポインタであるため、本来はアドレスが変化した時点で使えなくなる
	//しかし、ライトをインデックスで管理しているため、アドレスが変化しても
	//目的のライトが見つかるようになっている

	usedLights.push_back(index);
	
	  // ライトの状態を「使用中」にする
	lights[index].used = true;
	
	  // 取り出したインデックスを返す
	return index;
}

/*
* 
* 割り当てを取り消す、解除する
* 
* AllocateLightで取得したインデックスをエンジンに返却し、ライトを未使用状態にする
*/
void Engine::DeallocateLight(int index)
 {
	if (index >= 0 && index < lights.size()) 
	{
		    // インデックスを未使用ライト配列に追加
		freeLights.push_back(index);
		
		    // ライトの状態を「未使用」にする
		lights[index].used = false;
		
	} // if
}

//ライトデータを取得する奴
LightData* Engine::GetLight(int index)
 {	
	if (index >= 0 && index < lights.size() && lights[index].used) {
		return &lights[index];		
	}	
	return nullptr;	
}

const LightData* Engine::GetLight(int index) const
 {
	return const_cast<Engine*>(this)->GetLight(index);
}

/**
* マウス座標から発射される光線を取得する
* ワールド座標系が欲しいので、スクリーン座標系からワールド座標系に変換する
* スクリーン座標系
*		↓
* NDC座標系
*		↓
* クリップ座標系
*		↓
* ビュー座標系
*		↓
* ワールド座標系
*/
Ray Engine::GetRayFromMousePosition() const
{
	// スクリーン座標系のマウスカーソル座標を取得
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	// スクリーン座標系からNDC座標系に変換
	//  NDC座標系 : -1～+1の範囲の座標系
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	vec3 nearPos = {
	static_cast<float>(x / w * 2 - 1),
	-static_cast<float>(y / h * 2 - 1),
	-1 };
	vec3 farPos = { nearPos.x, nearPos.y, 1 };

	// 深度値の計算結果が-1～+1になるようなパラメータA, Bを計算
	// 頂点シェーダの値を一致させること
	const float near = 0.35f;
	const float far = 1000;
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);
	
	// NDC座標系からクリップ座標系に変換
	//wによる除算」を逆に実行することで実現できる
	nearPos *= near;
	farPos *= far;
	nearPos.z = (nearPos.z - A) / B;
	farPos.z = (farPos.z - A) / B;

	
	// クリップ座標系からビュー座標系に変換
	//FOVの比率とアスペクト比を利用する
	const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
	const float invFovScale = 1.0f / GetFovScale();
	nearPos.x *= invFovScale * aspectRatio;
	nearPos.y *= invFovScale;
	farPos.x *= invFovScale * aspectRatio;
	farPos.y *= invFovScale;

	//カメラの回転を素直に適用した座標変換
	//カメラ座標を減算してから回転したため、逆に回転してからカメラの座標を加算
	  // ビュー座標系からワールド座標系に変換
	const float cameraSinY = std::sin(camera.rotation.y);
	const float cameraCosY = std::cos(camera.rotation.y);
	nearPos = {
	nearPos.x * cameraCosY - near * cameraSinY,
	nearPos.y,
	nearPos.x * -cameraSinY - near * cameraCosY };
	nearPos += camera.position;
	
	farPos = {
	farPos.x * cameraCosY - far * cameraSinY,
	farPos.y,
	farPos.x * -cameraSinY - far * cameraCosY };
	farPos += camera.position;


	  // 近平面の座標と遠平面の座標から光線の向きベクトルを求める
	//向きベクトルは正規化する必要があるため、ベクトルの長さで割っている
	vec3 direction = farPos - nearPos;
	const float length = sqrt(
	direction.x * direction.x +
	direction.y * direction.y +
	direction.z * direction.z);
	direction *= 1.0f / length;
	
	return Ray{ nearPos, direction };

}

/**
* 光線とコライダーの交差判定
*/
bool Engine::Raycast(const Ray& ray, RaycastHit& hitInfo,
	const RaycastPredicate& pred) const
{
	// 交点の情報を初期化
	hitInfo.collider = nullptr;
	hitInfo.distance = FLT_MAX;

	for (const auto& go : gameObjects) {
		for (const auto& collider : go->colliders) {
			// AABBをワールド座標系に変換
			// コライダーをワールド座標系に変換
			const auto worldCollider =
			collider->GetTransformedCollider(go->GetTransformMatrix());

			// 光線との交差判定
			float d;
			bool hit = false;
			switch (collider->GetType()) {
			case Collider::Type::AABB:
				hit = Intersect(static_cast<AabbCollider&>(*worldCollider).aabb, ray, d);
				break;
			case Collider::Type::Sphere:
				hit = Intersect(static_cast<SphereCollider&>(*worldCollider).sphere, ray, d);
				break;
			case Collider::Type::Box:
				hit = Intersect(static_cast<BoxCollider&>(*worldCollider).box, ray, d);
				break;

			
			}

			if (!hit) {
				continue;

			}

			// 交差判定の対象でなければ飛ばす
			if (!pred(collider, d)) {
				continue;

			}

			// より発射点に近い交点を持つコライダーを選ぶ
			if (d < hitInfo.distance) {
				hitInfo.collider = collider;
				hitInfo.distance = d;

			}

		} // for colliders

	} // for gameObjects

		  // 交差するコライダーがあれればtrue、なければfalseを返す
	if (hitInfo.collider) {
		// 交点の座標を計算
		hitInfo.point = ray.start + ray.direction * hitInfo.distance;
		return true;

	}
	return false;
}
/**
* ゲームエンジンを初期化する
*
* @retval 0     正常に初期化された
* @retval 0以外 エラーが発生した
*/
int Engine::Initialize()
{
	//GLFWの初期化
	if (glfwInit() != GLFW_TRUE) {
		return 1;	//初期化失敗
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	window = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		return 1;		//ウィンドウ作成失敗
	}

	//OpenGLコンテキストの作成
	glfwMakeContextCurrent(window);

	//OpenGL関数のアドレスを取得
	//ライブラリ間の不整合を解消するためにreinterpret_castを使う
	//ポインタ型の戻り値を使う関数を使用するためCastが必要	
	//やばいが任意のポインタ型に返すためにreinterpret_castを使う
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwTerminate();
		return 1;	//アドレス取得失敗
	}

	// メッセージコールバックの設定::nullptrのところはuserParam関数の引数に渡される値）
	glDebugMessageCallback(DebugCallback, nullptr);

	//ライトを初期化する
	InitializeLight();

	// シェーダを読み込んでコンパイル
	vs = CompileShader(GL_VERTEX_SHADER, "Res/standard.vert");
	fs = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");

	// 2つのシェーダをリンク
	prog3D = glCreateProgram();
	glAttachShader(prog3D, vs);
	glAttachShader(prog3D, fs);
	glLinkProgram(prog3D);
	
	// アンリット・シェーダを作成
	// 二つのシェーダをリンク
	vsUnlit = CompileShader(GL_VERTEX_SHADER, "Res/unlit.vert");
	fsUnlit = CompileShader(GL_FRAGMENT_SHADER, "Res/unlit.frag");
	progUnlit = glCreateProgram();
	glAttachShader(progUnlit, vsUnlit);
	glAttachShader(progUnlit, fsUnlit);
	glLinkProgram(progUnlit);

	// スケルタル・シェーダを作成
	vsSkeletal = CompileShader(GL_VERTEX_SHADER, "Res/skeletal.vert");
	fsSkeletal = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");
	progSkeletal = glCreateProgram();
	glAttachShader(progSkeletal, vsSkeletal);
	glAttachShader(progSkeletal, fsSkeletal);
	glLinkProgram(progSkeletal);

	// シャドウ・シェーダを作成
	vsShadow = CompileShader(GL_VERTEX_SHADER, "Res/shadow.vert");
	fsShadow = CompileShader(GL_FRAGMENT_SHADER, "Res/shadow.frag");
	progShadow = glCreateProgram();
	glAttachShader(progShadow, vsShadow);
	glAttachShader(progShadow, fsShadow);
	glLinkProgram(progShadow);
	glProgramUniform1f(progShadow, locAlphaCutoff, 0.5f); // カットオフ値を設定しておく
	
	// FBOを作成
	//深度バッファだけ必要なので、深度テクスチャを作成
	//GPUやドライバによって精度を差分けられる
	//今回はGL_DEPTH_COMPONENT32（GeForce(Nvidia)は凄い！）
	//24,32がいまでは無難らしい
	//
	auto texShadow = std::make_shared<TexHelper>(
		"FBO(depth)", 2048, 2048, GL_DEPTH_COMPONENT32);
	fboShadow = std::make_shared<FramebufferObject>(nullptr, texShadow);
	// 関節用のSSBOを作成
	ssboJointMatrices = BufferObject::Create(jointMatricesBytes);

	// 図形データの情報
	struct MeshData {
		const char* name;            // メッシュ名
		const char* textureFilename; // テクスチャファイル名

		size_t vertexSize;      // 頂点データのバイト数
		size_t indexSize;       // インデックスデータのバイト数
		const void* vertexData; // 頂点データのアドレス
		const void* indexData;  // インデックスデータのアドレス

	};
	const MeshData meshes[] =
	{ 
		{"crystal", "Res/MeshData/crystal_blue.tga", sizeof(crystal_vertices), sizeof(crystal_indices), crystal_vertices, crystal_indices },
		{"barrel", "Res/MeshData/barrel.tga", sizeof(barrel_vertices), sizeof(barrel_indices), barrel_vertices, barrel_indices },
		{ "plane_xy", nullptr,sizeof(plane_xy_vertices), sizeof(plane_xy_indices), plane_xy_vertices, plane_xy_indices },
		{ "wall", "Res/MeshData/barrel.tga",sizeof(wall_vertices), sizeof(wall_indices),wall_vertices, wall_indices},
		{"skull", "Res/MeshData/skull.tga", sizeof(skull_vertices), sizeof(skull_indices), skull_vertices, skull_indices },
		{"chest", "Res/MeshData/chest.tga", sizeof(chest_close_vertices), sizeof(chest_close_indices), chest_close_vertices, chest_close_indices },
		{"pillar", "Res/MeshData/pillar.tga", sizeof(pillar_vertices), sizeof(pillar_indices), pillar_vertices, pillar_indices },
	};
	meshBuffer = MeshBuffer::Create(32'000'000);
	meshBuffer->SetTextureCallback(
		[this](const char* filename) { return GetTexture(filename); });

	for (const auto& e : meshes)
	{
		// 法線を設定するために図形データのコピーを作る
		auto pVertex = static_cast<const Vertex*>(e.vertexData);
		auto pIndex = static_cast<const uint16_t*>(e.indexData);
		std::vector<Vertex> v(pVertex, pVertex + e.vertexSize / sizeof(Vertex));

		// コピーした図形データに法線を設定
		for (auto& e : v) {
			e.normal = { 0, 0, 0 };

		}
		FillMissingNormals(v.data(), v.size(), pIndex, e.indexSize / sizeof(uint16_t));

		// 法線を設定した図形データをGPUメモリにコピー
		meshBuffer->AddVertexData(v.data(), e.vertexSize, pIndex, e.indexSize);


		// 直前のAddVertexDataで作成した描画パラメータを取得
		const DrawParams& drawParams =
			meshBuffer->GetDrawParams(meshBuffer->GetDrawParamsCount() - 1);

		// テクスチャを作成
		TexturePtr texBaseColor;
		if (e.textureFilename) {
			texBaseColor = GetTexture(e.textureFilename);
		}

		// 描画パラメータとテクスチャからスタティックメッシュを作成
		meshBuffer->CreateStaticMesh(e.name, drawParams, texBaseColor);

	}
	
	 // OBJファイルを読み込む
	 skySphere = meshBuffer->LoadOBJ("Res/MeshData/sky_sphere/sky_sphere.obj");
	 cloud = meshBuffer->LoadOBJ("Res/MeshData/Athletic/SlidingBlock/SlidingBlock.obj");

	 //
	 meshBuffer->LoadOBJ("Res/MeshData/Stair/Stair.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Door/MainDoor.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Door/MainDoorFrame.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Spawned/SpawnerEnemy.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/ghost/ghost.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Bullet/Bullet.obj");

	 //AthleticScene用のゲームオブジェクトの設定
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Block/Block.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Enemy/Enemy.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Face/Face.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/FrontBackBoard/Base.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/FrontBackBoard/FrontBackBoard.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Gate/Gate.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Gems/Gem.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/JumpingPlatforms/JumpingPlatforms.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Pendulum/Basis.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Pendulum/Hammer.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/TextBlock/TextBlock.obj");
	// meshBuffer->LoadOBJ("Res/MeshData/Athletic/RotationBlock/RotationBlock.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/RotateBar/Center/RotateBar_Center.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/RotateBar/Wide/RotateBar_Wide.obj");

	 //DeathForestシーン用のゲームオブジェクトの設定
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/GoalBlock/GoalBlock.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Light/Light.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/PlayerSphere/PlayerSphere.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Motor/Motor.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Rock/Rock.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Face/Yoshie.obj");

	 //集めるパーツ
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Item/Battery.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Item/Gas_Can.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Item/Seat.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Item/Tyre.obj");

	// カメラの初期設定
	camera.position = { 3, 1, 3 };
	camera.rotation.x = 3.14159265f;
	camera.rotation.y = 3.14159265f;
	return 0; // 正常に初期化された
}

/**
* ゲームエンジンの状態を更新する
*/
void Engine::Update()
{

	//=============時間の計測====================//
	  // デルタタイム(前回の更新からの経過時間)を計算
	const double currentTime = glfwGetTime(); // 現在時刻
	deltaTime = static_cast<float>(currentTime - previousTime);
	previousTime = currentTime;
	
	if (deltaTime > 0.5f) {
		deltaTime = 1.0f / 60.0f;
	}
	//===========================================//
	
	  // マウスボタンの状態を取得
	for (size_t i = 0; i < std::size(mouseButtons); ++i) {
		MouseButton& e = mouseButtons[i];
		e.previous = e.current;
		e.current = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT + i);

		// 押されている時間を更新
		if (e.previous) {
			e.timer += deltaTime;

		}
		else {
			e.timer = 0;

		}

		// 以下の条件を全て満たす場合はクリックと判定
		// - 前フレームで押されている
		// - 現在フレームで押されていない
		// - 押されている時間が0.3秒以下
		if (e.previous && !e.current && e.timer <= mouseClickSpeed) {
			e.click = true;

		}
		else {
			e.click = false;

		}

	} // for i

	// シーンの切り替え
	if (nextScene)
	{
		if (scene)
		{
			scene->Finalize(*this);

		}
		nextScene->Initialize(*this);
		scene = std::move(nextScene);

	}

	// シーンを更新
	if (scene) {
		scene->Update(*this, deltaTime);

	}

	UpdateGameObject(deltaTime);
	HandleGameObjectCollision();
}


/**
* ゲームオブジェクトの衝突を処理する
*/
void Engine::HandleGameObjectCollision()
{
	// ワールド座標系の衝突判定を作成
	std::vector<WorldColliderList> colliders;
	colliders.reserve(gameObjects.size());
	for (const auto& e : gameObjects) {
		if (e->colliders.empty())
		{
			continue;
		}

		// 「接地していない」状態にする
		e->isGrounded = false;

		//衝突判定を作成
		WorldColliderList list(e->colliders.size());
		for (int i = 0; i < e->colliders.size(); ++i)
		{
			// オリジナルのコライダーをコピー
			list[i].origin = e->colliders[i];
			list[i].world = e->colliders[i]->
				GetTransformedCollider(e->GetTransformMatrix());
		}
		colliders.push_back(list);
	}


	if (colliders.size() >= 2) {
		// ゲームオブジェクト単位の衝突判定
		for (auto a = colliders.begin(); a != colliders.end() - 1; ++a)
		{
			const GameObject* goA = a->at(0).origin->GetOwner();
			if (goA->IsDestroyed())
			{
				continue; // 削除済みなので飛ばす

			}
			for (auto b = a + 1; b != colliders.end(); ++b)
			{
				const GameObject* goB = b->at(0).origin->GetOwner();
				if (goB->IsDestroyed())
				{
					continue; // 削除済みなので飛ばす

				}
				HandleWorldColliderCollision(&*a, &*b); // コライダー単位の衝突判定
			} // for b

		} // for a

	}
} // HandleGameObjectCollision

/**
* 型によって交差判定関数を呼び分けるための関数テンプレート
*/
template<typename T, typename U>
bool CallIntersect(const ColliderPtr& a, const ColliderPtr& b, vec3& p)
{
	return Intersect(static_cast<T&>(*a).GetShape(),
		static_cast<U&>(*b).GetShape(), p);
}


/**
* 型によって交差判定関数を呼び分けるための関数テンプレート
*
* 交差判定関数に渡す引数を逆にするバージョン
*/
//AABBと球体の判定はあるが、球体とAABBの判定は無いので
//逆バージョンも定義
template<typename T, typename U>
bool CallIntersectReverse(const ColliderPtr& a, const ColliderPtr& b, vec3& p)
{
	if (Intersect(static_cast<U&>(*b).GetShape(),
		static_cast<T&>(*a).GetShape(), p)) {
		p *= -1; // 貫通ベクトルを逆向きにする
		//貫通ベクトルの向きと、引数a,bが関係しているため
		//逆向きを判定するなら、貫通ベクトルも逆に
		return true;
		
	}
	return false;
}


/**
* 常にfalseを返す関数テンプレート
*
* 未実装または実装予定のない組み合わせ用
*/
template<typename T, typename U>
bool NotImplemented(const ColliderPtr& a, const ColliderPtr& b, vec3& p)
{
	return false;
}


/**
* 貫通ベクトルをゲームオブジェクトに反映する
*/
void Engine::ApplyPenetration(WorldColliderList* worldColliders,
	GameObject* gameObject, const vec3& penetration)
{
	// 接地判定
	// 衝突ベクトルが垂直に近い場合に、床に触れたとみなす
	static const float cosGround = cos(radians(30)); // 床とみなす角度
	static const float sinGround = sin(radians(30)); // 床とみなす角度
	//貫通ベクトルが明らかに判定が取れるときは
	//あらかじめ除外しておく
	if (penetration.y > 0) {
		// 対象が単位垂直ベクトルであることを利用して、内積による角度の比較を単純化
		const float d = length(penetration);
		if (penetration.y >= d * cosGround) {
			gameObject->isGrounded = true; // 接地した

		}

	}// if penetration.y
	if (penetration.x > 0) {
		// 対象が単位垂直ベクトルであることを利用して、内積による角度の比較を単純化
		const float d = length(penetration);
		if (penetration.x >= d * sinGround) {
			gameObject->isGrounded = true; // 接地した

		}

	}// if penetration.x

// ゲームオブジェクトを移動
	gameObject->position += penetration;

	// 全てのワールドコライダーを移動
	for (auto& e : *worldColliders) {
		e.world->AddPosition(penetration);

	}
}

/**
* コライダー単位の衝突判定
*
* @param a 判定対象のワールドコライダー配列その1
* @param b 判定対象のワールドコライダー配列その2
*/
void Engine::HandleWorldColliderCollision(WorldColliderList* a, WorldColliderList* b)
{
	  // 関数ポインタ型を定義
	using FuncType = bool(*)(const ColliderPtr&, const ColliderPtr&, vec3&);
	
	  // 組み合わせに対応する交差判定関数を選ぶための配列
	//まだ作っていない組み合わせに対しては、必ずfalseをかえすようにする
	static const FuncType funcList[3][3] = {
		{
		CallIntersect<AabbCollider, AabbCollider>,
		CallIntersect<AabbCollider, SphereCollider>,
		NotImplemented<AabbCollider, BoxCollider>,
		},
		{
		CallIntersectReverse<SphereCollider, AabbCollider>,
		CallIntersect<SphereCollider, SphereCollider>,
		CallIntersectReverse<SphereCollider, BoxCollider>,
		},
		{
		NotImplemented<BoxCollider, AabbCollider>,
		CallIntersect<BoxCollider, SphereCollider>,
		NotImplemented<BoxCollider, BoxCollider>,
		},
	};
	
	// コライダー単位の衝突判定
	for ( auto& colA : *a)
	{
		for ( auto& colB : *b)
		{
			// スタティックコライダー同士は衝突しない
			if (colA.origin->isStatic && colB.origin->isStatic)
			{
				continue;

			}

			// 衝突判定を行う
			vec3 penetration;
			const int typeA = static_cast<int>(colA.origin->GetType());
			const int typeB = static_cast<int>(colB.origin->GetType());

			//関数ポインタの二次元配列
			//コライダーの型によって、関数を呼び分けている
			if (funcList[typeA][typeB](colA.world, colB.world, penetration)) {
				
				GameObject* goA = colA.origin->GetOwner();
				GameObject* goB = colB.origin->GetOwner();

				// コライダーが重ならないように座標を調整
				if (!colA.origin->isTrigger && !colB.origin->isTrigger)
				{
					if (colA.origin->isStatic) {
						// Aは動かないのでBを移動させる
						
						ApplyPenetration(b, goB, penetration);
					}
					else if (colB.origin->isStatic) {
						//VecMath.hの方で定義しておいた演算子ですっきりしたコードに
						// Bは動かないのでAを移動させる
						ApplyPenetration(a, goA, -penetration);
					}
					else {
						// AとBを均等に移動させる
						ApplyPenetration(b, goB, penetration * 0.5f);
						ApplyPenetration(a, goA, penetration * -0.5f);

					}

				}


				// イベント関数の呼び出し
				goA->OnCollision(colA.origin, colB.origin);
				goB->OnCollision(colB.origin, colA.origin);

				// イベントの結果、どちらかのゲームオブジェクトが破棄されたらループ終了
				if (goA->IsDestroyed() || goB->IsDestroyed()) {
					return; // 関数を終了
					
				}
			}

		} // for colB

	} // for colA

}


/**
* 破棄予定のゲームオブジェクトを削除する
*/
void Engine::RemoveDestroyedGameObject()
{
	if (gameObjects.empty()) {
		return; // ゲームオブジェクトを持っていなければ何もしない
	}

	// 破棄予定の有無でゲームオブジェクトを分ける
	const auto iter = std::partition(
		gameObjects.begin(), gameObjects.end(),
		[](const GameObjectPtr& e) { return !e->IsDestroyed(); });

	// 破棄予定のオブジェクトを別の配列に移動
	GameObjectList destroyList(
		std::make_move_iterator(iter),
		std::make_move_iterator(gameObjects.end()));

	// 配列から移動済みオブジェクトを削除
	gameObjects.erase(iter, gameObjects.end());

	// 破棄予定のオブジェクトのOnDestroyを実行
	for (auto& e : destroyList) {
		e->OnDestroy();

	}

	// ここで実際にゲームオブジェクトが削除される(destoyListの寿命が終わるため)
}


/**
* カメラに近いライトを選んでGPUメモリにコピーする
*/
void Engine::UpdateShaderLight()
{
	// 環境光をGPUメモリにコピー
	glProgramUniform3fv(prog3D, locAmbientLight, 1, &ambientLight.x);
	
	// 平行光源のパラメータをGPUメモリにコピー
	const vec3 color = directionalLight.color * directionalLight.intensity;
	glProgramUniform3fv(prog3D, locDirectionalLightColor, 1, &color.x);
	glProgramUniform3fv(prog3D, locDirectionalLightDirection,
		1, &directionalLight.direction.x);

		// コピーするライトがなければライト数を0に設定
	if (usedLights.empty()) {
		glProgramUniform1i(prog3D, locLightCount, 0);
		return;
	}
		
	
		//未使用になったライト	・・・・
		// usedLights配列に含まれるライトのうち
		// 「usedメンバ変数がfalseになっているライト」
		// 使用中ライトの配列から、未使用になったライトを除外する
		
		const auto itrUnused = std::remove_if(usedLights.begin(), usedLights.end(),
		[&](int i) { return !lights[i].used; });
		//vectorオブジェクトから特定の条件を満たすデータを削除するために
		//remove_if関数　と　eraseメンバ関数を組み合わせる
		usedLights.erase(itrUnused, usedLights.end());

		//重複する番号を除去する
		//整理された奴をユニークにしたい
		std::sort(usedLights.begin(), usedLights.end());
		//連続(1,2,3,4,,,)で並んでいないと行けないため、
		//あらかじめソートしておく
		
		auto itr = std::unique(usedLights.begin(), usedLights.end());
		usedLights.erase(itr, usedLights.end());
			
		// カメラの正面ベクトルを計算
		const vec3 front = { -sin(camera.rotation.y), 0, -cos(camera.rotation.y) };

		  // カメラからライトまでの距離を計算
		struct Distance 
		{
			float distance;      // カメラからライトの半径までの距離
			const LightData* p; // ライトのアドレス
			
		};
		std::vector<Distance> distanceList;
		distanceList.reserve(lights.size());
		for (auto index : usedLights) 
		{
			const auto& light = lights[index];
			const vec3 v = light.position - camera.position;
			// カメラの後ろにあるライトを除外
			//dot算で負になるときはライトがカメラの後ろにある判定
			
			if (dot(front, v) <= -light.radius)
			{
				continue;

			}
			const float d = length(v) - light.radius; // カメラからライトの半径までの距離
			distanceList.push_back({ d, &light });

		}
		
		  // 画面に影響するライトがなければライト数を0に設定
		if (distanceList.empty())
		{
			glProgramUniform1i(prog3D, locLightCount, 0);
			return;
		
		}
		  // カメラに近いライトを優先する
		//ライトの距離順を並び替える
		
		std::stable_sort(
			distanceList.begin(), distanceList.end(),

			[&](const auto& a, const auto& b)
			{
				return a.distance < b.distance;
			});

		  // ライトデータをGPUメモリにコピー
		const int lightCount = static_cast<int>(
		std::min(distanceList.size(), maxShaderLightCount)); // コピーするライト数
		
		std::vector<vec4> colorAndFalloffAngle(lightCount);
		std::vector<vec4> positionAndRadius(lightCount);
		std::vector<vec4> directionAndConeAngle(lightCount);
		
		for (int i = 0; i < lightCount; ++i) {
			const LightData* p = distanceList[i].p;
			const vec3 color = p->color * p->intensity;
			colorAndFalloffAngle[i] = {
			color.x, color.y, color.z, p->falloffAngle 
			};
			positionAndRadius[i] = {
			p->position.x, p->position.y, p->position.z, p->radius 
			};
			directionAndConeAngle[i] = {
			p->direction.x, p->direction.y, p->direction.z, p->coneAngle 
			};

		}
		glProgramUniform4fv(prog3D, locLightColorAndFalloffAngle,
			lightCount, &colorAndFalloffAngle[0].x);

		glProgramUniform4fv(prog3D, locLightPositionAndRadius,
			lightCount, &positionAndRadius[0].x);

		glProgramUniform4fv(prog3D, locLightDirectionAndConeAngle,
			lightCount, &directionAndConeAngle[0].x);

		glProgramUniform1i(prog3D, locLightCount, lightCount);

		// UpdateShaderLightメンバ関数が完成です。
}


/**
* スカイスフィアを描画する
*/
void Engine::DrawSkySphere()
{
	// シーンにスカイスフィアが設定されていない場合は描画しない
	if (!skySphere || !scene->skysphereMaterial) {
		return;

	}

	// アンリットシェーダで描画
	glUseProgram(progUnlit);
	glBindVertexArray(*meshBuffer->GetVAO());
	glDepthMask(GL_FALSE); // 深度バッファへの書き込みを禁止

	// スカイスフィアモデルの半径を0.5mと仮定し、描画範囲の95%の大きさに拡大
	static const float far = 1000; // 描画範囲の最大値
	static const float scale = far * 0.95f / 0.5f;
	static const mat4 transformMatrix ={
	vec4(scale, 0,    0,  0),
	vec4(0, scale,    0,  0),
	vec4(0,     0, scale, 0),
	vec4(0,     0,    0,  1),
	};
	glProgramUniformMatrix4fv(progUnlit,
		locTransformMatrix, 1, GL_FALSE, &transformMatrix[0].x);

	// 色はマテリアルカラーで調整するのでオブジェクトカラーは白に設定
	static const vec4 color = { 1, 1, 1, 1 };
	glProgramUniform4fv(progUnlit, 100, 1, &color.x);
	
	// スカイスフィアはカメラを原点として描画
	static const vec3 skySphereCameraPosition = { 0, 0, 0, };
	glProgramUniform3fv(progUnlit, 4, 1, &skySphereCameraPosition.x);
	
	// スカイスフィアを描画
	const MaterialList materials(1, scene->skysphereMaterial);
	Draw(*skySphere, progUnlit, materials);
	
	glProgramUniform3fv(progUnlit, 4, 1, &camera.position.x); // カメラ座標を元に戻す
	glDepthMask(GL_TRUE); // 深度バッファへの書き込みを許可

	//ここで戻しておかないと、半透明描画が出来なくなっていしまうので
	//ちゃんと戻しておく
	glUseProgram(prog3D);

}

/**
* デプスシャドウマップを作成
*/
void Engine::CreateShadowMap(
	GameObjectList::iterator begin,
	GameObjectList::iterator end)
{
	// 描画先フレームバッファを変更
	glBindFramebuffer(GL_FRAMEBUFFER, *fboShadow);

	// ビューポートをフレームバッファのサイズに合わせる
	const auto& texShadow = fboShadow->GetDepthTexture();
	glViewport(0, 0, texShadow->GetWidth(), texShadow->GetHeight());

	//カラーバッファを持たないため深度だけでいい
	// 深度バッファをクリア
	glClear(GL_DEPTH_BUFFER_BIT);

	// VAOとシェーダをOpenGLコンテキストに割り当てる
	glBindVertexArray(*meshBuffer->GetVAO());
	glUseProgram(progShadow);


	// 影の描画パラメータ
	const float shadowAreaSize = 100; // 影の描画範囲(XY平面)
	const float shadowNearZ = 1;      // 影の描画範囲(近Z平面)
	const float shadowFarZ = 200;     // 影の描画範囲(遠Z平面)
	const float shadowCenterZ = (shadowNearZ + shadowFarZ) * 0.5f; // 描画範囲の中心
	const vec3 target = { 0, 0, -50 }; // カメラの注視点
	const vec3 eye =
		target - directionalLight.direction * shadowCenterZ; // カメラの位置

	// ビュープロジェクション行列を計算
	const mat4 matShadowView = LookAt(eye, target, vec3(0, 1, 0));
	const mat4 matShadowProj = Orthogonal(
		-shadowAreaSize / 2, shadowAreaSize / 2,
		-shadowAreaSize / 2, shadowAreaSize / 2,
		shadowNearZ, shadowFarZ);
	const mat4 matShadow = matShadowProj * matShadowView;

	// ビュープロジェクション行列をGPUメモリにコピー
	glProgramUniformMatrix4fv(progShadow,
		locViewProjectionMatrix, 1, GL_FALSE, &matShadow[0].x);

	// メッシュを描画
	for (GameObjectList::iterator i = begin; i != end; ++i) {
		const auto& e = *i;
		if (!e->staticMesh) {
			continue;

		}

		glProgramUniformMatrix4fv(progShadow,
			locTransformMatrix, 1, GL_FALSE, &e->GetTransformMatrix()[0].x);
		if (e->materials.empty()) {
			Draw(*e->staticMesh, progShadow, e->staticMesh->materials);

		}
		else {
			Draw(*e->staticMesh, progShadow, e->materials);

		}

	} // for

	// 深度テクスチャを割り当てる
	glBindTextureUnit(2, *texShadow);

	// 深度テクスチャ座標への変換行列を作成
	static const mat4 matTexture = {
	{ 0.5f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.5f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.5f, 0.0f },
	{ 0.5f, 0.5f, 0.5f, 1.0f } };

	// シャドウテクスチャ行列をGPUメモリにコピー
	const mat4 matShadowTexture = matTexture * matShadowProj * matShadowView;
	glProgramUniformMatrix4fv(prog3D,
		locShadowTextureMatrix, 1, GL_FALSE, &matShadowTexture[0].x);

	  // 法線方向の補正値を設定
	const float texelSize = shadowAreaSize / static_cast<float>(texShadow->GetWidth());
	glProgramUniform1f(prog3D, locShadowNormalOffset, texelSize);

}
/**
* ゲームエンジンの状態を描画する
*/
void Engine::Render()
{

	//バックバッファをクリア
	//色を指定するの
	glClearColor(0.1f, 0.2f, 0.5f, 1.0f);
	//色を描画しなおすの
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//GPUが早く作動するためにいる

	// フレームバッファの大きさを取得
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	const GLuint programs[] = { prog3D, progUnlit,progSkeletal };
	for (auto prog : programs)
	{
		// アスペクト比と視野角を設定
		const float aspectRatio =
			static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
		glProgramUniform2f(prog, 3, fovScaleY / aspectRatio, fovScaleY);

		// カメラパラメータを設定
		glProgramUniform3fv(prog, 4, 1, &camera.position.x);
		glProgramUniform2f(prog, 5, sin(-camera.rotation.y), cos(-camera.rotation.y));
		glProgramUniform2f(prog, 7, sin(-camera.rotation.x), cos(-camera.rotation.x));

	}

	// 深度テストを有効化
	glEnable(GL_DEPTH_TEST);
	
	 // 半透明合成を有効化
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	
	UpdateShaderLight();
	  // ゲームオブジェクトをレンダーキュー順に並べ替える
	std::stable_sort(
		gameObjects.begin(), gameObjects.end(),
		[](const GameObjectPtr& a, const GameObjectPtr& b) {return a->renderQueue < b->renderQueue; }
	);
	
	//半透明同士での深度バッファの書き込みを失敗してしまうので
	// 深度バッファの書き込みを禁止にする
	// 
	// transparentキューの先頭を検索
	const auto transparentBegin = std::lower_bound(
		gameObjects.begin(), gameObjects.end(), RenderQueue_transparent,
		[](const GameObjectPtr& e, int value) { return e->renderQueue < value; });

	  // overlayキューの先頭を検索
	const auto overlayBegin = std::lower_bound(
		transparentBegin, gameObjects.end(), RenderQueue_overlay,
		[](const GameObjectPtr& e, int value) { return e->renderQueue < value; });
	
	// デプスシャドウマップを作成
	CreateShadowMap(gameObjects.begin(), transparentBegin);
	
	  // 描画先をデフォルトフレームバッファに戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, fbWidth, fbHeight);
	glUseProgram(prog3D);

	//RenderQueue_overlayにUIが設定されているため
	//overlay以降をアンリットシェーダに切り替える
	//overlayレンダーキューでの3Dモデル描画をしないように
	glProgramUniform1f(prog3D, locAlphaCutoff, 0.5f);

	// transparent以前のキューを描画
	DrawGameObject(prog3D,gameObjects.begin(), transparentBegin);
	//シェーダーでのカットオフをする
	glProgramUniform1f(prog3D, locAlphaCutoff, 0);
	//スカイスフィアの描画
	DrawSkySphere();

	// transparentからoverlayまでのキューを描画
	//深度バッファへの書き込みを禁止、または許可するには
	// glDepthMask(ジーエル・デプス・マスク)関数を使います
	//
	glDepthMask(GL_FALSE); // 深度バッファへの書き込みを禁止
	DrawGameObject(prog3D, transparentBegin, overlayBegin);
	glDepthMask(GL_TRUE); // 深度バッファへの書き込みを許可

	  // overlay以降のキューを描画
	glDisable(GL_DEPTH_TEST); // 深度テストを無効化
	glUseProgram(progUnlit);
	DrawGameObject(progUnlit,overlayBegin, gameObjects.end());
	
	//深度バッファがどのようなものかを確認したら、
	//上のプログラムの#if 1を#if 0に変更して、
	//シャドウマップのチェックプログラムを無効化してください。
	#if 0
	  // シャドウマップのチェック用
	{
	glBindVertexArray(*meshBuffer->GetVAO());
	MaterialList materials(1, std::make_shared<Material>());
	materials[0]->texBaseColor = fboShadow->GetDepthTexture();
	mat4 m = GetTransformMatrix(vec3(1), vec3(0), vec3(0, 1.5f, -4));
	glProgramUniformMatrix4fv(progUnlit, locTransformMatrix, 1, GL_FALSE, &m[0].x);
	Draw(*meshBuffer->GetStaticMesh("plane_xy"), progUnlit, materials);
	glBindVertexArray(0);
	}
	 #endif
	glfwSwapBuffers(window);//表示と描画を繰り返す
	glfwPollEvents();		//マウスやキーボード状態を教えてくれる


}

/**
* ゲームオブジェクト配列を描画する
* @param prog  描画に使うプログラムパイプラインオブジェクト 
* @param begin 描画するゲームオブジェクト範囲の先頭
* @param end   描画するゲームオブジェクト範囲の終端
*/
void Engine::DrawGameObject(
	GLuint prog,
	GameObjectList::iterator begin,
	GameObjectList::iterator end)
{
	glBindVertexArray(*meshBuffer->GetVAO());

	for (GameObjectList::iterator i = begin; i != end; ++i)
	{
		const auto& e = *i;		// 図形番号がリストにない場合は描画しない
		// スタティックメッシュが設定されていない場合は描画しない
		if (!e->staticMesh && !e->skeletalMesh) {
			continue;
		}
		// ユニフォーム変数にデータをコピー
		glProgramUniform4fv(prog, 100, 1, &e->color.x);
		
		// 座標変換ベクトルの配列をGPUメモリにコピー
		glProgramUniformMatrix4fv(prog, 0, 1, GL_FALSE, &e->GetTransformMatrix()[0].x);
		if (prog != progUnlit) {
			glProgramUniformMatrix3fv(prog, 1, 1, GL_FALSE, &e->GetNormalMatrix()[0].x);
		}
		//2つの変換ベクトルの配列をGPUメモリにコピーするだけで、
		//親と子が何階層あろうとも正しく機能するようになった

		if (e->staticMesh) {
			// 図形を描画::glDrawElementsInstanced(GL_TRIANGLES, indexData, GL_UNSIGNED_SHORT, indexDataの0番目から（0だと）, 1);
			if (e->materials.empty())
			{
				Draw(*e->staticMesh, prog, e->staticMesh->materials);

			}
			else
			{
				Draw(*e->staticMesh, prog, e->materials);

			}
		}
		
		    // スケルタルメッシュを描画
		if (e->skeletalMesh) {
			// 仮の関節行列をGPUメモリにコピー
			std::vector<mat4> jointMatrices(4);
			jointMatrices[0] = GetTransformMatrix(vec3(1), { 0.0f, 0, 0 }, vec3(0));
			jointMatrices[1] = GetTransformMatrix(vec3(1), { 0.2f, 0, 0 }, vec3(0));
			jointMatrices[2] = GetTransformMatrix(vec3(1), { -0.2f, 0, 0 }, vec3(0));
			jointMatrices[3] = GetTransformMatrix(vec3(1), { 0.0f, 0, 0 }, vec3(0));
			ssboJointMatrices->CopyData(0, jointMatrices.data(), sizeof(mat4) * 4);

			// スケルタルメッシュ用のシェーダとVAOを割り当てる
			glUseProgram(progSkeletal);
			glBindVertexArray(*meshBuffer->GetVAOSkeletal());

			// SSBOをバインド
			//GPU側のシェーダーストレージブロックに割り当て
			glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
				*ssboJointMatrices, 0, sizeof(mat4) * 4);

			// 座標変換行列をGPUメモリにコピー
			glProgramUniformMatrix4fv(progSkeletal, locTransformMatrix,
				1, GL_FALSE, &e->GetTransformMatrix()[0].x);

			// 固有マテリアルがあれば固有マテリアルを、なければ共有マテリアルを使って描画
			const MaterialList* materials = &e->skeletalMesh->materials;
			if (!e->materials.empty()) {
				materials = &e->materials;

			}
			Draw(e->skeletalMesh->drawParamsList, progSkeletal, *materials, &e->color);

			// 元のシェーダとVAOに戻す
			glBindVertexArray(*meshBuffer->GetVAO());
			glUseProgram(prog);

		} // if e->skeletalMesh
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(0);
}

/**
* ゲームオブジェクトの状態を更新する
*
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void Engine::UpdateGameObject(float deltaTime)
{
	// 要素の追加に対応するため添字を選択
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		GameObject* e = gameObjects[i].get();
		if (!e->IsDestroyed())
		{
			e->Start();
			e->Update(deltaTime);

		}

	}
	
	//親子階層ができたため、親が動くと子供も動くようになった
	//それにより、オブジェクトごとに全ての座標変換行列を計算していたため、非効率的
	//改善案として「自分の座標変換行列を先に計算しておく」
	//そして、「親の座標変換行列を必要になるたびに計算しておく」という無駄を省く

	  // ローカル座標変換行列を計算
	for (int i = 0; i < gameObjects.size(); i++)
	{
		//// ローカル座標変換行列の作成
		GameObject* e = gameObjects[i].get();
		e->transformMatrix = GetTransformMatrix(e->scale, e->rotation, e->position);
		e->normalMatrix = GetRotationMatrix(e->rotation);

	}
	
	  // ワールド座標変換行列を計算
	std::vector<mat4> worldTransforms(gameObjects.size());
	std::vector<mat3> worldNormals(gameObjects.size());
	for (int i = 0; i < gameObjects.size(); ++i) {
		//先に計算したローカル座標変換行列を使っている
		GameObject* e = gameObjects[i].get();
		//計算されたワールド座標変換行列をメンバ変数に代入していない
		//計算の最中にローカル座標変換行列上書きするのはをNG
		//同じ親を持つ他のゲームオブジェクトの座標変換行列を
		//計算できなくなるためでもある
		mat4 m = e->transformMatrix;
		mat3 n = e->normalMatrix;
		for (e = e->parent; e; e = e->parent) {
			m = e->transformMatrix * m;
			n = e->normalMatrix * n;

		}
		worldTransforms[i] = m;
		worldNormals[i] = n;

	}
	
	  // ワールド座標変換行列をゲームオブジェクトに設定
	for (int i = 0; i < gameObjects.size(); ++i) {
		gameObjects[i]->transformMatrix = worldTransforms[i];
		gameObjects[i]->normalMatrix = worldNormals[i];

	}
} // UpdateGameObject



