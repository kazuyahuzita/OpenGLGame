#ifndef EXPLORATIONSCENE_H_INCLUDED
#define EXPLORATIONSCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include "Engine/Vecmath.h"
#include <functional>

// 先行宣言
class GameObjectSettingsJSON;
class Gate;
class GateEffect;
class MeasureTime;
class MoveAthleticScene;
class MoveHorrorScene;
class MoveStage1Scene;
class PlayerComponent;
class TextBlock;
class UILayout;
/**
* フィールド探検シーン
*/
class ExplorationScene : public Scene
{
public:
	ExplorationScene() = default;
	virtual ~ExplorationScene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;
	// ゲームの状態
	using State = std::function<void(ExplorationScene*, Engine&, float)>;
	State state = &ExplorationScene::State_Playing;

	// 状態に対応する更新関数
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;	//画面のシーン遷移用のオブジェクト

	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson; //GameObjectSettingsJSONヘッダーが入る箱
	std::shared_ptr<Gate> gateComponent;							//GateComponentヘッダーが入る箱
	std::shared_ptr<GateEffect> gateEffectComponent;				//GateEffectComponentヘッダーが入る箱の1つ目
	std::shared_ptr<GateEffect> gateEffectComponent1;				//GateEffectComponentヘッダーが入る箱の2つ目
	std::shared_ptr<GateEffect> gateEffectComponent2;				//GateEffectComponentヘッダーが入る箱の3つ目
	std::shared_ptr<GateEffect> gateEffectComponent3;				//GateEffectComponentヘッダーが入る箱の4つ目
	std::shared_ptr<MoveAthleticScene> moveAthleticScene;			//MoveAthleticSceneヘッダーが入る箱
	std::shared_ptr<MoveHorrorScene> moveHorrorScene;				//MoveHorrorSceneヘッダーが入る箱
	std::shared_ptr<MoveStage1Scene> moveStage1Scene;				//MoveStage1Sceneヘッダーが入る箱
	std::shared_ptr<PlayerComponent> playerComponent;				//PlayerComponentヘッダーが入る箱
	std::shared_ptr<TextBlock> textBlockComponent;					//TextBlockヘッダーが入る箱の1つ目
	std::shared_ptr<TextBlock> textBlockComponent1;					//TextBlockヘッダーが入る箱の2つ目
	std::shared_ptr<TextBlock> textBlockComponent2;					//TextBlockヘッダーが入る箱の3つ目
	std::shared_ptr<TextBlock> textBlockComponent3;					//TextBlockヘッダーが入る箱の4つ目

	float fadeTimer =0.0f;		//フェードオブジェクトの変化のスピードを変更してくれるタイマー

	bool isAthletic = false;	//練習シーンに遷移させるときに使うフラグ
	bool isHorror = false;		//HorrorGameSceneに遷移させるときに使うフラグ
	bool isStage1 = false;		//AthleticScene1に遷移させるときに使うフラグ
};

#endif // EXPLORATIONSCENE_H_INCLUDED