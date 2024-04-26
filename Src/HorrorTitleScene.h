#ifndef HORRORTITLESCENE_H_INCLUDED
 #define HORRORTITLESCENE_H_INCLUDED
 #include "Engine/Scene.h"
 #include "Engine/GameObject.h"

/**
* Horror�Q�[���p�̃^�C�g�����
*/
class HorrorTitleScene : public Scene
{
public:	HorrorTitleScene() = default;
	  virtual ~HorrorTitleScene() = default;
	  virtual bool Initialize(Engine& engine) override;
	  virtual void Update(Engine& engine, float deltaTime) override;
	  virtual void Finalize(Engine& engine) override;
	  
private:
	GameObjectPtr fadeObject;	//��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g
	float fadeTimer = 0;		//�t�F�[�h�I�u�W�F�N�g�̕ω��̃X�s�[�h��ύX���Ă����^�C�}�[

	bool isClick = false;		//SE���A�����ĕ������邱�Ƃ��N�������߁A���̑΍�ɂ���t���O
	bool isMove = false;		//�i�ރ{�^���������ꂽ�Ƃ��̃t���O
	bool isNotMove = false;		//�߂�{�^���������ꂽ�Ƃ��̃t���O
};

#endif // HORRORTITLESCENE_H_INCLUDED