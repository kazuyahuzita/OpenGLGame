#include "HorrorItemUI.h"
#include "Engine/UILayout.h"

void HorrorItemUI::Awake()
{
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();

	const vec2 fbSize = engine->GetFramebufferSize();
	//アイテムのバッテリーのUIを設定
	auto battery = engine->CreateUIObject<UILayout>("Res/UI_PlayGame/Suspension.tga", "gemsUI", { 0.4f, -0.8f }, 0.15f);
	Battery = battery.first;
	Battery->renderQueue = RenderQueue_overlay + 1;
	//アイテムのガスカンのUIを設定
	auto gasCan = engine->CreateUIObject<UILayout>("Res/UI_PlayGame/gasoline.tga", "gemsUI", { 0.8f, -0.8f }, 0.15f);
	GasCan = gasCan.first;
	GasCan->renderQueue = RenderQueue_overlay + 1;
	//アイテムのバイクのシートのUIを設定
	auto seat = engine->CreateUIObject<UILayout>("Res/UI_PlayGame/Cover.tga", "gemsUI", { 1.2f, -0.8f }, 0.15f);
	Seat = seat.first;
	Seat->renderQueue = RenderQueue_overlay + 1;
	//アイテムのタイヤのUIを設定
	auto tyre = engine->CreateUIObject<UILayout>("Res/UI_PlayGame/tyre.tga", "gemsUI", { 1.6f, -0.8f }, 0.15f);
	Tyre = tyre.first;
	Tyre->renderQueue = RenderQueue_overlay + 1;
	//右下にあるアイテムを誇張させるための画像
	auto backShadow = engine->CreateUIObject<UILayout>("Res/Image/black.tga", "gemsUI", { 1.0f, -0.8f }, 1.0f);
	BackShadow = backShadow.first;
	BackShadow->color.w = 0.8f;
	BackShadow->scale = { 0.8f,0.2f,1 };
	BackShadow->renderQueue = RenderQueue_overlay;
}

void HorrorItemUI::Update(float deltaTime)
{
	auto owner = GetOwner();

	//それぞれの画像のalpha値の変更を専用のフラグで管理

	if (isHitBattery)Battery->color.w = 1.0f;
	else Battery->color.w = 0.4f;

	if (isHitGasCan)GasCan->color.w = 1.0f;
	else GasCan->color.w = 0.4f;

	if (isHitSeat)Seat->color.w = 1.0f;
	else Seat->color.w = 0.4f;

	if (isHitTyre)Tyre->color.w = 1.0f;
	else Tyre->color.w = 0.4f;

}

