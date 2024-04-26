#ifndef ATHLETICSTAGE3SCENE_H_INCLUDED
#define ATHLETICSTAGE3SCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include "Engine/Vecmath.h"
#include <functional>

// ��s�錾
class GameObjectSettingsJSON;
class MeasureTime;
class MoveExplorationScene;
class PlayerComponent;
class TextBlock;
class TornadoEffect;
/**
* �A�X���`�b�N�Q�[���V�[����Stage3
*/
class AthleticStage3Scene : public Scene
{
public:
	AthleticStage3Scene() = default;
	virtual ~AthleticStage3Scene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;
	// �Q�[���̏��
	using State = std::function<void(AthleticStage3Scene*, Engine&, float)>;
	State state = &AthleticStage3Scene::State_Playing;

	// ��ԂɑΉ�����X�V�֐�
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;		//��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g

	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson; //GameObjectSettingsJSON�w�b�_�[�����锠
	std::shared_ptr<MeasureTime> measureTimeComponent;				//MeasureTime�w�b�_�[�����锠
	std::shared_ptr<MoveExplorationScene> moveExplorationScene;		//MoveExplorationScene�w�b�_�[�����锠
	std::shared_ptr<PlayerComponent> playerComponent;				//PlayerComponent�w�b�_�[�����锠
	std::shared_ptr<TornadoEffect> tornadoComponent;				//TornadoEffect�w�b�_�[�����锠
	std::shared_ptr<TextBlock> textBlockComponent;					//TextBlock�w�b�_�[�����锠

	float fadeTimer =0.0f;	//��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g

	bool isAthleticStage3 = false;	//AthleticScene2�ɑJ�ڂ�����Ƃ��Ɏg���t���O
	bool isGoalScene = false;		//GoalScene�ɑJ�ڂ�����Ƃ��Ɏg���t���O
	bool isTimeOut = false;			//�^�C���A�b�v�ɂȂ�����Q�[���I�[�o�[�ɂȂ�t���O
};

#endif // ATHLETICSTAGE2SCENE_H_INCLUDED