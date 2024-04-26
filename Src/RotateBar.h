#ifndef ROTATEBAR_H_INCLUDED
#define ROTATEBAR_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"
/**
* 回転するバー
*  回転する仕様以外は書いていないので
*  右回りをし続けるだけの仕様です
*/
class RotateBar : public Component
{
public:
	RotateBar() = default;
	virtual ~RotateBar() = default;

	virtual void Update(float deltaTime) override;

	enum class RotateBarState
	{
		alive,
		dead,
		stop,

	};

	RotateBarState GetState() const { return rotateBarState; }

	void SetState(RotateBarState _state) {
		rotateBarState = _state;
	}

private:
	RotateBarState rotateBarState = RotateBarState::alive;
	float speed = radians(100);		//バーの回転スピード
};

#endif // ROTATEBAR_H_INCLUDED