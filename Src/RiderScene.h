#ifndef RIDERSCENE_H_INCLUDED
#define RIDERSCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include <functional>

// 先行宣言
class Rider;
class MoveEnemy;
class GameObjectSettingsJSON;
/**
* フィールド探検シーン
*/
class RiderScene : public Scene
{
public:
	RiderScene() = default;
	virtual ~RiderScene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;

	// ゲームの状態
	using State = std::function<void(RiderScene*, Engine&, float)>;
	State state = &RiderScene::State_Playing;

	// 状態に対応する更新関数
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;		//	画面のシーン遷移用のオブジェクト

	std::shared_ptr<Rider> riderComponent;							//Riderヘッダーが入る箱
	std::shared_ptr<MoveEnemy> enemyComponent;						//MoveEnemyヘッダーが入る箱
	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson;	//GameObjectSettingsJSONヘッダーが入る箱

	float fadeTimer = 0;		//フェードオブジェクトの変化のスピードを変更してくれるタイマー
	bool isBack = false;		//ゲームを終了させるフラグ
};

#endif // RIDERSCENE_H_INCLUDED