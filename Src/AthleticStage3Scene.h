#ifndef ATHLETICSTAGE3SCENE_H_INCLUDED
#define ATHLETICSTAGE3SCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include "Engine/Vecmath.h"
#include <functional>

// 先行宣言
class GameObjectSettingsJSON;
class MeasureTime;
class MoveExplorationScene;
class PlayerComponent;
class TextBlock;
class TornadoEffect;
/**
* アスレチックゲームシーンのStage3
*/
class AthleticStage3Scene : public Scene
{
public:
	AthleticStage3Scene() = default;
	virtual ~AthleticStage3Scene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;
	// ゲームの状態
	using State = std::function<void(AthleticStage3Scene*, Engine&, float)>;
	State state = &AthleticStage3Scene::State_Playing;

	// 状態に対応する更新関数
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;		//画面のシーン遷移用のオブジェクト

	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson; //GameObjectSettingsJSONヘッダーが入る箱
	std::shared_ptr<MeasureTime> measureTimeComponent;				//MeasureTimeヘッダーが入る箱
	std::shared_ptr<MoveExplorationScene> moveExplorationScene;		//MoveExplorationSceneヘッダーが入る箱
	std::shared_ptr<PlayerComponent> playerComponent;				//PlayerComponentヘッダーが入る箱
	std::shared_ptr<TornadoEffect> tornadoComponent;				//TornadoEffectヘッダーが入る箱
	std::shared_ptr<TextBlock> textBlockComponent;					//TextBlockヘッダーが入る箱

	float fadeTimer =0.0f;	//画面のシーン遷移用のオブジェクト

	bool isAthleticStage3 = false;	//AthleticScene2に遷移させるときに使うフラグ
	bool isGoalScene = false;		//GoalSceneに遷移させるときに使うフラグ
	bool isTimeOut = false;			//タイムアップになったらゲームオーバーになるフラグ
};

#endif // ATHLETICSTAGE2SCENE_H_INCLUDED