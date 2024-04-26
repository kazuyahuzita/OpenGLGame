#include "GemsUI.h"
#include "Engine/UILayout.h"

void GemsUI::Awake()
{
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	
	for (int i = 0;i < number; i++)
	{
		//ƒWƒFƒ€‚ÌUI‚ðÝ’è
		auto gem = engine->CreateUIObject<UILayout>("Res/UI_PlayGame/Gem.tga", "gemsUI", {1.6f-0.16f * i, 0.9f}, 0.075f);
		gemObject = gem.first;
		gemObject->renderQueue = RenderQueue_overlay;
		num[i] = gemObject;
	}
	
}

void GemsUI::Update(float deltaTime)
{
	auto owner = GetOwner();

	if (isHitPlayer)num[0]->color.w = 1.0f;
	else num[0]->color.w = 0.2f;

	if (isHitPlayer1)num[1]->color.w = 1.0f;
	else num[1]->color.w = 0.2f;

	if (isHitPlayer2)num[2]->color.w = 1.0f;
	else num[2]->color.w = 0.2f;

	if (isHitPlayer3)num[3]->color.w = 1.0f;
	else num[3]->color.w = 0.2f;

	if (isHitPlayer4)num[4]->color.w = 1.0f;
	else num[4]->color.w = 0.2f;
}

