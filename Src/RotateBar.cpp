#include "RotateBar.h"

void RotateBar::Update(float deltaTime)
{
	//rotateBarStateがalive以外の時は動かない
	if (rotateBarState != RotateBarState::alive)return;

	auto owner = GetOwner();
	owner->rotation.y += speed * deltaTime;

}

