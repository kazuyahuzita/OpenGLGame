#ifndef NORMALENEMY_H_INCLUDED
#define NORMALENEMY_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
#include <functional>
/**
* ������ӂ�����Ă���G�l�~�[
* 

*/
class NormalEnemy : public Component
{
public:
	NormalEnemy() = default;
	virtual ~NormalEnemy() = default;
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);
	enum class NormalEnemyState
	{
		alive,	//�����Ă��鎞
		stop,	//�����̃T�C�N���̈��
		hit,	//�v���C���[�ɓ���������
		dead,	//���񂾂Ƃ�
	};
	NormalEnemyState GetState() const { return normalEnemyState; }
	void SetState(NormalEnemyState _state) {
		normalEnemyState = _state;
	}


private:
	NormalEnemyState normalEnemyState = NormalEnemyState::alive;
	float moveTimer = 0; // ���쐧��p�^�C�}�[	
	float timer = 0; // ���쐧��p�^�C�}�[	
	float timerCount = 0; // ���쐧��p�^�C�}�[�J�E���g
	const float speed = 0.5f;
	float baseY = 0.6f;    // ��̍���(m)
	float range = 0.1f; // ���E�͈̔�(m)
	float cycle = 2;    // ���E���s��������Ԋu(�b)

};

#endif // NORMALENEMY_H_INCLUDED