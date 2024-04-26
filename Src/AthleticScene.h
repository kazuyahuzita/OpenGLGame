#ifndef ATHLETICSCENE_H_INCLUDED
#define ATHLETICSCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include "Engine/Vecmath.h"
#include <functional>

// ��s�錾
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
* �A�X���`�b�N�Q�[���V�[���̗��K�X�e�[�W
*/
class AthleticScene : public Scene
{
public:
	AthleticScene() = default;
	virtual ~AthleticScene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;
	// �Q�[���̏��
	using State = std::function<void(AthleticScene*, Engine&, float)>;
	State state = &AthleticScene::State_Playing;

	// ��ԂɑΉ�����X�V�֐�
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;	 //��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g

	std::shared_ptr<DartSmokeEffect> dartSmokeEffectComponent;			//DartSmokeEffect�w�b�_�[�����锠
	std::shared_ptr<Gate> gateComponent;								//Gate�w�b�_�[�����锠
	std::shared_ptr<GateEffect> gateEffectComponent;					//GateEffect�w�b�_�[�����锠
	std::shared_ptr<GameObjectSettingsTXTMAP> gameObjectSettingstxtmap; //GameObjectSettingsTXTMAP�w�b�_�[�����锠
	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson;		//GameObjectSettingsJSON�w�b�_�[�����锠
	std::shared_ptr<GemsUI> gemsUIComponent;							//GemsUI�w�b�_�[�����锠
	std::shared_ptr<GemItem> itemComponent;								//GemItem�w�b�_�[�����锠
	std::shared_ptr<ItemBlock> itemBlockComponent;						//ItemBlock�w�b�_�[�����锠
	std::shared_ptr<JumpingPlatforms> jumpingPlatforms;					//JumpingPlatforms�w�b�_�[�����锠
	std::shared_ptr<MoveEnemy> trackingEnemyComponent;					//MoveEnemy�w�b�_�[�����锠
	std::shared_ptr<MoveExplorationScene> moveExplorationScene;			//MoveExplorationScene�w�b�_�[�����锠
	std::shared_ptr<NormalEnemy> normalEnemyComponent;					//NormalEnemy�w�b�_�[�����锠
	std::shared_ptr<PlayerComponent> playerComponent;					//PlayerComponent�w�b�_�[�����锠

								 
	int timerCount = 0;			 //�G�t�F�N�g�Ȃǂ̐؂�ւ����̃J�E���g�^�C�}�[
								 
	float fadeTimer = 0;		 //�t�F�[�h�I�u�W�F�N�g�̕ω��̃X�s�[�h��ύX���Ă����^�C�}�[
	float GEmoveTimer =0.0f;	 //�Q�[�g�������̓������Ǘ�����J�E���g
	float timer = 0;			 //�G�t�F�N�g�Ȃǂ̐؂�ւ����̃J�E���g
								 
	bool isAthleticScene = false;//AthleticScene�ɑJ�ڂ�����Ƃ��Ɏg���t���O
	bool isExploration = false;	 //ExplorationScene�ɑJ�ڂ�����Ƃ��Ɏg���t���O
};

#endif // ATHLETICSCENE_H_INCLUDED