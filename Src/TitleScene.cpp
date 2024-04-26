#include "TitleScene.h"
//���̃I�u�W�F�N�g�w�b�_�[����
#include "AudioSettings.h"
#include "ExplorationScene.h"
#include "ImageSettings.h"
//Engine�o�R�̋@�\�Ɋւ���w�b�_�[����
#include "Engine/UILayout.h"
#include "Engine/Engine.h"
#include "Engine/UIButton.h"
#include "Engine/Light.h"
#include "Engine/EasyAudio/EasyAudio.h"

/**
* �^�C�g����ʂ�����������
*
* @retval true  ����������
* @retval false ���������s
*/
bool TitleScene::Initialize(Engine& engine)
{
	  // ���C�g��z�u
	auto lightObject = engine.Create<GameObject>("light", "light",
		engine.GetMainCamera().position);
	auto light = lightObject->AddComponent<Light>();
	light->color = { 1.0f, 0.9f, 0.8f };
	light->intensity = 10;
	light->radius = 5;
	
	//�^�C�g���̔w�i��\�����Ă���
	auto b = engine.CreateUIObject<UILayout>(Image_Title::title_bg, "Title", { 0, 0 }, 1);
	b.first->renderQueue = RenderQueue_overlay ;

	//�^�C�g���̃��S��\�����Ă���
	auto l = engine.CreateUIObject<UILayout>(Image_Title::title_logo, "Title", { 0, 0.4f }, 0.2f);
	l.first->renderQueue = RenderQueue_overlay ;

	//�i�ލۂ̃{�^��
	auto startButton = 
		engine.CreateUIObject<UIButton>(Image_Title::start_button,"Start", { 0.0f, -0.4f }, 0.12f);
	startButton.first->renderQueue = RenderQueue_overlay ;

	//�߂�ۂ̃{�^��
	auto QuitButton = 
		engine.CreateUIObject<UIButton>(Image_Title::quit_button, "Quit", { 0.0f, -0.7f }, 0.12f);
	QuitButton.first->renderQueue = RenderQueue_overlay ;

	//�i�ލۂ̃{�^���������ꂽ�Ƃ��̏���
	startButton.second->onClick.push_back(
		[this](UIButton* button) {
			fadeTimer = 1;					//�V�[���J�ڂ̏������n�߂�
			button->interactable = false;	// �{�^���𖳌���
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
			isFinished = true;				//�Q�[�����I��������
			button->interactable = false;	// �{�^���𖳌���
			if (!isClick)
			{
				EasyAudio::PlayOneShot(SE::buttonClick);
				isClick = true;
			}

		});
	
	// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
	auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
	fadeObject = fade.first;
	// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
	const vec2 fbSize = engine.GetFramebufferSize();
	fadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	// �J���[���u���A�����v�ɐݒ�
	std::fill_n(&fadeObject->color.x, 4, 0.0f);

	// BGM���Đ�
	EasyAudio::Play(AudioPlayer::bgm, BGM_Exploration::title, engine.MusicVolume, true);

	return true; // ����������
}

/**
* �^�C�g����ʂ̍X�V����
*
* @param engine    �Q�[���G���W��
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void TitleScene::Update(Engine& engine, float deltaTime)
{
	if (fadeTimer > 0) {
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		fadeTimer -= deltaTime * 2;
		fadeObject->color.w = 1 - fadeTimer;

		// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
		auto Loading = engine.CreateUIObject<UILayout>(Image_Fade::nowLoading, "Loading", { 0.8f, -0.8f }, 0.1f);
		Loading.first->color.w = 1 - fadeTimer;

		// �^�C�}�[��0�ȉ��ɂȂ����烁�C���Q�[���V�[���ɐ؂�ւ���
		if (fadeTimer <= 0.0f) {
			engine.SetNextScene<ExplorationScene>();

		}

	} // if fadeTimer

	if (isFinished)
	{
		glfwTerminate();
	}
}
/**
* �^�C�g����ʂ̏I������
*/
void TitleScene::Finalize(Engine& engine)
{
	//�����Ő��������I�u�W�F�N�g�̍폜
	//�����̃X�g�b�v������A���ꂼ��Ńv���C���[�ԍ����Ⴄ�̂ł��ꂼ�����������w�肷��
	engine.ClearGameObjectAll();
	EasyAudio::Stop(AudioPlayer::bgm);
}

