#ifndef HORRORTITLESCENE_H_INCLUDED
 #define HORRORTITLESCENE_H_INCLUDED
 #include "Engine/Scene.h"
 #include "Engine/GameObject.h"

/**
* Horrorゲーム用のタイトル画面
*/
class HorrorTitleScene : public Scene
{
public:	HorrorTitleScene() = default;
	  virtual ~HorrorTitleScene() = default;
	  virtual bool Initialize(Engine& engine) override;
	  virtual void Update(Engine& engine, float deltaTime) override;
	  virtual void Finalize(Engine& engine) override;
	  
private:
	GameObjectPtr fadeObject;	//画面のシーン遷移用のオブジェクト
	float fadeTimer = 0;		//フェードオブジェクトの変化のスピードを変更してくれるタイマー

	bool isClick = false;		//SEが連発して聞こえることが起きたため、その対策にあるフラグ
	bool isMove = false;		//進むボタンを押されたときのフラグ
	bool isNotMove = false;		//戻るボタンを押されたときのフラグ
};

#endif // HORRORTITLESCENE_H_INCLUDED