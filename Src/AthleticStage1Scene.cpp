#include "AthleticStage1Scene.h"
//他のオブジェクトヘッダーから
#include "AthleticStage2Scene.h"
#include "AudioSettings.h"
#include "ExplorationScene.h"
#include "FirstPersonCamera.h"
#include "GameObjectSettingsJSON.h"
#include "Gate.h"
#include "GateEffect.h"
#include "ImageSettings.h"
#include "MeasureTime.h"
#include "MoveExplorationScene.h"
#include "MoveStage2Scene.h"
#include "TextBlock.h"
#include "Tornado.h"
//Engine経由の機能に関するヘッダーから
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/PlayerComponent.h"
#include "Engine/UIButton.h"


/**
* シーンの初期化
*/
bool AthleticStage1Scene::Initialize(Engine& engine)
{
	//環境光の設定：全体的に明るくしてある
	engine.ambientLight = vec3(0.50f, 0.50f, 0.50f);

	// ゲームオブジェクト配置ファイルを読み込む
	auto jsonobject = engine.Create<GameObject>("object", "object", {0,0,0});
	gameObjectSettingsjson = jsonobject->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->AthleticStage1MapLoad();
	// プレイヤーを配置
	auto player = engine.Create<GameObject>("player", "player", { -1.5f, 3.0f, -5.5f },{0,0.0f,0});
	player->rotation.y = -radians(90);
	playerComponent = player->AddComponent<PlayerComponent>();
	player->AddComponent<FirstPersonCamera>();

	//時間制限を設けるためのオブジェクト
	auto measureTime =
		engine.Create<GameObject>("timer", "timer", { 0.0f, 0.0f, 0.0f });
	measureTimeComponent = measureTime->AddComponent<MeasureTime>();
	//時間制限の設定をステージごとに決めている
	measureTimeComponent->SetState(MeasureTime::MeasureTimeState::alive);
	measureTimeComponent->SetCountTime(1);
	measureTimeComponent->SetCountTime1(3);

	//アスレチックシーンに遷移用のオブジェクト
	auto meScene =
		engine.Create<GameObject>("exploration", "exploration", { -5.9f, 5.6f,4.0f });
	moveExplorationScene = meScene->AddComponent<MoveExplorationScene>();
	//アスレチックシーンに遷移用のオブジェクト
	auto ms2Scene =
		engine.Create<GameObject>("Stage2", "Stage2", { -5.9f, 5.6f,7.0f });
	moveStage2Scene = ms2Scene->AddComponent<MoveStage2Scene>();

	//ゴールの直前に設置しているゴールっぽく演出しているオブジェクト
	auto tornado =
		engine.Create<GameObject>("tornado", "tornado", { -5.6f, 5.6f,4.0f });
	tornadoComponent = tornado->AddComponent<TornadoEffect>();
	auto tornado1 =
		engine.Create<GameObject>("tornado", "tornado", { -5.6f, 5.6f,7.0f });
	tornadoComponent1 = tornado1->AddComponent<TornadoEffect>();

	// ネクストシーンの説明の表示
	auto textBlock =
		engine.Create<GameObject>("next", "next", { -5.2f, 5.6f,7.0f });
	textBlockComponent = textBlock->AddComponent<TextBlock>();

	// シーンを戻るときの説明の表示
	auto textBlock1 =
		engine.Create<GameObject>("back", "back", { -5.2f, 5.6f,4.0f });
	textBlockComponent1 = textBlock1->AddComponent<TextBlock>();

	// スカイスフィアを設定
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::skysphere);

	// BGMを再生
	EasyAudio::Play(AudioPlayer::bgm, BGM_Athletic::playScene, engine.MusicVolume - 0.2f, true);

	return true;
}

void AthleticStage1Scene::Update(Engine& engine, float deltaTime)
{

	//case文で分けるとごちゃごちゃしてしまうため、分けて書く
	// 状態に対応する更新関数を実行
	state(this, engine, deltaTime);

	if (fadeTimer > 0) {
		// タイマーに合わせて徐々にフェードアウトさせる
		fadeTimer -= 0.5f*deltaTime;
		//パッと白くなって、少し待ってからシーンに移動する形に
		FadeObject->color.w += 1.0f*deltaTime;
		 
		// タイマーが0以下になったらゲームシーンに切り替える
		//boolでどのシーンに行くかはわかれている.
		if (fadeTimer <= 0.0f && isExploration)
		{
			engine.SetNextScene<ExplorationScene>();

		}
		else if (fadeTimer <= 0.0f && isAthleticStage1)
		{
			engine.SetNextScene<AthleticStage1Scene>();

		}
		else if (fadeTimer <= 0.0f && isAthleticStage2)
		{
			engine.SetNextScene<AthleticStage2Scene>();

		}
		else if (fadeTimer <= 0.0f && isTimeOut) {
			engine.SetNextScene<AthleticStage1Scene>();
		}

	} // if fadeTimer
}

