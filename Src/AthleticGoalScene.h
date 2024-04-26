#ifndef ATHLETICGOALSCENE_H_INCLOUDED
#define ATHLETICGOALSCENE_H_INCLOUDED
#include "Engine/Scene.h"
#include "Engine/GameObject.h"

/**
* AthleticStage3Sceneの後に来れるAthleticSceneのゲームクリアシーン
*/
class AthleticGoalScene : public Scene
{
public:	AthleticGoalScene() = default;
	  virtual ~AthleticGoalScene() = default;
	  virtual bool Initialize(Engine& engine) override;
	  virtual void Update(Engine& engine, float deltaTime) override;
	  virtual void Finalize(Engine& engine) override;
	  
private:
	GameObjectPtr fadeObject;	//画面のシーン遷移用のオブジェクト
	float fadeTimer = 0;		//フェードオブジェクトの変化のスピードを変更してくれるタイマー
	bool isClick = false;		//SEが連発して聞こえることが起きたため、その対策にあるフラグ
};

#endif // ATHLETICGOALSCENE_H_INCLOUDED
