#ifndef GAMEOBJECTSETTINGSJSON_H_INCLUDED
#define GAMEOBJECTSETTINGSJSON_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
/**
* ���K�V�[���̃A�C�e���⓮���Ă��镨�̂�ݒ肵�Ă���
* @param Json�t�@�C���ŃI�u�W�F�N�g�𐶐����Ă���
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

	float timer = 0; // ���쐧��p�^�C�}�[	
};

#endif // GAMEOBJECTSETTINGSJSON_H_INCLUDED