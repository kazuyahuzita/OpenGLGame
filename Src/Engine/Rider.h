#ifndef RIDER_H_INCLUDED
#define RIDER_H_INCLUDED
#include "../FluorescentLight.h"
#include "CharacterMovement.h"
#include "Component.h"
#include "Engine.h"
#include "Light.h"
#include "SphereCollider.h"
#include "UILayout.h"
/**
* RiderScene�̎��̃v���C���[
*/
class Rider : public Component
{
public:	Rider() = default;
	  virtual ~Rider() = default;
	  virtual void Awake()override;
	  virtual void Update(float deltaTime) override;
	  // �Փˎ��̏���
	  virtual void OnCollision(
		  const ComponentPtr& self, const ComponentPtr& other) override;

	  // �v���C���[�̏�Ԃ�\���񋓌^
	  enum class RiderState
	  {
		  drive, // �����Ă���
		  goal,  // �S�[������
		  dead,  // ����ł���
		  stop,	//�~�߂����Ƃ���
	  };
	  RiderState GetState() const { return riderState; }

	  void SetState(RiderState _state) {
		  riderState = _state;
	  }

	  bool GetESC()const { return isESC; }
	  void SetESC(bool _esc) {
		  isESC = _esc;
	  }

private:
	GLFWgamepadstate gamepadstate;	//�Q�[���p�b�h�̏��������Ă���
	GameObjectPtr DashEffectObject;	//�_�b�V�����̃G�t�F�N�g1
	GameObjectPtr DashEffectObject1;//�_�b�V�����̃G�t�F�N�g2

	RiderState riderState = RiderState::drive; // Rider�V�[���̃v���C���[�̏��
	float timer = 0; // ���쐧��p�^�C�}�[	
	// �J�����̈ړ�
	//�\���L�[��WSAD�L�[�ŕ����邽�߂ɓ����
	const float cameraSpeed = 2.0f;
	const float addcameraMoveSpeed = 2.0f;

	const float addJumpPower = 2.0f;//�W�����v�̈З�
	float bikerotate = 0.3f;		//���E�Ɉړ�����Ƃ��̗�

	bool isESC = false;		//esc�L�[���������Ƃ��ɔ�������t���O

};

#endif // RIDER_H_INCLUDED