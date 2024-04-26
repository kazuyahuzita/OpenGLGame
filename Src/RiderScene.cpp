#include "RiderScene.h"
//他のオブジェクトヘッダーから
#include "AudioSettings.h"
#include "Bike.h"
#include "FirstPersonCamera.h"
#include "GameObjectSettingsJSON.h"
#include "GoalObject.h"
#include "HorrorDeathScene.h"
#include "HorrorGoalScene.h"
#include "ImageSettings.h"
#include "MoveEnemy.h"
#include "TitleScene.h"
//Engine経由の機能に関するヘッダーから
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/Rider.h"
#include "Engine/UIButton.h"

bool RiderScene::Initialize(Engine& engine)
{
	// ゲームオブジェクト配置ファイルを読み込む
	auto object = engine.Create<GameObject>("object", "object", { 0,0,0 });
	gameObjectSettingsjson = object->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->RiderSceneMapLoad();

	//トンネルの中身を照らすスポットライト0
	vec3 pos = { 24,0.8f,-75 };
	auto lightObject = engine.Create<GameObject>("light", "light", pos, { 0,120,180 });
	lightObject->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Light/Light.obj");
	auto light = lightObject->AddComponent<Light>();
	light->color = { 0.95f, 1.0f, 0.98f };
	light->intensity = 150;
	light->radius = 50;
	light->type = Light::Type::spotLight;

	//トンネルの中身を照らすスポットライト1
	auto lightObject1 = engine.Create<GameObject>("light", "light", { 16.5,0.8f,-85 }, { 0,50,180 });
	lightObject1->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Light/Light.obj");
	auto light1 = lightObject1->AddComponent<Light>();
	light1->color = { 0.95f, 1.0f, 0.98f };
	light1->intensity = 150;
	light1->radius = 50;
	light1->type = Light::Type::spotLight;

	//トンネルの中身を照らすスポットライト2
	auto lightObject2 = engine.Create<GameObject>("light", "light", { 10,0.8f,-60 }, { 0,30,180 });
	lightObject2->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Light/Light.obj");
	auto light2 = lightObject2->AddComponent<Light>();
	light2->color = { 0.95f, 1.0f, 0.98f };
	light2->intensity = 150;
	light2->radius = 50;
	light2->type = Light::Type::spotLight;

	// プレイヤーを配置
	auto rider = engine.Create<GameObject>("Rider", "Rider", { 22, 1.6f, -20 }, { 0,0,0 });
	riderComponent = rider->AddComponent<Rider>();
	rider->AddComponent<FirstPersonCamera>();
	//プレイヤーの視界を制限するオブジェクト
	auto PlayerSphere =
		engine.Create<GameObject>("playerspehe", "playerspehe");
	PlayerSphere->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/PlayerSphere/PlayerSphere.obj");
	PlayerSphere->SetParent(rider);

	// エネミーの表示
	auto enemy = engine.Create<GameObject>("enemy", "enemy");
	enemy->AddComponent<MoveEnemy>();
	enemyComponent = enemy->AddComponent<MoveEnemy>();

	//ゴールオブジェクトの設置
	auto goal = engine.Create<GameObject>("goal", "goal");
	goal->AddComponent<GoalObject>();

	// 画像を表示
	auto Target =engine.CreateUIObject<UILayout>(Image_Horror::target, "target", { -1.6f, 0.9f }, 0.07f);
	auto Search =engine.CreateUIObject<UILayout>(Image_Horror::goal, "goal", { -1.1f, 0.9f }, 0.07f);

	// スカイスフィアを設定
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::graysphere);

	// BGMを再生
	EasyAudio::Play(AudioPlayer::bgm, BGM_Horror::riderScene, engine.MusicVolume-0.2f, true);

	return true;
}

void RiderScene::Update(Engine& engine, float deltaTime)
{
	//case文で分けるとごちゃごちゃしてしまうため、分けて書く
	// 状態に対応する更新関数を実行
	state(this, engine, deltaTime);

	//プレイヤーがクリアしたら直ぐにシーンを移動
	if (fadeTimer > 0)
	{
		// タイマーに合わせて徐々にフェードアウトさせる
		fadeTimer -= deltaTime;
		FadeObject->color[3] = 1 - fadeTimer ;

		// タイマーが0以下になったらゲームシーンに切り替える
		if (fadeTimer <= 0.94f) {
			engine.SetNextScene<HorrorGoalScene>();

		}

	} // if fadeTimer

	//escキーで終了
	if (isBack)
	{
		glfwTerminate();
	}

}

void RiderScene::Finalize(Engine& engine)
{
	//ここで生成したオブジェクトの削除
	//音声のストップをする、それぞれでプレイヤー番号が違うのでそれぞれをしっかり指定する
	EasyAudio::Stop(AudioPlayer::bgm);
	EasyAudio::Stop(AudioPlayer::se);
	EasyAudio::Stop(AudioPlayer::se1);
	engine.ClearGameObjectAll();
}

void RiderScene::State_Playing(Engine& engine, float deltaTime)
{
	//初期設定
	auto rider = riderComponent->GetOwner();
	auto enemy = enemyComponent->GetOwner();

	//エネミーにプレイヤーの位置を渡す
	enemyComponent->Tracking(rider->position, deltaTime);

	// プレイヤーが死んでいたら、ゲームオーバーシーンに移動
	if (riderComponent->GetState() == Rider::RiderState::dead)
	{
		// ゲームオーバー画像を表示
		engine.SetNextScene<HorrorDeathScene>();
	}

	// プレイヤーがゴールしたら、クリアシーンに移動
	if (riderComponent->GetState() == Rider::RiderState::goal)
	{
		if (fadeTimer <= 0)
		{
			//重ならないように位置を戻す.
			enemy->position = { 12,0,-25 };
			fadeTimer = 1;		//シーン遷移の処理を始める

			// フェードアウト用UIオブジェクト
			// 画面全体を覆うサイズに設定
			// カラーを「黒、透明」に設定
			auto goalfade = engine.CreateUIObject<UILayout>("Res/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = goalfade.first;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 4, 0.0f);
		}// if fadeTimer
	}//if riderComponent->GetState() == Rider::RiderState::goal

	//escキーを押されたときの対処法
	if (riderComponent->GetESC())
	{
		riderComponent->SetState(Rider::RiderState::stop);

		// 警告画像を表示
		engine.CreateUIObject<UILayout>("Res/Image/BackWindow.tga", "Back", { 0, 0 }, 0.25f);

		// はいボタンを表示
		auto Yesbutton = engine.CreateUIObject<UIButton>(
			"Res/Image/Yes.tga", "Yes", { -0.5f,  -0.5f }, 0.1f);
		Yesbutton.second->onClick.push_back([this](UIButton* button)
			{
				EasyAudio::PlayOneShot(SE::buttonClick);
				isBack = true;		//ゲームを終了する処理を進める
			});
		// いいえボタンを表示
		auto Nobutton = engine.CreateUIObject<UIButton>(
			"Res/Image/No.tga", "No", { 0.5f,  -0.5f }, 0.1f);
		Nobutton.second->onClick.push_back([this](UIButton* button)
			{
				button->interactable = false; // ボタンを無効化
				EasyAudio::PlayOneShot(SE::missClick);

			});

	}//if riderComponent->GetESC() == true

}

void RiderScene::State_GameOver(Engine& engine, float deltaTime)
{
	//先にプレイヤーを削除する
	auto rider = riderComponent->GetOwner();
	rider->Destroy();

}
