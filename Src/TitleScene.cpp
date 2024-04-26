#include "TitleScene.h"
//他のオブジェクトヘッダーから
#include "AudioSettings.h"
#include "ExplorationScene.h"
#include "ImageSettings.h"
//Engine経由の機能に関するヘッダーから
#include "Engine/UILayout.h"
#include "Engine/Engine.h"
#include "Engine/UIButton.h"
#include "Engine/Light.h"
#include "Engine/EasyAudio/EasyAudio.h"

/**
* タイトル画面を初期化する
*
* @retval true  初期化成功
* @retval false 初期化失敗
*/
bool TitleScene::Initialize(Engine& engine)
{
	  // ライトを配置
	auto lightObject = engine.Create<GameObject>("light", "light",
		engine.GetMainCamera().position);
	auto light = lightObject->AddComponent<Light>();
	light->color = { 1.0f, 0.9f, 0.8f };
	light->intensity = 10;
	light->radius = 5;
	
	//タイトルの背景を表示している
	auto b = engine.CreateUIObject<UILayout>(Image_Title::title_bg, "Title", { 0, 0 }, 1);
	b.first->renderQueue = RenderQueue_overlay ;

	//タイトルのロゴを表示している
	auto l = engine.CreateUIObject<UILayout>(Image_Title::title_logo, "Title", { 0, 0.4f }, 0.2f);
	l.first->renderQueue = RenderQueue_overlay ;

	//進む際のボタン
	auto startButton = 
		engine.CreateUIObject<UIButton>(Image_Title::start_button,"Start", { 0.0f, -0.4f }, 0.12f);
	startButton.first->renderQueue = RenderQueue_overlay ;

	//戻る際のボタン
	auto QuitButton = 
		engine.CreateUIObject<UIButton>(Image_Title::quit_button, "Quit", { 0.0f, -0.7f }, 0.12f);
	QuitButton.first->renderQueue = RenderQueue_overlay ;

	//進む際のボタンを押されたときの処理
	startButton.second->onClick.push_back(
		[this](UIButton* button) {
			fadeTimer = 1;					//シーン遷移の処理を始める
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
			isFinished = true;				//ゲームを終了させる
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

	// BGMを再生
	EasyAudio::Play(AudioPlayer::bgm, BGM_Exploration::title, engine.MusicVolume, true);

	return true; // 初期化成功
}

/**
* タイトル画面の更新処理
*
* @param engine    ゲームエンジン
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void TitleScene::Update(Engine& engine, float deltaTime)
{
	if (fadeTimer > 0) {
		// タイマーに合わせて徐々にフェードアウトさせる
		fadeTimer -= deltaTime * 2;
		fadeObject->color.w = 1 - fadeTimer;

		// フェードアウト用UIオブジェクト
		auto Loading = engine.CreateUIObject<UILayout>(Image_Fade::nowLoading, "Loading", { 0.8f, -0.8f }, 0.1f);
		Loading.first->color.w = 1 - fadeTimer;

		// タイマーが0以下になったらメインゲームシーンに切り替える
		if (fadeTimer <= 0.0f) {
			engine.SetNextScene<ExplorationScene>();

		}

	} // if fadeTimer

	if (isFinished)
	{
		glfwTerminate();
	}
}
/**
* タイトル画面の終了処理
*/
void TitleScene::Finalize(Engine& engine)
{
	//ここで生成したオブジェクトの削除
	//音声のストップをする、それぞれでプレイヤー番号が違うのでそれぞれをしっかり指定する
	engine.ClearGameObjectAll();
	EasyAudio::Stop(AudioPlayer::bgm);
}

