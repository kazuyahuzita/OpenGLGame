#ifndef HORRORSCENEGOAL_H_INCLOUDED
#define HORRORSCENEGOAL_H_INCLOUDED
#include "Engine/GameObject.h"
#include "Engine/Scene.h"

/**
* RiderScene�ŃS�[���ɒ�������o��V�[��
*/
class HorrorGoalScene : public Scene
{
public:	HorrorGoalScene() = default;
	  virtual ~HorrorGoalScene() = default;
	  virtual bool Initialize(Engine& engine) override;
	  virtual void Update(Engine& engine, float deltaTime) override;
	  virtual void Finalize(Engine& engine) override;
	  
private:
	GameObjectPtr fadeObject;	//��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g
	float fadeTimer = 0;		//�t�F�[�h�I�u�W�F�N�g�̕ω��̃X�s�[�h��ύX���Ă����^�C�}�[
	bool isClick = false;		//SE���A�����ĕ������邱�Ƃ��N�������߁A���̑΍�ɂ���t���O

};

#endif // HORRORSCENEGOAL_H_INCLOUDED
