#include "AthleticStage3Scene.h"
//���̃I�u�W�F�N�g�w�b�_�[����
#include "AthleticGoalScene.h"
#include "AudioSettings.h"
#include "ExplorationScene.h"
#include "FirstPersonCamera.h"
#include "GameObjectSettingsJSON.h"
#include "Gate.h"
#include "GateEffect.h"
#include "ImageSettings.h"
#include "MeasureTime.h"
#include "MoveExplorationScene.h"
#include "TextBlock.h"
#include "Tornado.h"

//Engine�o�R�̋@�\�Ɋւ���w�b�_�[����
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/PlayerComponent.h"
#include "Engine/UIButton.h"

/**
* �V�[���̏�����
*/
bool AthleticStage3Scene::Initialize(Engine& engine)
{
	//�����̐ݒ�F�S�̓I�ɖ��邭���Ă���
	engine.ambientLight = vec3(0.50f, 0.50f, 0.50f);

	// �Q�[���I�u�W�F�N�g�z�u�t�@�C����ǂݍ���
	auto jsonobject = engine.Create<GameObject>("object", "object", {0,0,0});
	gameObjectSettingsjson = jsonobject->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->AthleticStage3MapLoad();

	// �v���C���[��z�u
	auto player = engine.Create<GameObject>("player", "player", { 0, 10.0f, 0 },{0,0.0f,0});
	player->rotation.y = -radians(90);
	playerComponent = player->AddComponent<PlayerComponent>();
	player->AddComponent<FirstPersonCamera>();

	//���Ԑ�����݂��邽�߂̃I�u�W�F�N�g
	auto measureTime =
		engine.Create<GameObject>("timer", "timer", { 0.0f, 0.0f, 0.0f });
	measureTimeComponent = measureTime->AddComponent<MeasureTime>();
	//���Ԑ����̐ݒ���X�e�[�W���ƂɌ��߂Ă���
	measureTimeComponent->SetState(MeasureTime::MeasureTimeState::alive);
	measureTimeComponent->SetCountTime(1);
	measureTimeComponent->SetCountTime1(9);

	//�X�e�[�W�Z���N�g�V�[���ɑJ�ڗp�̃I�u�W�F�N�g
	auto meScene =
		engine.Create<GameObject>("exploration", "exploration", { 40.0f, 14.5f,20.0f });
	moveExplorationScene = meScene->AddComponent<MoveExplorationScene>();

	//�g���l�[�h�G�t�F�N�g�̃I�u�W�F�N�g
	auto tornado =
		engine.Create<GameObject>("tornado", "tornado", { 40.3f, 14.5f,20.0f });
	tornadoComponent = tornado->AddComponent<TornadoEffect>();

	// �l�N�X�g�V�[���̐����̕\��
	auto textBlock =
		engine.Create<GameObject>("next", "next", { 40.7f, 14.5f,20.0f });
	textBlockComponent = textBlock->AddComponent<TextBlock>();

	// �X�J�C�X�t�B�A��ݒ�
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::skysphere);

	// BGM���Đ�
	EasyAudio::Play(AudioPlayer::bgm, BGM_Athletic::playScene, engine.MusicVolume - 0.2f, true);

	return true;
}

void AthleticStage3Scene::Update(Engine& engine, float deltaTime)
{

	//case���ŕ�����Ƃ����Ⴒ���Ⴕ�Ă��܂����߁A�����ď���
	// ��ԂɑΉ�����X�V�֐������s
	state(this, engine, deltaTime);

	if (fadeTimer > 0) {
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		fadeTimer -= 0.5f*deltaTime;
		//�p�b�ƈÓ]���āA�����҂��Ă���V�[���Ɉړ�����`��
		FadeObject->color.w += 1.0f*deltaTime;
		 
		// �^�C�}�[��0�ȉ��ɂȂ�����Q�[���V�[���ɐ؂�ւ���
		//bool�łǂ̃V�[���ɍs�����͂킩��Ă���.
		if (fadeTimer <= 0.0f && isGoalScene)
		{
			engine.SetNextScene<AthleticGoalScene>();

		}
		if (fadeTimer <= 0.0f && isAthleticStage3)
		{
			engine.SetNextScene<AthleticStage3Scene>();

		}
		else if (fadeTimer <= 0.0f && isTimeOut) {
			engine.SetNextScene<AthleticStage3Scene>();
		}

	} // if FadeTimer
}

