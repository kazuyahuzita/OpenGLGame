#include "RotateBar.h"

void RotateBar::Update(float deltaTime)
{
	//rotateBarState��alive�ȊO�̎��͓����Ȃ�
	if (rotateBarState != RotateBarState::alive)return;

	auto owner = GetOwner();
	owner->rotation.y += speed * deltaTime;

}

