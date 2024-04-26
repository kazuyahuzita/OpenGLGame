#ifndef ATHLETICGOALSCENE_H_INCLOUDED
#define ATHLETICGOALSCENE_H_INCLOUDED
#include "Engine/Scene.h"
#include "Engine/GameObject.h"

/**
* AthleticStage3Scene�̌�ɗ����AthleticScene�̃Q�[���N���A�V�[��
*/
class AthleticGoalScene : public Scene
{
public:	AthleticGoalScene() = default;
	  virtual ~AthleticGoalScene() = default;
	  virtual bool Initialize(Engine& engine) override;
	  virtual void Update(Engine& engine, float deltaTime) override;
	  virtual void Finalize(Engine& engine) override;
	  
private:
	GameObjectPtr fadeObject;	//��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g
	float fadeTimer = 0;		//�t�F�[�h�I�u�W�F�N�g�̕ω��̃X�s�[�h��ύX���Ă����^�C�}�[
	bool isClick = false;		//SE���A�����ĕ������邱�Ƃ��N�������߁A���̑΍�ɂ���t���O
};

#endif // ATHLETICGOALSCENE_H_INCLOUDED
