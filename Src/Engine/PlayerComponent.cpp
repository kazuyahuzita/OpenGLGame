#include "PlayerComponent.h"

void PlayerComponent::Awake()
{
	auto owner = GetOwner();
	auto engine = owner->GetEngine();

	//スポットライト
	//auto lightObject = engine->Create<GameObject>("light", "light");
	auto light = GetOwner()->AddComponent<Light>();
	light->color = { 0.95f, 1.0f, 0.98f };
	light->intensity = 15;
	light->radius = 20;
	light->type = Light::Type::spotLight;

	// コライダーを設定
	// 球体コライダーを上下に並べて置くことで、縦長の衝突形状を再現
	for (float i = 0; i < 2; ++i) {
		auto collider = owner->AddComponent<SphereCollider>();
		collider->sphere.radius = 0.5f;
		collider->sphere.position.y = -0.5f * i;

	}
	//重力の追加
	owner->AddComponent<CharacterMovement>();
	//滑る床に触れたときの処理
	owner->AddComponent<SlidingMovement>();

	const vec2 fbSize = engine->GetFramebufferSize();
	//被ダメージエフェクト（ファイアバージョン）を作りたい
	auto damageEffect = 
		engine->CreateUIObject<UILayout>("Res/Effect/Damage.tga", "Damage", { 0,0 }, 1);

	DamageEffectObject = damageEffect.first;
	// 画面全体を覆うサイズに設定
	DamageEffectObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	//始めに設定しておかないと画面にちらつきが起きるので定義
	DamageEffectObject->color.x = 1;
	DamageEffectObject->color.y = 0;
	DamageEffectObject->color.z = 0;
	DamageEffectObject->color.w = 0;

}

