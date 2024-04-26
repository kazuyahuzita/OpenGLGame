#include "HorrorGameScene.h"
//他のオブジェクトヘッダーから
#include "AudioSettings.h"
#include "Bike.h"
#include "ExplorationScene.h"
#include "FirstPersonCamera.h"
#include "FluorescentLight.h"
#include "GameObjectSettingsJSON.h"
#include "HorrorDeathScene.h"
#include "HorrorItemUI.h"
#include "ImageSettings.h"
#include "Item.h"
#include "MoveEnemy.h"
#include "RiderScene.h"
#include "TitleScene.h"
//Engine経由の機能に関するヘッダーから
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/PlayerComponent.h"
#include "Engine/UIButton.h"

/**
* シーンの初期化
*/
bool HorrorGameScene::Initialize(Engine& engine)
{
	//環境光の設定：全体的に暗くくしてある
	engine.ambientLight = vec3(0.10f,0.10f,0.10f);

	// ゲームオブジェクト配置ファイルを読み込む
	auto jsonobject = engine.Create<GameObject>("object", "object", { 0,0,0 });
	gameObjectSettingsjson = jsonobject->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->SubMainGameSceneMapLoad();

	// プレイヤーを配置
	auto player = engine.Create<GameObject>("player","player", { 30, 3.0f, -30 }, {0,-90,0});
	playerComponent = player->AddComponent<PlayerComponent>();
	player->AddComponent<FirstPersonCamera>();
	//プレイヤーの視界を大きな球で制限
	auto PlayerSphere = 
		engine.Create<GameObject>("playerspehe", "playerspehe");
	PlayerSphere->scale = vec3{0.5f,0.5f,0.5f};
	PlayerSphere->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/PlayerSphere/PlayerSphere.obj");
	PlayerSphere->SetParent(player);

	// よしえの表示
	auto enemy = engine.Create<GameObject>("enemy", "enemy",{0,1.8f,0});
	enemyComponent = enemy->AddComponent<MoveEnemy>();
	enemy->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Face/Yoshie.obj");

	// でかい虫エネミーを表示
	auto enemy1 = engine.Create<GameObject>("enemy1", "enemy1",{0,-30.0f,0});
	enemyComponent1 = enemy1->AddComponent<MoveEnemy>();
	enemy1->scale = vec3{ 0.5f,0.5f,0.5f };
	enemy1->staticMesh = engine.GetStaticMesh("Res/MeshData/Spawned/SpawnerEnemy.obj");

	//バイクの配置
	auto bike = engine.Create<GameObject>("motor", "motor", { 0, 0.4f, -28 });
	BikeComponent = bike->AddComponent<Bike>();

	//アイテム用のUIの配置
	auto horrorItemUI = engine.Create<GameObject>("horrorItemUI", "horrorItemUI", { 0,0,0 });
	horrorItemUIComponent = horrorItemUI->AddComponent<HorrorItemUI>();

	//真ん中右にアイテムの配置
	auto battery = engine.Create<GameObject>("Item", "battery");
	battery->AddComponent<GemItem>();
	battery->position = { 45,6.5f,-30 };
	battery->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Item/Battery.obj");
	//右上にアイテムの配置
	auto Gas_Can = engine.Create<GameObject>("Item", "Gas_Can");
	Gas_Can->AddComponent<GemItem>();
	Gas_Can->position = { 35,6.4f,-65 };
	Gas_Can->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Item/Gas_Can.obj");
	//左上にアイテムの配置
	auto seat = engine.Create<GameObject>("Item", "seat");
	seat->AddComponent<GemItem>();
	seat->position = { 14,0.2f,-50 };
	seat->scale = { 2,2,2 };
	seat->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Item/Seat.obj");
	//左下にアイテムの配置
	auto tyre = engine.Create<GameObject>("Item", "tyre");
	tyre->AddComponent<GemItem>();
	tyre->position = { -4,1.0f,-13 };
	tyre->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Item/Tyre.obj");

	// スカイスフィアを設定
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::graysphere);

	// 画像を表示
	//目標という文字を表示している
	auto Target = engine.CreateUIObject<UILayout>(Image_Horror::target, "target", { -1.6f, 0.9f }, 0.07f);
	//目標の中身を書いている.
	auto Search = engine.CreateUIObject<UILayout>(Image_Horror::search, "Search", { -0.65f, 0.9f }, 0.07f);
	// BGMを再生
	EasyAudio::Play(AudioPlayer::bgm, BGM_Horror::playScene, engine.MusicVolume - 0.2f, true);

	return true;
}

