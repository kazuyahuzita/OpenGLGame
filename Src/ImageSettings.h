/**
* @file ImageSettings.h
*/
#ifndef IMAGESETTINGS_H_INCLUDED
#define IMAGESETTINGS_H_INCLUDED

//ファイルの名前を間違えないように、それぞれを引数に持たせる
//inline constexprは「定義をひとつに保つ」という意味
//インクルード先でコピーが作られて、実行ファイルのサイズが肥大化するのを防ぐ

/**
* 画像とロゴ画面の画像の設定
*/
namespace Image_Title
{
	inline constexpr char title_bg[]		= "Res/Image/Title/Title_StartBg.tga";		// タイトルの背景画像
	inline constexpr char title_logo[]		= "Res/Image/Title/Title_StartLogo.tga";	// タイトル画面のロゴ画像
	inline constexpr char start_button[]	= "Res/Image/Title/Title_StartButton.tga";	// スタートボタンの画像
	inline constexpr char quit_button[]		= "Res/Image/Title/Title_Quitbutton.tga";	// ゲーム終了ボタンの画像

} // namespace Image_Title

/*
* ホラーゲーム用の画像の設定
*
*/
namespace Image_Horror
{
	//タイトル用の画像
	inline constexpr char title_bg[] = "Res/Image/Horror/Horror_TitleBg.tga";				// ゲームクリアの画像
	//ゲームクリア用の画像
	inline constexpr char clear_bg[] = "Res/Image/Horror/Horror_GameClearbg.tga";				// ゲームクリアの画像
	inline constexpr char clear_logo[] = "Res/Image/Horror/Horror_GameClearlogo.tga";		// ゲームクリア画面のロゴ画像
	//ゲームオーバー用の画像
	inline constexpr char over_bg[] = "Res/Image/Horror/Horror_GameOverbg.tga";			// ゲームオーバーのロゴ画像
	inline constexpr char over_logo[] = "Res/Image/Horror/Horror_GameOverlogo.tga";			// ゲームオーバーのロゴ画像
	//ゲームプレイ中の画像
	inline constexpr char target[] 
		= "Res/Image/Horror/Horror_TextTarget.tga";			// ゲームシーンの画像(目標と書いてある)
	inline constexpr char search[] 
		= "Res/Image/Horror/Horror_TextSeach.tga";			// ゲームシーンの画像(やる内容が書いてある)
	inline constexpr char search_last[] 
		= "Res/Image/Horror/Horror_TextSearchLast.tga";		// ゲームシーンの画像(やる内容が書いてある)
	inline constexpr char goal[] 
		= "Res/Image/Horror/Horror_TextGoal.tga";			// ゲームシーンの画像(ゴールを目指すよう書いてある)

	//ボタン用の画像
	inline constexpr char back_button[]  = "Res/Image/Horror/Horror_Quitbutton.tga";			// 戻るときの画像
	inline constexpr char retry_button[] = "Res/Image/Athletic/Athletic_RetryButton.tga";			// 戻るときの画像
	inline constexpr char start_button[] = "Res/Image/Horror/Horror_StartButton.tga";			// 戻るときの画像

}//namespace Image_Horror

/*
* アスレチックゲーム用の画像の設定
*
*/
namespace Image_Athletic
{
	//タイトル用の画像
	inline constexpr char title_bg[]	   = "Res/Image/Athletic/Athletic_Titlebg.tga";				// タイトルの画像
	inline constexpr char title_logo[] = "Res/Image/Athletic/Athletic_Titlelogo.tga";		// タイトル画面のロゴ画像
	//ゲームクリア用の画像
	inline constexpr char clear_bg[]   = "Res/Image/Athletic/Athletic_GameClearbg.tga";		// ゲームクリアの画像
	inline constexpr char clear_logo[] = "Res/Image/Athletic/Athletic_GameClearlogo.tga";	// ゲームクリア画面のロゴ画像
	//ゲームオーバー用の画像
	inline constexpr char over_logo[]  = "Res/Image/Athletic/Athletic_GameOverlogo.tga";		// ゲームオーバーのロゴ画像
	//ゲームプレイ中の画像
	inline constexpr char backscene[]  = "Res/Image/Athletic/BackScene.tga";					// ステージ選択シーンに戻るテキスト画像
	inline constexpr char nextscene[]  = "Res/Image/Athletic/NextScene.tga";					// 次のステージに行くテキスト画像
	inline constexpr char clock[]	   = "Res/Image/Athletic/Clock.tga";							// 制限時間用の時計の画像
	inline constexpr char time_bg[]	   = "Res/Image/Athletic/Time_bg.tga";						// 制限時間の後ろ画像
	inline constexpr char opengate[]   = "Res/Image/Athletic/GateOpen.tga";					// 門が開いた時のテキスト画像
	//ボタン用の画像
	inline constexpr char back_button[]  = "Res/Image/Athletic/Athletic_BackButton.tga";			// 戻るときの画像
	inline constexpr char retry_button[] = "Res/Image/Athletic/Athletic_RetryButton.tga";			// 戻るときの画像
	inline constexpr char start_button[] = "Res/Image/Athletic/Athletic_StartButton.tga";			// 戻るときの画像

	//制限時間の時に使った数字の画像
	inline constexpr char number0[] = "Res/Image/Athletic/0.tga";			
	inline constexpr char number1[] = "Res/Image/Athletic/1.tga";			
	inline constexpr char number2[] = "Res/Image/Athletic/2.tga";			
	inline constexpr char number3[] = "Res/Image/Athletic/3.tga";			
	inline constexpr char number4[] = "Res/Image/Athletic/4.tga";			
	inline constexpr char number5[] = "Res/Image/Athletic/5.tga";			
	inline constexpr char number6[] = "Res/Image/Athletic/6.tga";			
	inline constexpr char number7[] = "Res/Image/Athletic/7.tga";			
	inline constexpr char number8[] = "Res/Image/Athletic/8.tga";			
	inline constexpr char number9[] = "Res/Image/Athletic/9.tga";			

}//namespace Image_Athletic

namespace Image_Fade
{
	//Fade用の画像
	inline constexpr char white[]  = "Res/Image/fade/white.tga"; // フェード用の白画像
	inline constexpr char black[]  = "Res/Image/fade/black.tga"; // フェード用の黒画像
	inline constexpr char nowLoading[]  = "Res/Image/fade/NowLoadig.tga"; // フェード用の黒画像

}//namespace Image_Fade

namespace Image_Sphere
{
	//Fade用の画像
	inline constexpr char skysphere[]  = "Res/MeshData/sky_sphere/sky.tga"; // フェード用の白画像
	inline constexpr char graysphere[]  = "Res/Image/fade/black.tga"; // フェード用の黒画像

}//namespace Image_Fade

#endif // IMAGESETTINGS_H_INCLUDED