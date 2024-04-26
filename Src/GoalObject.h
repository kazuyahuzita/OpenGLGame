#ifndef GOALOBJECT_H_INCLUDED
#define GOALOBJECT_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/BoxCollider.h"

/**
* ゴール演出を行うコンポーネント
*  脱出ゲートの少し後ろに配置する想定のオブジェクト
*  nameをScene側のプログラムで指定しているので
*  それに合わせてPlayer側のプログラムで指定している
*/
class GoalObject : public Component
{
public:	GoalObject() = default;	
	  virtual ~GoalObject() = default;	
	  virtual void Awake() override;
};

#endif // GOALOBJECT_H_INCLUDED