#include "AthleticTitleScene.h"
//他のオブジェクトヘッダーから
#include "AthleticStage1Scene.h"
#include "AudioSettings.h"
#include "ExplorationScene.h"
#include "ImageSettings.h"
//Engine経由の機能に関するヘッダーから
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/Engine.h"
#include "Engine/Light.h"
#include "Engine/UIButton.h"
#include "Engine/UILayout.h"

/**
* タイトル画面を初期化する
*
* @retval true  初期化成功
* @retval false 初期化失敗
*/
bool AthleticTitleScene::Initialize(Engine& engine)
{
	  // ライトを配置
	auto lightObject = engine.Create<GameObject>("light", "light",
		engine.GetMainCamera().position);
	auto light = lightObject->AddComponent<Light>();
	light->color = { 1.0f, 0.9f, 0.8f };
	light->intensity = 10;
	light->radius = 5;

	//アスレチックゲームタイトルの背景を表示している
	auto b = engine.CreateUIObject<UILayout>(Image_Athletic::title_bg, "Title", { 0, 0 }, 1);
	b.first->renderQueue = RenderQueue_overlay ;

	//アスレチックゲームタイトルのロゴを表示している
	auto l = engine.CreateUIObject<UILayout>(Image_Athletic::title_logo, "Title", { 0, 0.4f }, 0.2f);
	l.first->renderQueue = RenderQueue_overlay ;

	//進む際のボタン
	auto StartButton = 
		engine.CreateUIObject<UIButton>(Image_Athletic::start_button,"Start", { 0.9f, -0.5f }, 0.12f);
	StartButton.first->renderQueue = RenderQueue_overlay ;

	//戻る際のボタン
	auto QuitButton = 
		engine.CreateUIObject<UIButton>(Image_Athletic::back_button, "Quit", { 0.9f, -0.8f }, 0.12f);
	QuitButton.first->renderQueue = RenderQueue_overlay ;
	//進む際のボタンを押されたときの処理
	StartButton.second->onClick.push_back(
		[this](UIButton* button) {
			fadeTimer = 1;					//シーン遷移の処理を始める
			isMove = true;					//戻る際のフラグをOnにする
			button->interactable = false;	// ボタンを無効化
			if (!isClick)
			{
				EasyAudio::PlayOneShot(SE::buttonClick);
				isClick = true;
			}
		});
	//戻る際のボタンを押されたときの処理
	QuitButton.second->onClick.push_back(
		[this](UIButton* button)
		{
			fadeTimer = 1;					//シーン遷移の処理を始める
			isNotMove = true;				//次に進める際のフラグをOnにする
			button->interactable = false;	// ボタンを無効化
			if (!isClick)
			{
				EasyAudio::PlayOneShot(SE::buttonClick);
				isClick = true;
			}

		});
	
  // フェードアウト用UIオブジェクト
	auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
	fadeObject = fade.first;
	  // 画面全体を覆うサイズに設定
	const vec2 fbSize = engine.GetFramebufferSize();
	fadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	  // カラーを「黒、透明」に設定
	std::fill_n(&fadeObject->color.x, 4, 0.0f);

	//void Play(再生に使うプレイヤー番号, 音声ファイル名, 音量, ループフラグ);
	// BGMを再生
	EasyAudio::Play(AudioPlayer::bgm, BGM_Athletic::title, engine.MusicVolume, true);

	return true; // 初期化成功
}

/**
* タイトル画面の更新処理
*
* @param engine    ゲームエンジン
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void AthleticTitleScene::Update(Engine& engine, float deltaTime)
{
	if (fadeTimer > 0) {
		// タイマーに合わせて徐々にフェードアウトさせる
		fadeTimer -= deltaTime * 2;
		fadeObject->color.w = 1 - fadeTimer;

		// フェードアウト用UIオブジェクト
		auto Loading = engine.CreateUIObject<UILayout>(Image_Fade::nowLoading, "Loading", { 0.8f, -0.8f }, 0.1f);
		Loading.first->color.w = 1 - fadeTimer;

		// タイマーが0以下になったらメインゲームシーンに切り替える
		//boolでどのシーンに行くかはわかれている.
		if (fadeTimer <= 0.0f && isMove) {
			engine.SetNextScene<AthleticStage1Scene>();
		}
		else if (fadeTimer <= 0.0f && isNotMove)
		{
			engine.SetNextScene<ExplorationScene>();
		}

	} // if fadeTimer
}
/**
* タイトル画面の終了処理
*/
void AthleticTitleScene::Finalize(Engine& engine)
{
	//ここで生成したオブジェクトの削除
	//音声のストップをする、それぞれでプレイヤー番号が違うのでそれぞれをしっかり指定する
	engine.ClearGameObjectAll();
	EasyAudio::Stop(AudioPlayer::bgm);
}

