#ifndef HORRORGAMESCENE_H_INCLUDED
#define HORRORGAMESCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include <functional>

// 先行宣言
class Bike;
class GameObjectSettingsJSON;
class HorrorItemUI;
class MoveEnemy;
class PlayerComponent;
/**
* ホラーゲームシーン
*/
class HorrorGameScene : public Scene
{
public:
	HorrorGameScene() = default;
	virtual ~HorrorGameScene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;

	// ゲームの状態
	using State = std::function<void(HorrorGameScene*, Engine&, float)>;
	State state = &HorrorGameScene::State_Playing;

	// 状態に対応する更新関数
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:

	GameObjectPtr FadeObject;		//画面のシーン遷移用のオブジェクト

	std::shared_ptr<Bike> BikeComponent;							//Bikeヘッダーが入る箱
	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson;	//GameObjectSettingsJSONヘッダーが入る箱
	std::shared_ptr<HorrorItemUI> horrorItemUIComponent;			//HorrorItemUIヘッダーが入る箱
	std::shared_ptr<MoveEnemy> enemyComponent;						//MoveEnemyヘッダーが入る箱
	std::shared_ptr<MoveEnemy> enemyComponent1;						//MoveEnemyヘッダーが入る箱
	std::shared_ptr<PlayerComponent> playerComponent;				//PlayerComponentヘッダーが入る箱

	float fadeTimer = 0;			//フェードオブジェクトの変化のスピードを変更してくれるタイマー

	bool isBack =false;				//ゲームを終了させるフラグ
	bool isDrive = false;			//RiderSceneに遷移させるフラグを直接触るために使うフラグ
	bool isExploration = false;		//ExplorationSceneに遷移させるときに使うフラグ
	bool isHorrorGameScene = false;	//リトライさせる際に使うフラグ
	bool isRiderScene = false;		//RiderSceneに遷移させるフラグ
	bool isTrackingSE =false;		//敵が追跡BGMを鳴らす際に使うフラグの1つ目
	bool isTrackingSE1 =false;		//敵が追跡BGMを鳴らす際に使うフラグの2つ目
};

#endif // HORRORGAMESCENE_H_INCLUDED