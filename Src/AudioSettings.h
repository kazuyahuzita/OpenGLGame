#ifndef AUDIOSETTINGS_H_INCLUDED
 #define AUDIOSETTINGS_H_INCLUDED

//ファイルの名前を間違えないように、それぞれを引数に持たせる
/**
* 音声再生プレイヤー番号
*/
namespace AudioPlayer
{
	inline constexpr int bgm = 0; // BGMの再生に使うプレイヤー番号
	inline constexpr int se =  1; // SEの再生に使うプレイヤー番号
	inline constexpr int se1 =  2; //SEの再生に使うプレイヤー番号
	inline constexpr int playerse0 =  3; // PlayerのSEの再生に使うプレイヤー番号
	
} // namespace Player

/**
* BGM設定
*/
namespace BGM_Horror
{
	inline constexpr char title[]		= "Res/Audio/Horror/TitleScene.mp3"; // タイトル画面のBGM
	inline constexpr char playScene[]	= "Res/Audio/Horror/PlayScene.mp3"; // ゲームシーンのBGM
	inline constexpr char riderScene[]	= "Res/Audio/Horror/RiderScene.wav"; // ライダーシーンのBGM
	inline constexpr char clear[]		= "Res/Audio/Horror/GameClear.wav"; // ゲームクリアのBGM
	inline constexpr char over[]		= "Res/Audio/Horror/GameOver.wav"; // ゲームオーバーのBGM
}
namespace BGM_Athletic
{
	inline constexpr char title[] = "Res/Audio/Athletic/Athletic_TitleScene.mp3"; // タイトル画面のBGM
	inline constexpr char playScene[]  = "Res/Audio/Athletic/Athletic_PlayScene.mp3"; // ゲームシーンのBGM
	inline constexpr char clear[] = "Res/Audio/Athletic/GameClear.wav"; // ゲームクリアのBGM
	inline constexpr char over[]  = "Res/Audio/Athletic/GameOver.wav"; // ゲームオーバーのBGM

}
namespace BGM_Exploration
{
	inline constexpr char title[]	 = "Res/Audio/Exploration/Exploration_PlayScene.mp3"; // タイトル画面のBGM
	inline constexpr char playScene[] = "Res/Audio/Exploration/Exploration_TitleScene.mp3"; // ゲームシーンのBGM
}

/**
* 効果音設定
*/
namespace SE
{
	inline constexpr char buttonClick[]		= "Res/Audio/SE/ClickButton.mp3";		// ボタンクリック音
	inline constexpr char missClick[]		= "Res/Audio/SE/missClick.mp3";		// ボタンのミスクリック音
	inline constexpr char goalEvent[]		= "Res/Audio/SE/GameClear.m4a";		// ゴールに到達した
	inline constexpr char gameOverEvent[]	= "Res/Audio/SE/GameOver.m4a";			// 敵につかまった
	inline constexpr char GetGem[]			= "Res/Audio/SE/GetGem.mp3";			// ジェムをゲットした
	inline constexpr char Jump[]			= "Res/Audio/SE/Jump.mp3";				// ジャンプした
	inline constexpr char Walk[]			= "Res/Audio/SE/WalkSound.mp3";		// 歩いている時の音
	inline constexpr char Stoke[]			= "Res/Audio/SE/Stoke.wav";			// 敵の追跡BGM

} // namespace SE

#endif // AUDIOSETTINGS_H_INCLUDED