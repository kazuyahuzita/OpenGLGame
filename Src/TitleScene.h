#ifndef TITLESCENE_H_INCLUDED
 #define TITLESCENE_H_INCLUDED
 #include "Engine/GameObject.h"
 #include "Engine/Scene.h"

/**
* ��ԏ��߂̃^�C�g�����
*/
class TitleScene : public Scene
{
public:	TitleScene() = default;
	  virtual ~TitleScene() = default;
	  virtual bool Initialize(Engine& engine) override;
	  virtual void Update(Engine& engine, float deltaTime) override;
	  virtual void Finalize(Engine& engine) override;
	  
private:
	GameObjectPtr fadeObject;	//��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g

	float fadeTimer = 0;		//�t�F�[�h�I�u�W�F�N�g�̕ω��̃X�s�[�h��ύX���Ă����^�C�}�[

	bool isClick = false;		//SE���A�����ĕ������邱�Ƃ��N�������߁A���̑΍�ɂ���t���O
	bool isFinished = false;	//�Q�[���I��������t���O
};

#endif // TITLESCENE_H_INCLUDED