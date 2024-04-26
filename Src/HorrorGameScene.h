#ifndef HORRORGAMESCENE_H_INCLUDED
#define HORRORGAMESCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include <functional>

// ��s�錾
class Bike;
class GameObjectSettingsJSON;
class HorrorItemUI;
class MoveEnemy;
class PlayerComponent;
/**
* �z���[�Q�[���V�[��
*/
class HorrorGameScene : public Scene
{
public:
	HorrorGameScene() = default;
	virtual ~HorrorGameScene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;

	// �Q�[���̏��
	using State = std::function<void(HorrorGameScene*, Engine&, float)>;
	State state = &HorrorGameScene::State_Playing;

	// ��ԂɑΉ�����X�V�֐�
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:

	GameObjectPtr FadeObject;		//��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g

	std::shared_ptr<Bike> BikeComponent;							//Bike�w�b�_�[�����锠
	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson;	//GameObjectSettingsJSON�w�b�_�[�����锠
	std::shared_ptr<HorrorItemUI> horrorItemUIComponent;			//HorrorItemUI�w�b�_�[�����锠
	std::shared_ptr<MoveEnemy> enemyComponent;						//MoveEnemy�w�b�_�[�����锠
	std::shared_ptr<MoveEnemy> enemyComponent1;						//MoveEnemy�w�b�_�[�����锠
	std::shared_ptr<PlayerComponent> playerComponent;				//PlayerComponent�w�b�_�[�����锠

	float fadeTimer = 0;			//�t�F�[�h�I�u�W�F�N�g�̕ω��̃X�s�[�h��ύX���Ă����^�C�}�[

	bool isBack =false;				//�Q�[�����I��������t���O
	bool isDrive = false;			//RiderScene�ɑJ�ڂ�����t���O�𒼐ڐG�邽�߂Ɏg���t���O
	bool isExploration = false;		//ExplorationScene�ɑJ�ڂ�����Ƃ��Ɏg���t���O
	bool isHorrorGameScene = false;	//���g���C������ۂɎg���t���O
	bool isRiderScene = false;		//RiderScene�ɑJ�ڂ�����t���O
	bool isTrackingSE =false;		//�G���ǐ�BGM��炷�ۂɎg���t���O��1��
	bool isTrackingSE1 =false;		//�G���ǐ�BGM��炷�ۂɎg���t���O��2��
};

#endif // HORRORGAMESCENE_H_INCLUDED