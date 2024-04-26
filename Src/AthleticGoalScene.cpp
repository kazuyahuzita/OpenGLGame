#include "AthleticGoalScene.h"
//���̃I�u�W�F�N�g�w�b�_�[����
#include "AudioSettings.h"
#include "ImageSettings.h"
#include "ExplorationScene.h"
//Engine�o�R�̋@�\�Ɋւ���w�b�_�[����
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/Engine.h"
#include "Engine/Light.h"
#include "Engine/UIButton.h"
#include "Engine/UILayout.h"
/**
* ����������
*
* @retval true  ����������
* @retval false ���������s
*/
bool AthleticGoalScene::Initialize(Engine& engine)
{
	  // ���C�g��z�u
	auto lightObject = engine.Create<GameObject>("light","light",
		engine.GetMainCamera().position);
	auto light = lightObject->AddComponent<Light>();
	light->color = { 1.0f, 0.9f, 0.8f };
	light->intensity = 10;
	light->radius = 5;
	
	//�Q�[���N���A�̔w�i��\�����Ă���
	auto b = engine.CreateUIObject<UILayout>(Image_Athletic::clear_bg,"Clear", {0, 0}, 1);
	b.first->renderQueue = RenderQueue_overlay ;
	//�Q�[���N���A�̃��S��\�����Ă���
	auto l = engine.CreateUIObject<UILayout>(Image_Athletic::clear_logo, "Clear", { 0, 0.4f }, 0.3f);
	l.first->renderQueue = RenderQueue_overlay ;
	//�Q�[���N���A���Đi�ލۂ̃{�^��
	auto nextButton = 
		engine.CreateUIObject<UIButton>(Image_Athletic::back_button, "Back", { 0, -0.6f }, 0.2f);
	nextButton.first->renderQueue = RenderQueue_overlay ;
	//nextButton�������ꂽ��̏���
	nextButton.second->onClick.push_back(
		[this](UIButton* button) {
			fadeTimer = 1;				  //�V�[���J�ڂ̏������n�߂�
			button->interactable = false; // �{�^���𖳌���
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

	//void Play(�Đ��Ɏg���v���C���[�ԍ�, �����t�@�C����, ����, ���[�v�t���O);
	// BGM���Đ�
	EasyAudio::Play(AudioPlayer::bgm, BGM_Athletic::clear, engine.MusicVolume, true);

	return true; // ����������
}

/**
* �^�C�g����ʂ̍X�V����
*
* @param engine    �Q�[���G���W��
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void AthleticGoalScene::Update(Engine& engine, float deltaTime)
{
	if (fadeTimer > 0) {
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		fadeTimer -= deltaTime;
		fadeObject->color.w = 1 - fadeTimer;

		// �^�C�}�[��0�ȉ��ɂȂ����烁�C���Q�[���V�[���ɐ؂�ւ���
		if (fadeTimer <= 0) {
			engine.SetNextScene<ExplorationScene>();

		}

	} // if fadeTimer
}
/**
* �^�C�g����ʂ̏I������
*/
void AthleticGoalScene::Finalize(Engine& engine)
{
	//�����Ő��������I�u�W�F�N�g�̍폜
	//�����̃X�g�b�v������A���ꂼ��Ńv���C���[�ԍ����Ⴄ�̂ł��ꂼ�����������w�肷��
	engine.ClearGameObjectAll();
	EasyAudio::Stop(AudioPlayer::bgm);
}