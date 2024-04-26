#include "HorrorGoalScene.h"
//他のオブジェクトヘッダーから
#include "AudioSettings.h"
#include "ImageSettings.h"
#include "TitleScene.h"
//Engine経由の機能に関するヘッダーから
#include "Engine/UILayout.h"
#include "Engine/Engine.h"
#include "Engine/UIButton.h"
#include "Engine/Light.h"
#include "Engine/EasyAudio/EasyAudio.h"

/**
* 初期化する
*
* @retval true  初期化成功
* @retval false 初期化失敗
*/
bool HorrorGoalScene::Initialize(Engine& engine)
{
	  // ライトを配置
	auto lightObject = engine.Create<GameObject>("light","light",
		engine.GetMainCamera().position);
	auto light = lightObject->AddComponent<Light>();
	light->color = { 1.0f, 0.9f, 0.8f };
	light->intensity = 10;
	light->radius = 5;
	
	//ゴールの背景を表示している
	auto b = engine.CreateUIObject<UILayout>(Image_Horror::clear_bg,"Clear", {0, 0}, 1);
	b.first->renderQueue = RenderQueue_overlay ;
	//ゴールのロゴを表示している
	auto l = engine.CreateUIObject<UILayout>(Image_Horror::clear_logo, "Clear", { 0, 0.4f }, 0.3f);
	l.first->renderQueue = RenderQueue_overlay;
	//進む際のボタン
	auto nextButton = 
		engine.CreateUIObject<UIButton>(Image_Horror::back_button, "Back", { 0, -0.6f }, 0.2f);
	nextButton.first->renderQueue = RenderQueue_overlay ;

	//進む際のボタンを押されたときの処理
	nextButton.second->onClick.push_back(
		[this](UIButton* button) {
			fadeTimer = 1;		
			button->interactable = false; // ボタンを無効化
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
	EasyAudio::Play(AudioPlayer::bgm, BGM_Athletic::clear, engine.MusicVolume, true);

	return true; // 初期化成功
}

/**
* タイトル画面の更新処理
*
* @param engine    ゲームエンジン
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void HorrorGoalScene::Update(Engine& engine, float deltaTime)
{
	if (fadeTimer > 0) {
		// タイマーに合わせて徐々にフェードアウトさせる
		fadeTimer -= deltaTime;
		fadeObject->color.w = 1 - fadeTimer;

		// タイマーが0以下になったらメインゲームシーンに切り替える
		if (fadeTimer <= 0) {
			engine.SetNextScene<TitleScene>();

		}

	} // if fadeTimer
}
/**
* タイトル画面の終了処理
*/
void HorrorGoalScene::Finalize(Engine& engine)
{
	//ここで生成したオブジェクトの削除
	//音声のストップをする、それぞれでプレイヤー番号が違うのでそれぞれをしっかり指定する
	engine.ClearGameObjectAll();
	EasyAudio::Stop(AudioPlayer::bgm);
}