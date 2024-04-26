#ifndef GOALOBJECT_H_INCLUDED
#define GOALOBJECT_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/BoxCollider.h"

/**
* �S�[�����o���s���R���|�[�l���g
*  �E�o�Q�[�g�̏������ɔz�u����z��̃I�u�W�F�N�g
*  name��Scene���̃v���O�����Ŏw�肵�Ă���̂�
*  ����ɍ��킹��Player���̃v���O�����Ŏw�肵�Ă���
*/
class GoalObject : public Component
{
public:	GoalObject() = default;	
	  virtual ~GoalObject() = default;	
	  virtual void Awake() override;
};

#endif // GOALOBJECT_H_INCLUDED