/**
* シーンを更新する
*/
void HorrorGameScene::Update(Engine& engine, float deltaTime)
{
	//case文で分けるとごちゃごちゃしてしまうため、分けて書く
	// 状態に対応する更新関数を実行
	state(this, engine, deltaTime);

	if (fadeTimer > 0) {
		// タイマーに合わせて徐々にフェードアウトさせる
		fadeTimer -= deltaTime;
		FadeObject->color[3] = 1 - fadeTimer;

		// タイマーが0以下になったらゲームシーンに切り替える
		//boolでどのシーンに行くかはわかれている.
		if (fadeTimer <= 0.0f&&isRiderScene) {
			engine.SetNextScene<RiderScene>();

		}
		else if (fadeTimer <= 0.0f && isExploration)
		{
			engine.SetNextScene<ExplorationScene>();

		}
		else if (fadeTimer <= 0.0f && isHorrorGameScene)
		{
			engine.SetNextScene<HorrorGameScene>();

		}

	} // if fadeTimer

	//プレイヤーが敵に触れたら即座に死亡シーンに切り替えている
	if (playerComponent->GetState() == PlayerComponent::PlayerState::dead)
	{
		engine.SetNextScene<HorrorDeathScene>();
	}

	//やめるボタンを押したらゲームを終えられるようにしている
	if (isBack)
	{
		glfwTerminate();
	}
}

/**
* シーンを終了する
* 
*/
void HorrorGameScene::Finalize(Engine& engine)
{
	//ここで生成したオブジェクトの削除
	//音声のストップをする、それぞれでプレイヤー番号が違うのでそれぞれをしっかり指定する
	EasyAudio::Stop(AudioPlayer::bgm);
	EasyAudio::Stop(AudioPlayer::se);
	EasyAudio::Stop(AudioPlayer::se1);
	engine.ClearGameObjectAll();
}

