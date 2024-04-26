#ifndef PLAYERCOMPONENT_H_INCLUDED
#define PLAYERCOMPONENT_H_INCLUDED
#include "../AudioSettings.h"
#include "CharacterMovement.h"
#include "Component.h"
#include "EasyAudio/EasyAudio.h"
#include "Engine.h"
#include "Light.h"
#include "SlidingMovement.h"
#include "SphereCollider.h"
#include "UILayout.h"
/**
* �v���C���[
*/
class PlayerComponent : public Component
{
public:	PlayerComponent() = default;
	  virtual ~PlayerComponent() = default;
	  virtual void Awake()override;
	  virtual void Update(float deltaTime) override;
	  // �Փˎ��̏���
	  virtual void OnCollision(
		  const ComponentPtr& self, const ComponentPtr& other) override;
	  //�W�����v��ɐG�ꂽ�ۂɐ�����΂�����
	  void Jumping(float deltaTime);
	  //�G�ɐG�ꂽ�ۂɐ�����΂�����
	  void Knockback(float deltaTime);


	  // �v���C���[�̏�Ԃ�\���񋓌^
	  enum class PlayerState
	  {
		  alive,		// �����Ă���
		  drive,		// �����Ă���
		  athletic,		// �����Ă���
		  jumpinghit,	// ��������
		  nolmalhit,	// ������̓G�ɓ���������
		  dead,			// ����ł���
		  goal,			// �S�[������
		  stop			// �v���C���[�̓������~�߂����Ƃ�
	  };
	  PlayerState GetState() const { return state; }

	  void SetState(PlayerState _state) {
		  state = _state;
	  }
	  //�v���C���[�̃|�W�V����
	  vec3 GetPosition() const { return GetOwner()->position; }
	  //�v���C���[�̃|�W�V����
	  void SetPosition(vec3 _position)  { 
		   GetOwner()->position = _position;
	  }

	  //�G�l�~�[�ɐG�ꂽ���ǂ���
	  void SetEnemy(bool _enemy) {
		  isEnemy = _enemy;
	  }
	  bool GetEnemy()const { return isEnemy; }

	  //�G�l�~�[�ɐG�ꂽ���ǂ���
	  void SetJump(bool _jump) {
		  isJump = _jump;
	  }
	  bool GetJump()const { return isJump; }

	  //�|�C���g�Ǘ�
	  int GetPoint() const { return ItemPoint; }
	  //�L�[�A�C�e����������Ƃ��Ƃ��̏���
	  //�z���[�Q�[���V�[���Ŏg���Ă���t���O�Ǘ���Getter
	  bool GetKeyItem()const { return isKeyItem; }
	  bool GetTyreItem()const { return isTyre; }
	  bool GetBatteryItem()const { return isBattery; }
	  bool GetSeatItem()const { return isSeat; }

	  //�Q�[���I������
	  void SetESC(bool esc) {
		  isESC = esc;
	  }
	  bool GetESC()const { return isESC; }


private:
	GLFWgamepadstate gamepadstate;			//�Q�[���p�b�h�̏��������Ă���
	PlayerState state = PlayerState::alive; // �v���C���[�̏��

	GameObjectPtr DamageEffectObject;//�_�b�V�����̃G�t�F�N�g

	// �J�����̈ړ�
	//�\���L�[��WSAD�L�[�ŕ����邽�߂ɓ����
	const float cameraSpeed = 2.0f;
	const float cameraMoveSpeed = 3.0f; 
	float addDashPower = 1.0f;			//����ۂɕύX���鑖���
	float JumpPower = 4.0f;				//�W�����v�̈З�
	float JumpTimer = 0.0f;				//�W�����v�̍ۂɉ����d�Ȃ�Ȃ��悤�ɂ���ׂɃt���O�𐧌䂷��^�C�}�[

	int ItemPoint = 0;					//�W�F�����Q�b�g�����ۂ̃|�C���g���Z�Ɏg��

	bool isBattery = false;				//HorrorGameScene�ŃQ�b�g�����A�C�e���̃t���O
	bool isEnemy = false;				//�G�l�~�[�ɓ��������ۂ̃t���O
	bool isESC = false;					//�Q�[���I������̃I�u�W�F�N�g
	bool isFire = false;				//�t�@�C�A�I�u�W�F�N�g(��Q��)�ɐG�ꂽ�ۂ̃t���O
	bool isGoal = false;				//�S�[�������ۂ̃t���O
	bool isKeyItem = false;				//HorrorGameScene�ŃQ�b�g�����A�C�e���̃t���O
	bool isJump = false;				//�W�����v�̍ۂɉ����d�Ȃ�Ȃ��悤�ɂ���t���O
	bool isPlayerSlide = false;			//�X���C�h�u���b�N�ɐG�ꂽ�ۂ̔���
	bool isSeat = false;				//HorrorGameScene�ŃQ�b�g�����A�C�e���̃t���O
	bool isTyre = false;				//HorrorGameScene�ŃQ�b�g�����A�C�e���̃t���O

};

#endif // PLAYERCOMPONENT_H_INCLUDED