void AthleticStage1Scene::Finalize(Engine& engine)
{
	//ここで生成したオブジェクトの削除
	//音声のストップをする、それぞれでプレイヤー番号が違うのでそれぞれをしっかり指定する
	EasyAudio::Stop(AudioPlayer::bgm);
	EasyAudio::Stop(AudioPlayer::se);
	EasyAudio::Stop(AudioPlayer::se1);
	engine.ClearGameObjectAll();
}

void AthleticStage1Scene::State_Playing(Engine& engine, float deltaTime)
{
	// 初期設定	 
	auto measureTime = measureTimeComponent->GetOwner();
	auto meScene = moveExplorationScene->GetOwner();
	auto ms2Scene = moveStage2Scene->GetOwner();
	auto player = playerComponent->GetOwner();
	auto tornado = tornadoComponent->GetOwner();
	auto tornado1 = tornadoComponent1->GetOwner();
	auto textBlock = textBlockComponent->GetOwner();
	auto textBlock1 = textBlockComponent1->GetOwner();

	//ゴールオブジェクトとして白い粒子を飛ばしている
	tornadoComponent->SetState(TornadoEffect::TornadoEffectState::alive);
	tornadoComponent1->SetState(TornadoEffect::TornadoEffectState::alive);

	//AthleticScene2に移動するときに画像をプレイヤーが近づいたら出してくれるようにしている
	if (textBlockComponent->IsPlayerInRange(player->position.x, player->position.z, 1.5f))
		textBlockComponent->SetIsHitPlayer4(true);
	else
		textBlockComponent->SetIsHitPlayer4(false);

	//ExplorationSceneに戻るときの画像をプレイヤーが近づいたら出してくれるようにしている
	if (textBlockComponent1->IsPlayerInRange(player->position.x, player->position.z, 1.5f))
		textBlockComponent1->SetIsHitPlayer5(true);
	else
		textBlockComponent1->SetIsHitPlayer5(false);

	//プレイヤーのリスタート
	if (playerComponent->GetPosition().y <= -2.0f)
	{

		if (fadeTimer <= 0)
		{
			//playerの動きをさせなくしている
			playerComponent->SetState(PlayerComponent::PlayerState::stop);
			//制限時間を止めている
			measureTimeComponent->SetState(MeasureTime::MeasureTimeState::dead);
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
			retryButton.second->onClick.push_back(
				[this](UIButton* button) {
					fadeTimer = 1;				  //シーン遷移の処理を始める
					isAthleticStage1 = true;      //もう一度始める際のフラグをOnに
					button->interactable = false; // ボタンを無効化
					EasyAudio::PlayOneShot(SE::buttonClick);
				});

			//バックボタンを設定している
			auto backButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::back_button, "Title", { 0.0f, -0.7f }, 0.12f);
			backButton.second->onClick.push_back(
				[this](UIButton* button)
				{
					fadeTimer = 1;				  //シーン遷移の処理を始める
					isExploration = true;		  //ExplorationSceneに飛ばすフラグをOnに
					button->interactable = false; // ボタンを無効化
					EasyAudio::PlayOneShot(SE::buttonClick);

				});

		}//if fadeTimer
	}//	if playerComponent->GetPosition().y <= -2.0f


	//制限時間を超えたときにゲームオーバー処理をさせる
	if (measureTimeComponent->GetIsCount())
	{
		//プレイヤーをアスレチックシーンに遷移させる
		if (fadeTimer <= 0)
		{
			fadeTimer = 1.0f;	//シーン遷移の処理を始める
			isTimeOut = true;	//リトライするフラグをOnに
			// フェードアウト用UIオブジェクト
			// 画面全体を覆うサイズに設定
			// カラーを「黒、透明」に設定
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 0, 0.0f);
		}//if fadeTimer

	}//	if measureTimeComponent->GetIsCount()
	/*
	* ゴール用の遷移フラグ
	*/

	//ExplorationSceneに戻る際のif文
	if (moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside)
	{
		//プレイヤーをアスレチックシーンに遷移させる
		if (fadeTimer <= 0) {
			fadeTimer = 1.0;		//シーン遷移の処理を始める
			isExploration = true;	//ExplorationSceneに飛ばすフラグをOnに
			// フェードアウト用UIオブジェクト
			// 画面全体を覆うサイズに設定
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;

		}//if fadeTimer
	}//	if moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside

	//Stage2に進む時のif文
	if (moveStage2Scene->GetState() == MoveStage2Scene::MoveStage2SceneState::Inside)
	{
		//プレイヤーをアスレチックシーンに遷移させる
		if (fadeTimer <= 0) {
			fadeTimer = 1.0;			//シーン遷移の処理を始める
			isAthleticStage2 = true;	//AthleticStage2に飛ばすフラグをOnに
			// フェードアウト用UIオブジェクト
			// 画面全体を覆うサイズに設定
			// カラーを「黒、透明」に設定
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;
		}
	}//	if moveStage2Scene->GetState() == MoveStage2Scene::MoveStage2SceneState::Inside

}

void AthleticStage1Scene::State_GameOver(Engine& engine, float deltaTime)
{
	//先にプレイヤーを削除する
	auto player = playerComponent->GetOwner();
	player->Destroy();

}