/**
* 「プレイ中」状態のシーンを更新する
*
* @param engine    ゲームエンジン
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void HorrorGameScene::State_Playing(Engine& engine, float deltaTime)
{

	/*			 *
	 * 初期設定	 * 
	 *			 */	
	auto bike = BikeComponent->GetOwner();
	auto enemy = enemyComponent->GetOwner();
	auto enemy1 = enemyComponent1->GetOwner();
	auto horrorItemUI = horrorItemUIComponent->GetOwner();
	auto player = playerComponent->GetOwner();

	//ジャンプ音が世界観と合わないため
	//プレイヤーのジャンプ音を消す
	playerComponent->SetJump(true);

	//プレイヤーのリスタート
	if (playerComponent->GetPosition().y <= -2.0f)
	{

		if (fadeTimer <= 0)
		{
			//playerの動きをさせなくしている
			playerComponent->SetState(PlayerComponent::PlayerState::stop);
			// フェードアウト用UIオブジェクト
			// 画面全体を覆うサイズに設定
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;

			//ゲームオーバーのロゴを表示してくれる
			auto gameOverLogo =
				engine.CreateUIObject<UILayout>(Image_Athletic::over_logo, "Title", { 0, 0.4f }, 0.2f);
			gameOverLogo.second->color.w = 0;

			//リトライボタンを設定している
			auto retryButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::retry_button, "Title", { 0.0f, -0.4f }, 0.12f);
			// ボタンが押されたらメインゲームシーンに切り替えるラムダ式を設定
			retryButton.second->onClick.push_back(
				[this](UIButton* button) {
					fadeTimer = 1;
					isHorrorGameScene = true;
					button->interactable = false; // ボタンを無効化
					EasyAudio::PlayOneShot(SE::buttonClick);
				});

			//バックボタンを設定している
			auto backButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::back_button, "Title", { 0.0f, -0.7f }, 0.12f);
			backButton.second->onClick.push_back(
				[this](UIButton* button)
				{
					fadeTimer = 1;
					isExploration = true;
					button->interactable = false; // ボタンを無効化
					EasyAudio::PlayOneShot(SE::buttonClick);

				});

		}//if fadeTimer
	}//if playerComponent->GetPosition().y <= -2.0f

	//始めから一体追跡キャラを追加
	enemyComponent->Tracking(player->position,deltaTime);

	//プレイヤーとの距離が近くなったら音が鳴るようにしている
	if (enemyComponent->IsPlayerInRange(player->position.x, player->position.z, 12.0f)) 
	{
		//何度も同じ再生をしないようにしている
		if (EasyAudio::IsPlaying(AudioPlayer::se))
		{
			return;
		}
		//別の追跡SEが流れていたら流さない
		if (!isTrackingSE1)
		{
			// 追跡SEを再生
			EasyAudio::Play(AudioPlayer::se, SE::Stoke, engine.MusicVolume - 0.2f, true);
			isTrackingSE = true;
		}

	}
	else
	{
		isTrackingSE = false;
		EasyAudio::Stop(AudioPlayer::se);
	}//if enemyComponent->IsPlayerInRange(player->position.x, player->position.z, 12.0f)


	/*										 *
	 * プレイヤーの状況により変化する設定	 *
	 *										 */
	//対応するアイテムが取られたらその部分のアイテム画像のalpha値を1に
	if(playerComponent->GetSeatItem())horrorItemUIComponent->SetIsHitSeat(true);
	if(playerComponent->GetBatteryItem())horrorItemUIComponent->SetIsHitBattery(true);
	if(playerComponent->GetTyreItem())horrorItemUIComponent->SetIsHitTyre(true);
	//キーアイテムを取った時の対処法
	if (playerComponent->GetKeyItem())
	{
		horrorItemUIComponent->SetIsHitGasCan(true);
		//エネミー登場
		enemy1->position.y = 1.5f;
		//追跡エネミーを増やす
		enemyComponent1->Tracking(player->position, deltaTime);
	}//if playerComponent->GetKeyItem()

	//プレイヤーとの距離が近くなったら音が鳴るようにしている
	if (enemyComponent1->IsPlayerInRange(player->position.x, player->position.z, 12.0f))
	{
		if (EasyAudio::IsPlaying(AudioPlayer::se1))
		{
			return;
		}
		//別の追跡SEが流れていたら流さない
		if (!isTrackingSE)
		{
			isTrackingSE1 = true;
			EasyAudio::Play(AudioPlayer::se1, SE::Stoke, engine.MusicVolume - 0.2f, true);
		}

	}
	else
	{
		isTrackingSE1 = false;
		EasyAudio::Stop(AudioPlayer::se1);
	}//if enemyComponent1->IsPlayerInRange(player->position.x, player->position.z, 12.0f)

	////アイテムを一定数獲得したら、ゴールとする
	if (playerComponent->GetPoint() == 8)
	{
		if (!isDrive) {
		isDrive = true;

		}
	}

	/*			　 *
	 * シーン遷移　*
	 *			　 */
	// バイクにふれて乗る
	if (isDrive)
	{
		if (fadeTimer <= 0) {
			//エネミーを飛ばしてバグらせないように処理
			enemy->position = { 12,0,-25 };
			enemy1->position = { 24,0,-25 };
			fadeTimer = 1;			 //シーン遷移の処理を始める
			isRiderScene = true;	 //RiderSceneに遷移させるフラグをOnに
			// フェードアウト用UIオブジェクト
			// 画面全体を覆うサイズに設定
			// カラーを「黒、透明」に設定
			auto fade = engine.CreateUIObject<UILayout>("Res/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay+10;

			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 4, 0.0f);
		}//if fadeTimer
	}//if isDrive
	
	/*			　	　 *
	 * ゲーム終了処理　*
	 *			　 　　*/

	//escキーを押されたときの対処法
	//今のところやめることしかできない
	if (playerComponent->GetESC())
	{
		//プレイヤーが動かないように
		playerComponent->SetState(PlayerComponent::PlayerState::stop);

		// 警告画像を表示
		engine.CreateUIObject<UILayout>("Res/Image/BackWindow.tga", "Back", { 0, 0 }, 0.25f);

		// はいボタンを表示
		auto Yesbutton = engine.CreateUIObject<UIButton>(
			"Res/Image/Yes.tga", "Yes", { -0.5f, -0.5f }, 0.1f);
		Yesbutton.second->onClick.push_back([this](UIButton* button)
			{
				EasyAudio::PlayOneShot(SE::buttonClick);
				isBack = true;
			});
		// いいえボタンを表示
		auto Nobutton = engine.CreateUIObject<UIButton>(
			"Res/Image/No.tga", "No", { 0.5f,  -0.5f }, 0.1f);
		Nobutton.second->onClick.push_back([this](UIButton* button)
			{
				button->interactable = false; // ボタンを無効化
				EasyAudio::PlayOneShot(SE::missClick);

			});

	}//if playerComponent->GetESC()
}

/**
* 「ゲームオーバー」状態のシーンを更新する
*
* @param engine    ゲームエンジン
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void HorrorGameScene::State_GameOver(Engine& engine, float deltaTime)
{
	//先にplayerを削除しておく
	auto player = playerComponent->GetOwner();
	player->Destroy();

}

