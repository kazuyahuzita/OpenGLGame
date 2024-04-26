#include "HorrorItemUI.h"
#include "Engine/UILayout.h"

void HorrorItemUI::Awake()
{
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();

	const vec2 fbSize = engine->GetFramebufferSize();
	//�A�C�e���̃o�b�e���[��UI��ݒ�
	auto battery = engine->CreateUIObject<UILayout>("Res/UI_PlayGame/Suspension.tga", "gemsUI", { 0.4f, -0.8f }, 0.15f);
	Battery = battery.first;
	Battery->renderQueue = RenderQueue_overlay + 1;
	//�A�C�e���̃K�X�J����UI��ݒ�
	auto gasCan = engine->CreateUIObject<UILayout>("Res/UI_PlayGame/gasoline.tga", "gemsUI", { 0.8f, -0.8f }, 0.15f);
	GasCan = gasCan.first;
	GasCan->renderQueue = RenderQueue_overlay + 1;
	//�A�C�e���̃o�C�N�̃V�[�g��UI��ݒ�
	auto seat = engine->CreateUIObject<UILayout>("Res/UI_PlayGame/Cover.tga", "gemsUI", { 1.2f, -0.8f }, 0.15f);
	Seat = seat.first;
	Seat->renderQueue = RenderQueue_overlay + 1;
	//�A�C�e���̃^�C����UI��ݒ�
	auto tyre = engine->CreateUIObject<UILayout>("Res/UI_PlayGame/tyre.tga", "gemsUI", { 1.6f, -0.8f }, 0.15f);
	Tyre = tyre.first;
	Tyre->renderQueue = RenderQueue_overlay + 1;
	//�E���ɂ���A�C�e�����֒������邽�߂̉摜
	auto backShadow = engine->CreateUIObject<UILayout>("Res/Image/black.tga", "gemsUI", { 1.0f, -0.8f }, 1.0f);
	BackShadow = backShadow.first;
	BackShadow->color.w = 0.8f;
	BackShadow->scale = { 0.8f,0.2f,1 };
	BackShadow->renderQueue = RenderQueue_overlay;
}

void HorrorItemUI::Update(float deltaTime)
{
	auto owner = GetOwner();

	//���ꂼ��̉摜��alpha�l�̕ύX���p�̃t���O�ŊǗ�

	if (isHitBattery)Battery->color.w = 1.0f;
	else Battery->color.w = 0.4f;

	if (isHitGasCan)GasCan->color.w = 1.0f;
	else GasCan->color.w = 0.4f;

	if (isHitSeat)Seat->color.w = 1.0f;
	else Seat->color.w = 0.4f;

	if (isHitTyre)Tyre->color.w = 1.0f;
	else Tyre->color.w = 0.4f;

}

