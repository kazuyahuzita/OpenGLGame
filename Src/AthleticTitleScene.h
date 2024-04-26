#ifndef ATHLETICTITLESCENE_H_INCLUDED
#define ATHLETICTITLESCENE_H_INCLUDED
 #include "Engine/GameObject.h"
 #include "Engine/Scene.h"

/**
* AthleticStageが始める前にあるタイトルシーン
*/
class AthleticTitleScene : public Scene
{
public:	AthleticTitleScene() = default;
	  virtual ~AthleticTitleScene() = default;
	  virtual bool Initialize(Engine& engine) override;
	  virtual void Update(Engine& engine, float deltaTime) override;
	  virtual void Finalize(Engine& engine) override;
	  
private:
	GameObjectPtr fadeObject;	//画面のシーン遷移用のオブジェクト

	float fadeTimer = 0;		//フェードオブジェクトの変化のスピードを変更してくれるタイマー

	bool isClick = false;		//SEが連発して聞こえることが起きたため、その対策にあるフラグ
	bool isMove =false;			//進むボタンを押されたときのフラグ
	bool isNotMove = false;		//戻るボタンを押されたときのフラグ
};

#endif // ATHLETICTITLESCENE_H_INCLUDED