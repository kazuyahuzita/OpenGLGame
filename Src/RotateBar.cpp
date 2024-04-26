#include "RotateBar.h"

void RotateBar::Update(float deltaTime)
{
	//rotateBarState‚ªaliveˆÈŠO‚ÌŽž‚Í“®‚©‚È‚¢
	if (rotateBarState != RotateBarState::alive)return;

	auto owner = GetOwner();
	owner->rotation.y += speed * deltaTime;

}

