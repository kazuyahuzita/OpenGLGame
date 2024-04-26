#include "HorrorTitleScene.h"
//���̃I�u�W�F�N�g�w�b�_�[����
#include "AudioSettings.h"
#include "ExplorationScene.h"
#include "HorrorGameScene.h"
#include "ImageSettings.h"
//Engine�o�R�̋@�\�Ɋւ���w�b�_�[����
#include "Engine/UILayout.h"
#include "Engine/Engine.h"
#include "Engine/UIButton.h"
#include "Engine/Light.h"
#include "Engine/EasyAudio/EasyAudio.h"

/**
* ����������
*
* @retval true  ����������
* @retval false ���������s
*/
bool HorrorTitleScene::Initialize(Engine& engine)
{
	  // ���C�g��z�u
	auto lightObject = engine.Create<GameObject>("light", "light",
		engine.GetMainCamera().position);
	auto light = lightObject->AddComponent<Light>();
	light->color = { 1.0f, 0.9f, 0.8f };
	light->intensity = 10;
	light->radius = 5;
	
	//�z���[�Q�[���^�C�g���̔w�i��\�����Ă���
	auto b = engine.CreateUIObject<UILayout>(Image_Horror::title_bg, "Title", { 0, 0 }, 1);
	b.first->renderQueue = RenderQueue_overlay ;

	//�i�ލۂ̃{�^��
		auto startButton = 
		engine.CreateUIObject<UIButton>(Image_Horror::start_button,"Start", { 0.9f, -0.5f }, 0.12f);
	startButton.first->renderQueue = RenderQueue_overlay ;

	//�߂�ۂ̃{�^��
	auto QuitButton = 
		engine.CreateUIObject<UIButton>(Image_Horror::back_button, "Quit", { 0.9f, -0.7f }, 0.12f);
	QuitButton.first->renderQueue = RenderQueue_overlay ;

	//�i�ލۂ̃{�^���������ꂽ�Ƃ��̏���
	startButton.second->onClick.push_back(
		[this](UIButton* button) {
			fadeTimer = 1;
			isMove = true;
			button->interactable = false; // �{�^���𖳌���
			if (!isClick)
			{
				EasyAudio::PlayOneShot(SE::buttonClick);
				isClick = true;
			}
		});
	//�߂�ۂ̃{�^���������ꂽ�Ƃ��̏���
	QuitButton.second->onClick.push_back(
		[this](UIButton* button)
		{
			fadeTimer = 1;
			isNotMove = true;
			button->interactable = false; // �{�^���𖳌���
			if (!isClick)
			{
				EasyAudio::PlayOneShot(SE::buttonClick);
				isClick = true;
			}

		});
	
	//�t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
	auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
	fadeObject = fade.first;
	// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
	const vec2 fbSize = engine.GetFramebufferSize();
	fadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	// �J���[���u���A�����v�ɐݒ�
	std::fill_n(&fadeObject->color.x, 4, 0.0f);

	//void Play(�Đ��Ɏg���v���C���[�ԍ�, �����t�@�C����, ����, ���[�v�t���O);
	// BGM���Đ�
	EasyAudio::Play(AudioPlayer::bgm, BGM_Horror::title, engine.MusicVolume, true);

	return true; // ����������
}

/**
* �^�C�g����ʂ̍X�V����
*
* @param engine    �Q�[���G���W��
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void HorrorTitleScene::Update(Engine& engine, float deltaTime)
{
	if (fadeTimer > 0) {
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		fadeTimer -= deltaTime * 2;
		fadeObject->color.w = 1 - fadeTimer;

		// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g�̏�ŉ摜��\�����Ă���
		auto Loading = engine.CreateUIObject<UILayout>(Image_Fade::nowLoading, "Loading", { 0.8f, -0.8f }, 0.1f);
		Loading.first->color.w = 1 - fadeTimer;

		// �^�C�}�[��0�ȉ��ɂȂ����烁�C���Q�[���V�[���ɐ؂�ւ���
		//bool�łǂ̃V�[���ɍs�����͂킩��Ă���.
		if (fadeTimer <= 0.0f && isMove) {
			engine.SetNextScene<HorrorGameScene>();
		}
		else if (fadeTimer <= 0.0f && isNotMove)
		{
			engine.SetNextScene<ExplorationScene>();
		}

	} // if fadeTimer

}
/**
* �^�C�g����ʂ̏I������
*/
void HorrorTitleScene::Finalize(Engine& engine)
{
	//�����Ő��������I�u�W�F�N�g�̍폜
	//�����̃X�g�b�v������A���ꂼ��Ńv���C���[�ԍ����Ⴄ�̂ł��ꂼ�����������w�肷��
	engine.ClearGameObjectAll();
	EasyAudio::Stop(AudioPlayer::bgm);
}