void AthleticStage3Scene::Finalize(Engine& engine)
{
	//�����Ő��������I�u�W�F�N�g�̍폜
	//�����̃X�g�b�v������A���ꂼ��Ńv���C���[�ԍ����Ⴄ�̂ł��ꂼ�����������w�肷��
	EasyAudio::Stop(AudioPlayer::bgm);
	EasyAudio::Stop(AudioPlayer::se);
	EasyAudio::Stop(AudioPlayer::se1);
	engine.ClearGameObjectAll();

}

void AthleticStage3Scene::State_Playing(Engine& engine, float deltaTime)
{
	//�����ݒ�
	auto measureTime = measureTimeComponent->GetOwner();
	auto meScene = moveExplorationScene->GetOwner();
	auto player = playerComponent->GetOwner();
	auto tornado = tornadoComponent->GetOwner();
	auto textBlock = textBlockComponent->GetOwner();

	//�S�[���I�u�W�F�N�g�Ƃ��Ĕ������q���΂��Ă���
	tornadoComponent->SetState(TornadoEffect::TornadoEffectState::alive);

	//ExplorationScene�ɖ߂�Ƃ��̉摜���v���C���[���߂Â�����o���Ă����悤�ɂ��Ă���
	if (textBlockComponent->IsPlayerInRange(player->position.x, player->position.z, 1.5f))
		textBlockComponent->SetIsHitPlayer5(true);
	else
		textBlockComponent->SetIsHitPlayer5(false);


	//�v���C���[�̃��X�^�[�g
	if (playerComponent->GetPosition().y <= -2.0f)
	{

		if (fadeTimer <= 0)
		{
			//player�̓����������Ȃ����Ă���
			playerComponent->SetState(PlayerComponent::PlayerState::stop);
			//�������Ԃ��~�߂Ă���
			measureTimeComponent->SetState(MeasureTime::MeasureTimeState::dead);
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;

			//�Q�[���I�[�o�[�̃��S��\�����Ă����
			auto gameOverLogo =
				engine.CreateUIObject<UILayout>(Image_Athletic::over_logo, "Title", { 0, 0.4f }, 0.2f);
			gameOverLogo.second->color.w = 0;

			//���g���C�{�^����ݒ肵�Ă���
			auto retryButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::retry_button, "Title", { 0.0f, -0.4f }, 0.12f);
			// �{�^���������ꂽ�烁�C���Q�[���V�[���ɐ؂�ւ��郉���_����ݒ�
			retryButton.second->onClick.push_back(
				[this](UIButton* button) {
					fadeTimer = 1;				  //�V�[���J�ڂ̏������n�߂�
					isAthleticStage3 = true;	  //������x�n�߂�ۂ̃t���O��On��
					button->interactable = false; // �{�^���𖳌���
					EasyAudio::PlayOneShot(SE::buttonClick);
				});
			//�o�b�N�{�^����ݒ肵�Ă���
			auto backButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::back_button, "Title", { 0.0f, -0.7f }, 0.12f);
			backButton.second->onClick.push_back(
				[this](UIButton* button)
				{
					fadeTimer = 1;					//�V�[���J�ڂ̏������n�߂�
					isGoalScene = true;				//AthleticGoalScene�ɔ�΂��t���O��On��
					button->interactable = false;	// �{�^���𖳌���
					EasyAudio::PlayOneShot(SE::buttonClick);

				});

		} //if fadeTimer
	}	//if playerComponent->GetPosition().y <= -2.0f

	//�������Ԃ𒴂����Ƃ��ɃQ�[���I�[�o�[������������
	if (measureTimeComponent->GetIsCount())
	{
		//�v���C���[���A�X���`�b�N�V�[���ɑJ�ڂ�����
		if (fadeTimer <= 0)
		{
			fadeTimer = 1.0f;//�V�[���J�ڂ̏������n�߂�
			isTimeOut = true;//���g���C����t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			// �J���[���u���A�����v�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>("Res/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 0, 0.0f);
		} //if fadeTimer
	} //if measureTimeComponent->GetIsCount()

	/*
	* �S�[���p�̑J�ڃt���O
	*/

	//ExplorationScene�ɖ߂�ۂ�if��
	if (moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside)
	{
		//�v���C���[���A�X���`�b�N�V�[���ɑJ�ڂ�����
		if (fadeTimer <= 0) {
			fadeTimer = 1.0f;	//�V�[���J�ڂ̏������n�߂�
			isGoalScene = true;	//AthleticGoalScene�ɔ�΂��t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>("Res/Image/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;
		}//if fadeTimer
	}//if moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside
}

void AthleticStage3Scene::State_GameOver(Engine& engine, float deltaTime)
{
	//���player���폜����
	auto player = playerComponent->GetOwner();
	player->Destroy();

}
