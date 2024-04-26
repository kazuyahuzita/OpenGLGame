#ifndef EXPLORATIONSCENE_H_INCLUDED
#define EXPLORATIONSCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include "Engine/Vecmath.h"
#include <functional>

// ��s�錾
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
* �t�B�[���h�T���V�[��
*/
class ExplorationScene : public Scene
{
public:
	ExplorationScene() = default;
	virtual ~ExplorationScene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;
	// �Q�[���̏��
	using State = std::function<void(ExplorationScene*, Engine&, float)>;
	State state = &ExplorationScene::State_Playing;

	// ��ԂɑΉ�����X�V�֐�
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;	//��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g

	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson; //GameObjectSettingsJSON�w�b�_�[�����锠
	std::shared_ptr<Gate> gateComponent;							//GateComponent�w�b�_�[�����锠
	std::shared_ptr<GateEffect> gateEffectComponent;				//GateEffectComponent�w�b�_�[�����锠��1��
	std::shared_ptr<GateEffect> gateEffectComponent1;				//GateEffectComponent�w�b�_�[�����锠��2��
	std::shared_ptr<GateEffect> gateEffectComponent2;				//GateEffectComponent�w�b�_�[�����锠��3��
	std::shared_ptr<GateEffect> gateEffectComponent3;				//GateEffectComponent�w�b�_�[�����锠��4��
	std::shared_ptr<MoveAthleticScene> moveAthleticScene;			//MoveAthleticScene�w�b�_�[�����锠
	std::shared_ptr<MoveHorrorScene> moveHorrorScene;				//MoveHorrorScene�w�b�_�[�����锠
	std::shared_ptr<MoveStage1Scene> moveStage1Scene;				//MoveStage1Scene�w�b�_�[�����锠
	std::shared_ptr<PlayerComponent> playerComponent;				//PlayerComponent�w�b�_�[�����锠
	std::shared_ptr<TextBlock> textBlockComponent;					//TextBlock�w�b�_�[�����锠��1��
	std::shared_ptr<TextBlock> textBlockComponent1;					//TextBlock�w�b�_�[�����锠��2��
	std::shared_ptr<TextBlock> textBlockComponent2;					//TextBlock�w�b�_�[�����锠��3��
	std::shared_ptr<TextBlock> textBlockComponent3;					//TextBlock�w�b�_�[�����锠��4��

	float fadeTimer =0.0f;		//�t�F�[�h�I�u�W�F�N�g�̕ω��̃X�s�[�h��ύX���Ă����^�C�}�[

	bool isAthletic = false;	//���K�V�[���ɑJ�ڂ�����Ƃ��Ɏg���t���O
	bool isHorror = false;		//HorrorGameScene�ɑJ�ڂ�����Ƃ��Ɏg���t���O
	bool isStage1 = false;		//AthleticScene1�ɑJ�ڂ�����Ƃ��Ɏg���t���O
};

#endif // EXPLORATIONSCENE_H_INCLUDED