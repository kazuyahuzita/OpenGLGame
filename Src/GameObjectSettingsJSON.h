#ifndef GAMEOBJECTSETTINGSJSON_H_INCLUDED
#define GAMEOBJECTSETTINGSJSON_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
/**
* 練習シーンのアイテムや動いている物体を設定している
* @param Jsonファイルでオブジェクトを生成している
*/
class GameObjectSettingsJSON : public Component
{
public:
	GameObjectSettingsJSON() = default;
	virtual ~GameObjectSettingsJSON() = default;

	void AsylumMapLoad();
	void AthleticStage1MapLoad();
	void AthleticStage2MapLoad();
	void AthleticStage3MapLoad();
	void AthleticSceneMapLoad();
	void ExplorationSceneMapLoad();
	void RiderSceneMapLoad();
	void SubMainGameSceneMapLoad();

private:

	float timer = 0; // 動作制御用タイマー	
};

#endif // GAMEOBJECTSETTINGSJSON_H_INCLUDED