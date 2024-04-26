#ifndef ITEMBLOCK_H_INCLUDED
#define ITEMBLOCK_H_INCLUDED
#include "Engine/Engine.h"
#include "Engine/Component.h"
#include "Engine/SphereCollider.h"
/**
* AthleticScene�ɂĒǐՂ���G�l�~�[���o���Ă����A�C�e���Ƃ��Ĕz�u���Ă���
*/
class ItemBlock : public Component
{
public:	ItemBlock() = default;
	  virtual ~ItemBlock() = default;
	  virtual void Awake()override;
	  virtual void Update(float deltaTime) override;
	  // �Փˎ��̏���
	  virtual void OnCollision(
		  const ComponentPtr& self, const ComponentPtr& other) override;

	  // �v���C���[�̏�Ԃ�\���񋓌^
	  enum class ItemBlockState
	  {
		  Onswitch,		// ������Ă��Ȃ�
		  Offswitch,	// ������Ă���
		  stopswitch	//�@������������
	  };
	  ItemBlockState GetState() const { return IBstate; }

	  void SetState(ItemBlockState _state) {
		  IBstate = _state;
	  }

	float Timer = 0.0f;
	bool isActive = false;
private:

	ItemBlockState IBstate = ItemBlockState::Onswitch; // ���
	float speed = 0.05f;
};

#endif // ITEMBLOCK_H_INCLUDED