#ifndef TITLESCENE_H_INCLUDED
 #define TITLESCENE_H_INCLUDED
 #include "Engine/GameObject.h"
 #include "Engine/Scene.h"

/**
* 一番初めのタイトル画面
*/
class TitleScene : public Scene
{
public:	TitleScene() = default;
	  virtual ~TitleScene() = default;
	  virtual bool Initialize(Engine& engine) override;
	  virtual void Update(Engine& engine, float deltaTime) override;
	  virtual void Finalize(Engine& engine) override;
	  
private:
	GameObjectPtr fadeObject;	//画面のシーン遷移用のオブジェクト

	float fadeTimer = 0;		//フェードオブジェクトの変化のスピードを変更してくれるタイマー

	bool isClick = false;		//SEが連発して聞こえることが起きたため、その対策にあるフラグ
	bool isFinished = false;	//ゲーム終了させるフラグ
};

#endif // TITLESCENE_H_INCLUDED