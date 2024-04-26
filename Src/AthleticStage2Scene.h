#ifndef ATHLETICSTAGE2SCENE_H_INCLUDED
 #define ATHLETICSTAGE2SCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include "Engine/Vecmath.h"
#include <functional>

// ��s�錾
class GameObjectSettingsJSON;
class MeasureTime;
class MoveExplorationScene;
class MoveStage3Scene;
class PlayerComponent;
class TextBlock;
class TornadoEffect;
/**
* �A�X���`�b�N�Q�[���V�[����Stage2
*/
class AthleticStage2Scene : public Scene
{
public:
	AthleticStage2Scene() = default;
	virtual ~AthleticStage2Scene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;
	// �Q�[���̏��
	using State = std::function<void(AthleticStage2Scene*, Engine&, float)>;
	State state = &AthleticStage2Scene::State_Playing;

	// ��ԂɑΉ�����X�V�֐�
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;//��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g

	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson; //GameObjectSettingsJSON�w�b�_�[�����锠
	std::shared_ptr<MeasureTime> measureTimeComponent;				//MeasureTime�w�b�_�[�����锠
	std::shared_ptr<MoveExplorationScene> moveExplorationScene;		//MoveExplorationScene�w�b�_�[�����锠
	std::shared_ptr<MoveStage3Scene> moveStage3Scene;				//MoveStage3Scene�w�b�_�[�����锠
	std::shared_ptr<PlayerComponent> playerComponent;				//PlayerComponent�w�b�_�[�����锠
	std::shared_ptr<TornadoEffect> tornadoComponent;				//TornadoEffect�w�b�_�[�����锠��1��
	std::shared_ptr<TornadoEffect> tornadoComponent1;				//TornadoEffect�w�b�_�[�����锠��2��
	std::shared_ptr<TextBlock> textBlockComponent;					//TextBlock�w�b�_�[�����锠��1��
	std::shared_ptr<TextBlock> textBlockComponent1;					//TextBlock�w�b�_�[�����锠��2��

	float fadeTimer =0.0f;	//�t�F�[�h�I�u�W�F�N�g�̕ω��̃X�s�[�h��ύX���Ă����^�C�}�[

	bool isAthleticStage2 = false;//AthleticScene2�ɑJ�ڂ�����Ƃ��Ɏg���t���O
	bool isAthleticStage3 = false;//AthleticScene3�ɑJ�ڂ�����Ƃ��Ɏg���t���O
	bool isExploration = false;	  //ExplorationScene�ɑJ�ڂ�����Ƃ��Ɏg���t���O
	bool isTimeOut = false;		  //�^�C���A�b�v�ɂȂ�����Q�[���I�[�o�[�ɂȂ�t���O

};

#endif // ATHLETICSTAGE2SCENE_H_INCLUDED