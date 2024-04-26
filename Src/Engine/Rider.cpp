#include "Rider.h"

void Rider::Awake()
{
	auto owner = GetOwner();
	auto engine = owner->GetEngine();

	// コライダーを設定
	// 球体コライダーを上下に並べて置くことで、縦長の衝突形状を再現
	for (float i = 0; i < 2; ++i) {
		auto collider = owner->AddComponent<SphereCollider>();
		collider->sphere.radius = 0.5f;
		collider->sphere.position.z = -0.5f;
		collider->sphere.position.y = -0.5f * i;
	}

	// バイクを追加
	auto bike = engine->Create<GameObject>("bike", "bike",{ 0.0f, -3.6f, -0.5f });
	bike->rotation.x = radians(10);
	bike->rotation.y = radians(180);
	bike->scale = vec3{ 2,2,2 };
	bike->staticMesh = engine->GetStaticMesh("Res/MeshData/DeathForest/Motor/Motor.obj");
	bike->SetParent(owner);

	//ダッシュエフェクトを作りたい
	auto dashEffect = engine->CreateUIObject<UILayout>("Res/Effect/DashEffect.tga","Dash", {0,0}, 1);
	auto dashEffect1 = engine->CreateUIObject<UILayout>("Res/Effect/DashEffect.tga","Dash", {0,0}, 1);

	DashEffectObject = dashEffect.first;
	// 画面全体を覆うサイズに設定
	const vec2 fbSize = engine->GetFramebufferSize();
	DashEffectObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	DashEffectObject->color.w = 0;

	DashEffectObject1 = dashEffect1.first;
	// 画面全体を覆うサイズに設定
	const vec2 fbSize1 = engine->GetFramebufferSize();
	DashEffectObject1->scale = { fbSize1.x / fbSize1.y, 1, 1 };
	DashEffectObject1->color.w = 0;

}

