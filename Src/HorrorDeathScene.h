#ifndef HORRORDEATHSCENE_H_INCLOUDED
#define HORRORDEATHSCENE_H_INCLOUDED
#include "Engine/GameObject.h"
#include "Engine/Scene.h"

/**
* HorrorGameSceneの死んでしまった時にあるタイトルシーン
*/
class HorrorDeathScene : public Scene
{
public:	HorrorDeathScene() = default;
	  virtual ~HorrorDeathScene() = default;
	  virtual bool Initialize(Engine& engine) override;
	  virtual void Update(Engine& engine, float deltaTime) override;
	  virtual void Finalize(Engine& engine) override;
	  
private:
	GameObjectPtr fadeObject;	//画面のシーン遷移用のオブジェクト
	float fadeTimer = 0;		//フェードオブジェクトの変化のスピードを変更してくれるタイマー

	bool isClick =false;		//SEが連発して聞こえることが起きたため、その対策にあるフラグ
};

#endif // HORRORDEATHSCENE_H_INCLOUDED
