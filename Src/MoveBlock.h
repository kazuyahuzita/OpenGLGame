#ifndef MOVEBLOCK_H_INCLUDED
#define MOVEBLOCK_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

///<summary>
///MoveBlock�Ƃ������O�ŉ����c�ɓ������̂̃w�b�_�[���Ƃ��ė��p���Ă��܂�
///���̃R�[�h�����Ȃ������̂łЂƂ܂Ƃ߂ɂ��Ă��邾���Ȃ̂�
///���ꂩ�瑝�₷���̓w�b�_�[���΂�΂�ɂ�������ǂ�
///</summary>

/**
* ���ɓ�����
* Mesh�̐ݒ�����Ă��Ȃ��̂Ō�X���邩
* �������������̂ɂ����AddComponent����悤��
* ���ɂ̂ݓ�������
* Y���̒l��0��180�x�̎��AX���ɓ����z
*/
class XaxisMoveBlock: public Component
{
public:

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float timer = 0;
	//// �^�C�}�[�ɂ���č��W���㉺�ɗh�炷
	float range = 5.0f;// �h�炷�����͈̔�(m)
	float cycle = 1.0f;// �h�炷�Ԋu(�b)


};//class XaxisMoveBlock

/**
* ���ɓ�����
* Mesh�̐ݒ�����Ă��Ȃ��̂Ō�X���邩
* �������������̂ɂ����AddComponent����悤��
* ���ɂ̂ݓ�������
* Y���̒l��90��270�x�̎��AZ���ɓ����z
*/
class ZaxisMoveBlock: public Component
{
public:

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float timer = 0;
	//// �^�C�}�[�ɂ���č��W���㉺�ɗh�炷
	float range = 5.0f;// �h�炷�����͈̔�(m)
	float cycle = 1.0f;// �h�炷�Ԋu(�b)


};//class ZaxisMoveBlock

/**
* �c�ɓ�����
* Mesh�̐ݒ�����Ă��Ȃ��̂Ō�X���邩
* �������������̂ɂ����AddComponent����悤��
* �c�ɂ̂ݓ�������
*/
class VerticalMoveBlock :public Component
{
public:

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
private:
	float timer = 0;
	//// �^�C�}�[�ɂ���č��W���㉺�ɗh�炷
	float range = 4.0f;// �h�炷�����͈̔�(m)
	float cycle = 1.0f;// �h�炷�Ԋu(�b)

};//class VerticalMoveBlock

#endif // MOVEBLOCK_H_INCLUDED