#ifndef ATHLETICSCENE_H_INCLUDED
#define ATHLETICSCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include "Engine/Vecmath.h"
#include <functional>

// 先行宣言
class DartSmokeEffect;
class Gate;
class GateEffect;
class GameObjectSettingsJSON;
class GameObjectSettingsTXTMAP;
class GemsUI;
class ItemBlock;
class GemItem;
class JumpingPlatforms;
class MoveEnemy;
class MoveExplorationScene;
class NormalEnemy;
class PlayerComponent;

/**
* アスレチックゲームシーンの練習ステージ
*/
class AthleticScene : public Scene
{
public:
	AthleticScene() = default;
	virtual ~AthleticScene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;
	// ゲームの状態
	using State = std::function<void(AthleticScene*, Engine&, float)>;
	State state = &AthleticScene::State_Playing;

	// 状態に対応する更新関数
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;	 //画面のシーン遷移用のオブジェクト

	std::shared_ptr<DartSmokeEffect> dartSmokeEffectComponent;			//DartSmokeEffectヘッダーが入る箱
	std::shared_ptr<Gate> gateComponent;								//Gateヘッダーが入る箱
	std::shared_ptr<GateEffect> gateEffectComponent;					//GateEffectヘッダーが入る箱
	std::shared_ptr<GameObjectSettingsTXTMAP> gameObjectSettingstxtmap; //GameObjectSettingsTXTMAPヘッダーが入る箱
	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson;		//GameObjectSettingsJSONヘッダーが入る箱
	std::shared_ptr<GemsUI> gemsUIComponent;							//GemsUIヘッダーが入る箱
	std::shared_ptr<GemItem> itemComponent;								//GemItemヘッダーが入る箱
	std::shared_ptr<ItemBlock> itemBlockComponent;						//ItemBlockヘッダーが入る箱
	std::shared_ptr<JumpingPlatforms> jumpingPlatforms;					//JumpingPlatformsヘッダーが入る箱
	std::shared_ptr<MoveEnemy> trackingEnemyComponent;					//MoveEnemyヘッダーが入る箱
	std::shared_ptr<MoveExplorationScene> moveExplorationScene;			//MoveExplorationSceneヘッダーが入る箱
	std::shared_ptr<NormalEnemy> normalEnemyComponent;					//NormalEnemyヘッダーが入る箱
	std::shared_ptr<PlayerComponent> playerComponent;					//PlayerComponentヘッダーが入る箱

								 
	int timerCount = 0;			 //エフェクトなどの切り替え時のカウントタイマー
								 
	float fadeTimer = 0;		 //フェードオブジェクトの変化のスピードを変更してくれるタイマー
	float GEmoveTimer =0.0f;	 //ゲート発生時の動きを管理するカウント
	float timer = 0;			 //エフェクトなどの切り替え時のカウント
								 
	bool isAthleticScene = false;//AthleticSceneに遷移させるときに使うフラグ
	bool isExploration = false;	 //ExplorationSceneに遷移させるときに使うフラグ
};

#endif // ATHLETICSCENE_H_INCLUDED