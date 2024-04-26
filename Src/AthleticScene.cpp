#define _CRT_SECURE_NO_WARNINGS

#include "AthleticScene.h"
//他のオブジェクトヘッダーから
#include "AudioSettings.h"
#include "DartSmoke.h"
#include "ExplorationScene.h"
#include "FireEmitter.h"
#include "FirstPersonCamera.h"
#include "FrontBackBoard.h"
#include "GameObjectSettingsJSON.h"
#include "Gate.h"
#include "GateEffect.h"
#include "GemsUI.h"
#include "ImageSettings.h"
#include "Item.h"
#include "ItemBlock.h"
#include "MoveBlock.h"
#include "MoveEnemy.h"
#include "MoveExplorationScene.h"
#include "NormalEnemy.h"
#include "TitleScene.h"

//Engine経由の機能に関するヘッダーから
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/PlayerComponent.h"
#include "Engine/UIButton.h"


/**
* シーンの初期化
*/
bool AthleticScene::Initialize(Engine& engine)
{
	engine.ambientLight = vec3(0.50f, 0.50f, 0.50f);

	// Jsonファイルでゲームオブジェクト配置ファイルを読み込む
	auto jsonobject = engine.Create<GameObject>("jsonObject", "jsonObject", {0,0,0});
	gameObjectSettingsjson = jsonobject->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->AthleticSceneMapLoad();
		
	/*
	* 実際のプレイ場面で設置されるObjectの設置
	*/
	
	//ジェム用のUIの配置
	auto gemsUI = engine.Create<GameObject>("gemUI", "gemUI",{0,0,0});
	gemsUIComponent = gemsUI->AddComponent<GemsUI>();

	//=== ゲート演出の際に必要なオブジェクト達 ↓===//
	
	//土煙の表示
	auto dartEffect =
		engine.Create<GameObject>("dart", "dart", { 62.0f, 3.5f, -72.5f },{0,0,0});
	dartSmokeEffectComponent =dartEffect->AddComponent<DartSmokeEffect>();

	// ゲートの表示
	auto gate = 
		engine.Create<GameObject>("gate", "gate", { 62.0f, -0.3f, -72.5f },{0,30,0});
	gateComponent =gate->AddComponent<Gate>();

	//ゲートのエフェクトの表示
	auto gateEffect =
		engine.Create<GameObject>("gateEffect", "gateEffect", { 61.5f, 1.5f, -72.5f }, { 0,320,0 });
	gateEffectComponent= gateEffect->AddComponent<GateEffect>();

	//探索シーンに戻すオブジェクト
	auto meScene =
		engine.Create<GameObject>("exploration", "exploration", { 60.5f, 12.0f, -72.5f }, { 0,30,0 });
	moveExplorationScene = meScene->AddComponent<MoveExplorationScene>();

	//=== ゲート演出の際に必要なオブジェクト達 ↑===//

	//=== 追跡するエネミーの表示や行動に必要なオブジェクト達 ↓===//

	//アイテムブロックを叩くことで、エネミーが発生
	auto itemBlock = engine.Create<GameObject>("itemBlock", "itemBlock", { 4.0f, 3.0f, -4.5f });
	itemBlockComponent = itemBlock->AddComponent<ItemBlock>();

	// 追いかけてくるエネミーの表示
	auto trackingEnemy =
		engine.Create<GameObject>("trackingEnemy", "enemy", { 2.5f, -3.0f, -12.5f });
	trackingEnemyComponent = trackingEnemy->AddComponent<MoveEnemy>();
	trackingEnemy->scale = {1.5f,1.5f,1.5f};
	trackingEnemy->staticMesh = engine.GetStaticMesh("Res/MeshData/Athletic/Face/Face.obj");

	//=== ゲート演出の際に必要なオブジェクト達 ↑===//

	// そこらを歩くエネミーの表示
	auto normalEnemy = 
		engine.Create<GameObject>("KnockbackEnemy", "KnockbackEnemy", { 2.5f, 0.5f, -4.5f });
	normalEnemyComponent =normalEnemy->AddComponent<NormalEnemy>();

	// プレイヤーを配置
	auto player = engine.Create<GameObject>("player", "player", { 0.0f, 2.0f, 0.0f });
	playerComponent = player->AddComponent<PlayerComponent>();
	player->AddComponent<FirstPersonCamera>();

	// スカイスフィアを設定
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::skysphere);

	// BGMを再生
	EasyAudio::Play(AudioPlayer::bgm, BGM_Athletic::playScene, engine.MusicVolume - 0.2f, true);

	return true;
}
/**
* シーンを更新する
*/
void AthleticScene::Update(Engine& engine, float deltaTime)
{
	//case文で分けるとごちゃごちゃしてしまうため、分けて書く
	// 状態に対応する更新関数を実行
	state(this, engine, deltaTime);

	if (fadeTimer > 0) {
		// タイマーに合わせて徐々にフェードアウトさせる
		fadeTimer -= deltaTime;
		FadeObject->color[3] = 1 - fadeTimer;

		// タイマーが0以下になってかつ対応するフラグが合えばシーンを切り替える
		//boolでどのシーンに行くかはわかれている.
		if (fadeTimer <= 0.0f && isExploration) {
			engine.SetNextScene<ExplorationScene>();

		}
		else if (fadeTimer <= 0.0f && isAthleticScene)
		{
			engine.SetNextScene<AthleticScene>();

		}

	} // if fadeTimer

	//プレイヤーが死んだら直でExplorationSceneに戻るようにしている
	if (playerComponent->GetState() == PlayerComponent::PlayerState::dead)
	{
		engine.SetNextScene<TitleScene>();
	}

}
/**
* シーンを終了する
*
*/
void AthleticScene::Finalize(Engine& engine)
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
void AthleticScene::State_Playing(Engine& engine, float deltaTime)
{
	// 初期設定	 
	auto dartEffect = dartSmokeEffectComponent->GetOwner();
	auto gateEffect = gateEffectComponent->GetOwner();
	auto gate = gateComponent->GetOwner();
	auto gemsUI = gemsUIComponent->GetOwner();
	auto itemBlock = itemBlockComponent->GetOwner();
	auto trackingEnemy = trackingEnemyComponent->GetOwner();
	auto normalEnemy = normalEnemyComponent->GetOwner();
	auto meScene = moveExplorationScene->GetOwner();
	auto player = playerComponent->GetOwner();

	//プレイヤーのリスタート
	//プレイヤーのポジションが一定値よりも下になったらリスタートするように
	if (playerComponent->GetPosition().y <= -2.0f)
	{

		if (fadeTimer <= 0)
		{
			//playerの動きを止めている
			playerComponent->SetState(PlayerComponent::PlayerState::stop);
			// フェードアウト用UIオブジェクト
			// 画面全体を覆うサイズに設定
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;

			//ゲームオーバーの文字表示
			auto gameOverLogo =
				engine.CreateUIObject<UILayout>(Image_Athletic::over_logo, "Title", { 0, 0.4f }, 0.2f);
			gameOverLogo.second->color.w = 0;
			//リトライの文字表示
			auto retryButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::retry_button, "Title", { 0.0f, -0.4f }, 0.12f);
			// ボタンが押されたらメインゲームシーンに切り替えるラムダ式を設定
			retryButton.second->onClick.push_back(
				[this](UIButton* button) {
					fadeTimer = 1;			//シーン遷移の処理を始める
					isAthleticScene = true;	//もう一度始める際のフラグをOnに
					button->interactable = false; // ボタンを無効化
					EasyAudio::PlayOneShot(SE::buttonClick);
				});
			//バックの文字表示
			auto backButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::back_button, "Title", { 0.0f, -0.7f }, 0.12f);
			backButton.second->onClick.push_back(
				[this](UIButton* button)
				{
					fadeTimer = 1;			//シーン遷移の処理を始める
					isExploration = true;	//Explorationに戻る際のフラグをOnに
					button->interactable = false; // ボタンを無効化
					EasyAudio::PlayOneShot(SE::buttonClick);

				});

		}// if fadeTimer
	}	//if playerComponent->GetPosition().y <= -2.0f


	//itemBlockに触れたら判定がOnになり
	//追跡型のエネミーの誕生する
	if (itemBlockComponent->isActive)
	{
		trackingEnemy->position.y = 1.0f;
		trackingEnemyComponent->Tracking(player->position, deltaTime);
	}

	//プレイヤーがジャンプ台に触れたら上方向に吹っ飛ばす処理
	if (playerComponent->GetState() == PlayerComponent::PlayerState::jumpinghit)
	{
		for (int i = 0; i < 5; i++)
		{
			playerComponent->Jumping(deltaTime);
		}
		if (player->isGrounded)
			playerComponent->SetState(PlayerComponent::PlayerState::alive);
	}//if playerComponent->GetState() == PlayerComponent::PlayerState::jumpinghit


	//UIで自分がどれだけ取れているかを表示
	//右から順にアイテムが追加されているように
	if (playerComponent->GetPoint() == 2)gemsUIComponent->SetIsHitPlayer(true);
	if (playerComponent->GetPoint() == 4)gemsUIComponent->SetIsHitPlayer1(true);
	if (playerComponent->GetPoint() == 6)gemsUIComponent->SetIsHitPlayer2(true);
	if (playerComponent->GetPoint() == 8)gemsUIComponent->SetIsHitPlayer3(true);

	//プレイヤーがジェムをゲットしてポイントが10になったら
	//ゴールの門が出現する仕様に
	
	if (playerComponent->GetPoint() >= 10)
	{
		//ジェムUIの画像のalpha値を1に
		gemsUIComponent->SetIsHitPlayer4(true);
		//ゲート本体の設定
		gateComponent->SetState(Gate::GateState::alive);
		//ゲート発生時にでるスモークの設定
		dartSmokeEffectComponent->SetState(DartSmokeEffect::DartSmokeEffectState::alive);
		gateEffectComponent->SetState(GateEffect::GateEffectState::alive);

		//タイミングの計測
		timer++;
		if (timer > 60)
		{
			timerCount++;
			timer = 0;
		}

		//タイミングが丁度いいタイミングで発動できるようにしている
		if (timerCount >= 15)
		{
			gateEffect->position.y = 5.5f;
			meScene->position.y = 5.5f;
			dartSmokeEffectComponent->SetState(DartSmokeEffect::DartSmokeEffectState::dead);
		}

	}//if playerComponent->GetPoint() >= 10


	//ExplorationSceneに遷移させる際のフラグとフェードタイマーを設定

	if (moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside)
	{
		//プレイヤーをアスレチックシーンに遷移させる
		if (fadeTimer <= 0) {
			fadeTimer = 1.0;		//シーン遷移の処理を始める
			isExploration = true;	//Explorationに戻る際のフラグをOnに
			// フェードアウト用UIオブジェクト
			// 画面全体を覆うサイズに設定
			// カラーを「黒、透明」に設定
			auto fade = engine.CreateUIObject<UILayout>("Res/Image/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;
		}//if fadeTimer
	}//	if moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside

}
/**
* 「ゲームオーバー」状態のシーンを更新する
*
* @param engine    ゲームエンジン
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void AthleticScene::State_GameOver(Engine& engine, float deltaTime)
{
	//先にプレイヤーを削除する
	auto player = playerComponent->GetOwner();
	player->Destroy();

}
