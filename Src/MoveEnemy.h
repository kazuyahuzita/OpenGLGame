#ifndef MOVEENEMY_H_INCLUDED
#define MOVEENEMY_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/SphereCollider.h"
#include "Engine/VecMath.h"

/**
* player��ǂ�������G�l�~�[
* 
* vec3�^��position��ݒ�ł���悤�ɂ��Ă���
* @param void Tracking : Position,deltaTime
* �|�W�V������X��Z���Ƌ�����ݒ�ł���
* �����͈͓���true�A�͈͊O��false��Ԃ��B
* @param bool IsPlayerInRange�@:positionX,positionZ,range

*/
class MoveEnemy : public Component
{
public:
	MoveEnemy() = default;
	virtual ~MoveEnemy() = default;
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	// �Փˎ��̏���
	virtual void OnCollision(
		const ComponentPtr& self, const ComponentPtr& other) override;

	//MoveEnemy�̃X�e�[�g
	enum class MoveEnemyState
	{
		alive, // �����Ă���
		hit,   //��������
		stop,  //�~�܂�
		dead,  //����
	};
	MoveEnemyState GetState() const { return moveEnemyState; }
	void SetState(MoveEnemyState _state) {
		moveEnemyState = _state;
	}
	
	void Tracking(vec3 Position, float deltaTime) ;
	bool IsPlayerInRange(float positionX, float positionZ, float range) ;

private:
	MoveEnemyState moveEnemyState = MoveEnemyState::alive;
	const float speed = 2.0f;
};

#endif // MOVEENEMY_H_INCLUDED