void PlayerComponent::Update(float deltaTime)
{
	Engine* engine = GetOwner()->GetEngine();
	GameObject& camera = *GetOwner();
	double& previousTime = engine->GetPreviousTime();

	// deadかgoal,stop,driveの状態なら反応しない
	if (state == PlayerState::dead || state == PlayerState::goal || 
		state == PlayerState::stop || state == PlayerState::drive)
		return;
	

	//カメラの回転をスムーズにするために配置
	const float cameraCosY = cos(camera.rotation.y);
	const float cameraSinY = sin(camera.rotation.y);
	/*
	* ゲームコントローラーでの操作
	*/
	if (glfwGetGamepadState(0, &gamepadstate))
	{
		//ジャンプの判定
		if (gamepadstate.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS)
		{
			camera.position.y += JumpPower * deltaTime;
			if (!isJump)
			{
				EasyAudio::PlayOneShot(SE::Jump);
				JumpTimer = 2.0f;
				isJump = true;
			}


		}
		//ダッシュの判定
		if (gamepadstate.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS)
		{
			addDashPower = 2.0f;

		}
		else {
			addDashPower = 1.0f;
		}

		//移動
		//横
		camera.position.x +=
			gamepadstate.axes[0] * cameraSpeed * addDashPower * cameraMoveSpeed * cameraCosY * deltaTime;
		camera.position.z +=
			gamepadstate.axes[0] * cameraSpeed * addDashPower * cameraMoveSpeed * -cameraSinY * deltaTime;
		//縦
		camera.position.x +=
			gamepadstate.axes[1] * cameraSpeed * addDashPower * cameraMoveSpeed * cameraSinY * deltaTime;
		camera.position.z +=
			gamepadstate.axes[1] * cameraSpeed * addDashPower * cameraMoveSpeed * cameraCosY * deltaTime;

		//向き
		camera.rotation.y -= gamepadstate.axes[2] * cameraSpeed * deltaTime;
		//縦
		// キャラクターのY座標を制限する（画面上部と下部にキャラクターが出ないようにする）
		if (camera.rotation.x < -0.6f) {
			camera.rotation.x = -0.6f;
		}
		else if (camera.rotation.x > 0.6f) {
			camera.rotation.x = 0.6f;
		}
		camera.rotation.x -= gamepadstate.axes[3] * cameraSpeed * deltaTime;

	}

	/*
	* キーボードでの操作
	*/

	//走る
	if (engine->GetKey(GLFW_KEY_LEFT_SHIFT)) {
		addDashPower = 2.0f;

	}
	else {
		addDashPower = 1.0f;
	}
	//飛ぶ
	//上下の移動
	if (engine->GetKey(GLFW_KEY_SPACE)) {
		camera.position.y += JumpPower * deltaTime;
		if (!isJump)
		{
			EasyAudio::PlayOneShot(SE::Jump);
			JumpTimer = 2.0f;
			isJump = true;
		}
	}
	//カメラのみの視点移動
	// カメラのY軸移動
	//回転

	//右に回転する
	if (engine->GetKey(GLFW_KEY_RIGHT)) {

		camera.rotation.y -= cameraSpeed * deltaTime;
	}
	//左に回転する
	if (engine->GetKey(GLFW_KEY_LEFT)) {

		camera.rotation.y += cameraSpeed * deltaTime;
	}
	//上に回転する
	if (engine->GetKey(GLFW_KEY_UP)) {
		//上限
		if (camera.rotation.x > 0.3f) camera.rotation.x = 0.3f;

		camera.rotation.x += cameraSpeed * deltaTime;
	}
	//下に回転する
	if (engine->GetKey(GLFW_KEY_DOWN)) {
		//下限
		if (camera.rotation.x < -0.1f) camera.rotation.x = -0.1f;

		camera.rotation.x -= cameraSpeed * deltaTime;
	}

	// ========= 一人称視点での移動 ============= //
	  // カメラのY軸移動
	if (engine->GetKey(GLFW_KEY_S)) {
		camera.position.x += cameraSpeed * cameraMoveSpeed * addDashPower * cameraSinY * deltaTime;
		camera.position.z += cameraSpeed * cameraMoveSpeed * addDashPower * cameraCosY * deltaTime;
	}//if GLFW_KEY_S(後移動)

	if (engine->GetKey(GLFW_KEY_W)) {
		camera.position.x -= cameraSpeed * cameraMoveSpeed * addDashPower * cameraSinY * deltaTime;
		camera.position.z -= cameraSpeed * cameraMoveSpeed * addDashPower * cameraCosY * deltaTime;
	}//if GLFW_KEY_W(前移動)

	// カメラのX軸移動
	if (engine->GetKey(GLFW_KEY_A)) {
		camera.position.x -= cameraSpeed * cameraMoveSpeed * addDashPower * cameraCosY * deltaTime;
		camera.position.z -= cameraSpeed * cameraMoveSpeed * addDashPower * -cameraSinY * deltaTime;
	}// if GLFW_KEY_A(左移動)

	if (engine->GetKey(GLFW_KEY_D)) {
		camera.position.x += cameraSpeed * cameraMoveSpeed * addDashPower * cameraCosY * deltaTime;
		camera.position.z += cameraSpeed * cameraMoveSpeed * addDashPower * -cameraSinY * deltaTime;
	}// if GLFW_KEY_D(右移動)

	//ゲーム終了
	if (engine->GetKey(GLFW_KEY_ESCAPE))
	{
		if (!isESC)isESC = true;
	}
	if (isESC)
	{
		glfwTerminate();
	}// if isESC

	//ファイアエフェクトに当たったら食らってるようにする
	if (isFire)
	{
		DamageEffectObject->color.w += 0.1f * deltaTime;

	}
	else
	{
		if (DamageEffectObject->color.w >= 0.0f)
			DamageEffectObject->color.w -= 0.2f * deltaTime;
		else
			DamageEffectObject->color.w = 0.0f;

	}// if isFire

	//滑る床に触れたかどうかを判断してくれる
	if (isPlayerSlide)
	{
		GetOwner()->isSlided = true;

	}
	else
	{
		GetOwner()->isSlided = false;
	} // if isPlayerSlide

	//ジャンプの際に音が重ならないようにする
	if (JumpTimer > 0) {
		// タイマーに合わせて徐々にフェードアウトさせる
		JumpTimer -= deltaTime * 2;
		// タイマーが0以下になったらメインゲームシーンに切り替える//ExplorationScene
		if (JumpTimer <= 0.0f) {
			isJump = false;
		}

	} // if JumpTimer

}

