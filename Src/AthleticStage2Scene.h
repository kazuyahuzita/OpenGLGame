#ifndef ATHLETICSTAGE2SCENE_H_INCLUDED
 #define ATHLETICSTAGE2SCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include "Engine/Vecmath.h"
#include <functional>

// 先行宣言
class GameObjectSettingsJSON;
class MeasureTime;
class MoveExplorationScene;
class MoveStage3Scene;
class PlayerComponent;
class TextBlock;
class TornadoEffect;
/**
* アスレチックゲームシーンのStage2
*/
class AthleticStage2Scene : public Scene
{
public:
	AthleticStage2Scene() = default;
	virtual ~AthleticStage2Scene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;
	// ゲームの状態
	using State = std::function<void(AthleticStage2Scene*, Engine&, float)>;
	State state = &AthleticStage2Scene::State_Playing;

	// 状態に対応する更新関数
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;//画面のシーン遷移用のオブジェクト

	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson; //GameObjectSettingsJSONヘッダーが入る箱
	std::shared_ptr<MeasureTime> measureTimeComponent;				//MeasureTimeヘッダーが入る箱
	std::shared_ptr<MoveExplorationScene> moveExplorationScene;		//MoveExplorationSceneヘッダーが入る箱
	std::shared_ptr<MoveStage3Scene> moveStage3Scene;				//MoveStage3Sceneヘッダーが入る箱
	std::shared_ptr<PlayerComponent> playerComponent;				//PlayerComponentヘッダーが入る箱
	std::shared_ptr<TornadoEffect> tornadoComponent;				//TornadoEffectヘッダーが入る箱の1つ目
	std::shared_ptr<TornadoEffect> tornadoComponent1;				//TornadoEffectヘッダーが入る箱の2つ目
	std::shared_ptr<TextBlock> textBlockComponent;					//TextBlockヘッダーが入る箱の1つ目
	std::shared_ptr<TextBlock> textBlockComponent1;					//TextBlockヘッダーが入る箱の2つ目

	float fadeTimer =0.0f;	//フェードオブジェクトの変化のスピードを変更してくれるタイマー

	bool isAthleticStage2 = false;//AthleticScene2に遷移させるときに使うフラグ
	bool isAthleticStage3 = false;//AthleticScene3に遷移させるときに使うフラグ
	bool isExploration = false;	  //ExplorationSceneに遷移させるときに使うフラグ
	bool isTimeOut = false;		  //タイムアップになったらゲームオーバーになるフラグ

};

#endif // ATHLETICSTAGE2SCENE_H_INCLUDED