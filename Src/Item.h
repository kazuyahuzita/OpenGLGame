#ifndef GEMITEM_H_INCLUDED
#define GEMITEM_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
/**
* �W�F���A�C�e���̔��������Ă����
*/
class GemItem : public Component
{
public:
	GemItem() = default;
	virtual ~GemItem() = default;
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	// �Փˎ��̏���
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other) override;

	// �W�F���A�C�e���̏�Ԃ�\���񋓌^
	enum class GemItemState
	{
		alive, // �����Ă���
		get,   // �Q�b�g���ꂽ
		dead,  // ����ł���

	};
	GemItemState GetState() const { return gemItemState; }

private:
	GemItemState gemItemState = GemItemState::alive; // �v���C���[�̏��

	float timer = 0; // ���쐧��p�^�C�}�[	
};

#endif // GEMITEM_H_INCLUDED