void PlayerComponent::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	// deadかgoal,stopの状態は衝突に反応しない
	if (state == PlayerState::dead || state == PlayerState::goal || 
		state == PlayerState::stop) 
		return;
	
	//nameに応じて処理を分けている
	const auto& name = other->GetOwner()->name;
	if (name == "goal") 
	{
		// goalのオブジェクトと衝突したらgoal状態にする
		state = PlayerState::goal;
	}
	else if (name == "athletic")
	{
		//athleticシーンに遷移させるときにプレイヤーの状態を変える
		state = PlayerState::athletic;
	}
	else if (name == "Item")
	{
		//アイテムオブジェクトに触れたらポイントが増える
		ItemPoint++;
	}
	else if (name == "jumpingPlat")
	{
		//飛ぶ際に移動されると埋まるorオブジェクトを貫通してしまうかもしれない
		//ので、いったん止めてから動けるようにしている
		state = PlayerState::jumpinghit;
	}
	else if (name == "KnockbackEnemy")
	{
		//プレイヤーを吹っ飛ばす感じになる
		state = PlayerState::nolmalhit;
	}
	else if (name == "SlidingFloor")
	{
		//触れたらプレイヤーを滑らせる
		isPlayerSlide = true;
	}
	else if (name != "SlidingFloor")
	{
		//離れたらプレイヤーを滑らせるのを止める

		isPlayerSlide = false;
	}
	else if (name == "motor")
	{
		//モーターオブジェクトに触れたら状態を変える
		state = PlayerState::drive;
	}
	else if (name == "fire")
	{
		//ダメージエフェクトのフラグをOnに
		isFire = true;
	}
	else if (name != "fire")
	{
		//ダメージエフェクトのフラグをOffに
		isFire = false;
	}

	//タグによって処理を分けている
	const auto& tag = other->GetOwner()->tag;

	if (tag == "Gas_Can")
	{
		//特別なオブジェクトの為のフラグをOnにしている
		isKeyItem = true;
	}
	if (tag == "Battery")
	{
		//バッテリー用のフラグをOnにしている
		isBattery = true;
	}
	if (tag == "Seat")
	{
		//シート用のフラグをOnにしている
		isSeat = true;
	}
	if (tag == "Tyre")
	{
		//タイヤ用のフラグをOnにしている
		isTyre = true;
	}
	else if (tag == "enemy")
	{
		// タグがenemyのオブジェクトと衝突したらdead状態にする
		state = PlayerState::dead;
	}
	else if (tag == "goal")
	{
		//ゴール用のタグも予め設置しておく
		state = PlayerState::goal;
	}

}

void PlayerComponent::Jumping(float deltaTime)
{
	GameObject& camera = *GetOwner();
	auto owner = GetOwner();
	//まっすぐ上に飛ばしている
	camera.position.y += 2.0f*deltaTime;
}

void PlayerComponent::Knockback(float deltaTime)
{
	GameObject& camera = *GetOwner();
	auto owner = GetOwner();
	const vec3 front = { cos(owner->rotation.y), 0, -sin(owner->rotation.y) };
	constexpr float powner = 0.01f;

	//プレイヤーを後ろに吹っ飛ばす形にする
	camera.position.z = -front.z +(sin(powner)* 0.01f) * deltaTime;
	camera.position.y +=  1.2f * deltaTime;

}