void Rider::Update(float deltaTime)
{
	auto owner = GetOwner();
	Engine* engine = GetOwner()->GetEngine();
	GameObject& camera = *GetOwner();

	double& previousTime = engine->GetPreviousTime();

	// drive以外の状態は移動できない
	if (riderState != RiderState::drive)return;

	// 動作制御用タイマーを更新
	timer += deltaTime;
	const float cameraCosY = cos(camera.rotation.y);
	const float cameraSinY = sin(camera.rotation.y);
	//常に進んでいるようにする
	camera.position.z -= 0.1f;

	//マクロ定義は、数字に意味を持たせているのと同義
	//めんどくさいときは、数字でもいい

	/*
	* ゲームコントローラーでの操作
	*/

	if (glfwGetGamepadState(0, &gamepadstate))
	{

		//移動
		//横の移動
		camera.position.x += gamepadstate.axes[0] * cameraSpeed  * cameraCosY * deltaTime;
		camera.position.z += gamepadstate.axes[0] * cameraSpeed  * -cameraSinY * deltaTime;
		//縦
		camera.position.x += gamepadstate.axes[1] * cameraSpeed  * cameraSinY  * deltaTime;
		camera.position.z += gamepadstate.axes[1] * cameraSpeed  * cameraCosY  * deltaTime;

		//速度が上がっているのを分かりやすくする
		if (gamepadstate.axes[1] <  -0.5f)
		{
			DashEffectObject1->color.w = 1;

		}
		else {

			DashEffectObject1->color.w = 0;
		}
		

		//向き
		//横
		// キャラクターのX座標を制限する（画面上部と下部にキャラクターが出ないようにする）
		if (camera.rotation.y < -0.4f) {
			camera.rotation.y = -0.4f;
		}
		else if (camera.rotation.y > 0.8f) {
			camera.rotation.y = 0.8f;
		}

		//バイクの右向き
		if (gamepadstate.axes[2] > 0.2f)
		{
			owner->GetChild(0)->rotation.y -= bikerotate*deltaTime ;
		}
		//バイクの左向き
		else if (gamepadstate.axes[2] < -0.2f)
		{
			owner->GetChild(0)->rotation.y += bikerotate*deltaTime;
		}
		else 
		{
			//3.1415926535
			owner->GetChild(0)->rotation.y = radians(180)/** cameraCosY*/;
		}

		camera.rotation.y -= gamepadstate.axes[2] * cameraSpeed * deltaTime;

		//縦
		// キャラクターのY座標を制限する（画面上部と下部にキャラクターが出ないようにする）
		if (camera.rotation.x < -0.1f) {
			camera.rotation.x = -0.1f;
		}
		else if (camera.rotation.x > 0.2f) {
			camera.rotation.x = 0.2f;
		}
		camera.rotation.x -= gamepadstate.axes[3] * cameraSpeed * deltaTime;
	}

	/*
	* キーボードでの操作
	*/

	//カメラのみの視点移動
	// カメラのY軸移動
	//回転
	
	//右に向ける
	if (engine->GetKey(GLFW_KEY_RIGHT)) {
		//下限
		if (camera.rotation.y < -0.2f)
			camera.rotation.y = -0.2f;

		camera.rotation.y -= cameraSpeed * deltaTime;
	}
	//左に向ける
	if (engine->GetKey(GLFW_KEY_LEFT)) {
		//上限
		if (camera.rotation.y > 0.6f)
			camera.rotation.y = 0.6f;
		owner->GetChild(0)->rotation.y += bikerotate * deltaTime;

		camera.rotation.y += cameraSpeed * deltaTime;
	}
	//上に向ける
	if (engine->GetKey(GLFW_KEY_UP)) {
		//上限
		if (camera.rotation.x > 0.1f) camera.rotation.x = 0.2f;

		camera.rotation.x += cameraSpeed * deltaTime;
	}
	//下に向ける
	if (engine->GetKey(GLFW_KEY_DOWN)) {
		//下限
		if (camera.rotation.x < -0.08f) camera.rotation.x = -0.08f;

		camera.rotation.x -= cameraSpeed * deltaTime;
	}

	// ========= 一人称視点での移動 ============= //
	// カメラのY軸移動
	  // カメラの後ろ移動
	if (engine->GetKey(GLFW_KEY_S)) {
		camera.position.x += cameraSpeed  * cameraSinY * deltaTime;
		camera.position.z += cameraSpeed  * cameraCosY * deltaTime;

	}
	//カメラの前移動
	if (engine->GetKey(GLFW_KEY_W)) {
		camera.position.x -= cameraSpeed  * cameraSinY * deltaTime;
		camera.position.z -= cameraSpeed  * cameraCosY * deltaTime;
		DashEffectObject->color.w = 1;

	}
	else {

		DashEffectObject->color.w = 0;
	}

	
	// カメラのX軸移動			
	// カメラの左移動		 
	if (engine->GetKey(GLFW_KEY_A)) {
		camera.position.x -= cameraSpeed  * cameraCosY * deltaTime;
		camera.position.z -= cameraSpeed  * -cameraSinY * deltaTime;
	}
	//カメラの右移動
	if (engine->GetKey(GLFW_KEY_D)) {
		camera.position.x += cameraSpeed  * cameraCosY * deltaTime;
		camera.position.z += cameraSpeed  * -cameraSinY * deltaTime;
	}
	//ゲームの強制終了
	if (engine->GetKey(GLFW_KEY_ESCAPE))
	{
		if (!isESC)isESC = true;
	}

}

void Rider::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	if (name == "stop")
	{
		//Stopオブジェクトに触れたら止まるように
		riderState = RiderState::stop;
	}
	else if (name == "goal") {
		//ゴールオブジェクトに触れたらゴールシーンに遷移させるように
		riderState = RiderState::goal;

	}
	else
	{
		//なにか他のものに触れたらゲームオーバーになるように
		riderState = RiderState::dead;
	}

}
