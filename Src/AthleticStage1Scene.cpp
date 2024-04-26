#include "AthleticStage1Scene.h"
//���̃I�u�W�F�N�g�w�b�_�[����
#include "AthleticStage2Scene.h"
#include "AudioSettings.h"
#include "ExplorationScene.h"
#include "FirstPersonCamera.h"
#include "GameObjectSettingsJSON.h"
#include "Gate.h"
#include "GateEffect.h"
#include "ImageSettings.h"
#include "MeasureTime.h"
#include "MoveExplorationScene.h"
#include "MoveStage2Scene.h"
#include "TextBlock.h"
#include "Tornado.h"
//Engine�o�R�̋@�\�Ɋւ���w�b�_�[����
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/PlayerComponent.h"
#include "Engine/UIButton.h"


/**
* �V�[���̏�����
*/
bool AthleticStage1Scene::Initialize(Engine& engine)
{
	//�����̐ݒ�F�S�̓I�ɖ��邭���Ă���
	engine.ambientLight = vec3(0.50f, 0.50f, 0.50f);

	// �Q�[���I�u�W�F�N�g�z�u�t�@�C����ǂݍ���
	auto jsonobject = engine.Create<GameObject>("object", "object", {0,0,0});
	gameObjectSettingsjson = jsonobject->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->AthleticStage1MapLoad();
	// �v���C���[��z�u
	auto player = engine.Create<GameObject>("player", "player", { -1.5f, 3.0f, -5.5f },{0,0.0f,0});
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
	measureTimeComponent->SetCountTime1(3);

	//�A�X���`�b�N�V�[���ɑJ�ڗp�̃I�u�W�F�N�g
	auto meScene =
		engine.Create<GameObject>("exploration", "exploration", { -5.9f, 5.6f,4.0f });
	moveExplorationScene = meScene->AddComponent<MoveExplorationScene>();
	//�A�X���`�b�N�V�[���ɑJ�ڗp�̃I�u�W�F�N�g
	auto ms2Scene =
		engine.Create<GameObject>("Stage2", "Stage2", { -5.9f, 5.6f,7.0f });
	moveStage2Scene = ms2Scene->AddComponent<MoveStage2Scene>();

	//�S�[���̒��O�ɐݒu���Ă���S�[�����ۂ����o���Ă���I�u�W�F�N�g
	auto tornado =
		engine.Create<GameObject>("tornado", "tornado", { -5.6f, 5.6f,4.0f });
	tornadoComponent = tornado->AddComponent<TornadoEffect>();
	auto tornado1 =
		engine.Create<GameObject>("tornado", "tornado", { -5.6f, 5.6f,7.0f });
	tornadoComponent1 = tornado1->AddComponent<TornadoEffect>();

	// �l�N�X�g�V�[���̐����̕\��
	auto textBlock =
		engine.Create<GameObject>("next", "next", { -5.2f, 5.6f,7.0f });
	textBlockComponent = textBlock->AddComponent<TextBlock>();

	// �V�[����߂�Ƃ��̐����̕\��
	auto textBlock1 =
		engine.Create<GameObject>("back", "back", { -5.2f, 5.6f,4.0f });
	textBlockComponent1 = textBlock1->AddComponent<TextBlock>();

	// �X�J�C�X�t�B�A��ݒ�
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::skysphere);

	// BGM���Đ�
	EasyAudio::Play(AudioPlayer::bgm, BGM_Athletic::playScene, engine.MusicVolume - 0.2f, true);

	return true;
}

void AthleticStage1Scene::Update(Engine& engine, float deltaTime)
{

	//case���ŕ�����Ƃ����Ⴒ���Ⴕ�Ă��܂����߁A�����ď���
	// ��ԂɑΉ�����X�V�֐������s
	state(this, engine, deltaTime);

	if (fadeTimer > 0) {
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		fadeTimer -= 0.5f*deltaTime;
		//�p�b�Ɣ����Ȃ��āA�����҂��Ă���V�[���Ɉړ�����`��
		FadeObject->color.w += 1.0f*deltaTime;
		 
		// �^�C�}�[��0�ȉ��ɂȂ�����Q�[���V�[���ɐ؂�ւ���
		//bool�łǂ̃V�[���ɍs�����͂킩��Ă���.
		if (fadeTimer <= 0.0f && isExploration)
		{
			engine.SetNextScene<ExplorationScene>();

		}
		else if (fadeTimer <= 0.0f && isAthleticStage1)
		{
			engine.SetNextScene<AthleticStage1Scene>();

		}
		else if (fadeTimer <= 0.0f && isAthleticStage2)
		{
			engine.SetNextScene<AthleticStage2Scene>();

		}
		else if (fadeTimer <= 0.0f && isTimeOut) {
			engine.SetNextScene<AthleticStage1Scene>();
		}

	} // if fadeTimer
}

void AthleticStage1Scene::Finalize(Engine& engine)
{
	//�����Ő��������I�u�W�F�N�g�̍폜
	//�����̃X�g�b�v������A���ꂼ��Ńv���C���[�ԍ����Ⴄ�̂ł��ꂼ�����������w�肷��
	EasyAudio::Stop(AudioPlayer::bgm);
	EasyAudio::Stop(AudioPlayer::se);
	EasyAudio::Stop(AudioPlayer::se1);
	engine.ClearGameObjectAll();
}

void AthleticStage1Scene::State_Playing(Engine& engine, float deltaTime)
{
	// �����ݒ�	 
	auto measureTime = measureTimeComponent->GetOwner();
	auto meScene = moveExplorationScene->GetOwner();
	auto ms2Scene = moveStage2Scene->GetOwner();
	auto player = playerComponent->GetOwner();
	auto tornado = tornadoComponent->GetOwner();
	auto tornado1 = tornadoComponent1->GetOwner();
	auto textBlock = textBlockComponent->GetOwner();
	auto textBlock1 = textBlockComponent1->GetOwner();

	//�S�[���I�u�W�F�N�g�Ƃ��Ĕ������q���΂��Ă���
	tornadoComponent->SetState(TornadoEffect::TornadoEffectState::alive);
	tornadoComponent1->SetState(TornadoEffect::TornadoEffectState::alive);

	//AthleticScene2�Ɉړ�����Ƃ��ɉ摜���v���C���[���߂Â�����o���Ă����悤�ɂ��Ă���
	if (textBlockComponent->IsPlayerInRange(player->position.x, player->position.z, 1.5f))
		textBlockComponent->SetIsHitPlayer4(true);
	else
		textBlockComponent->SetIsHitPlayer4(false);

	//ExplorationScene�ɖ߂�Ƃ��̉摜���v���C���[���߂Â�����o���Ă����悤�ɂ��Ă���
	if (textBlockComponent1->IsPlayerInRange(player->position.x, player->position.z, 1.5f))
		textBlockComponent1->SetIsHitPlayer5(true);
	else
		textBlockComponent1->SetIsHitPlayer5(false);

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
			retryButton.second->onClick.push_back(
				[this](UIButton* button) {
					fadeTimer = 1;				  //�V�[���J�ڂ̏������n�߂�
					isAthleticStage1 = true;      //������x�n�߂�ۂ̃t���O��On��
					button->interactable = false; // �{�^���𖳌���
					EasyAudio::PlayOneShot(SE::buttonClick);
				});

			//�o�b�N�{�^����ݒ肵�Ă���
			auto backButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::back_button, "Title", { 0.0f, -0.7f }, 0.12f);
			backButton.second->onClick.push_back(
				[this](UIButton* button)
				{
					fadeTimer = 1;				  //�V�[���J�ڂ̏������n�߂�
					isExploration = true;		  //ExplorationScene�ɔ�΂��t���O��On��
					button->interactable = false; // �{�^���𖳌���
					EasyAudio::PlayOneShot(SE::buttonClick);

				});

		}//if fadeTimer
	}//	if playerComponent->GetPosition().y <= -2.0f


	//�������Ԃ𒴂����Ƃ��ɃQ�[���I�[�o�[������������
	if (measureTimeComponent->GetIsCount())
	{
		//�v���C���[���A�X���`�b�N�V�[���ɑJ�ڂ�����
		if (fadeTimer <= 0)
		{
			fadeTimer = 1.0f;	//�V�[���J�ڂ̏������n�߂�
			isTimeOut = true;	//���g���C����t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			// �J���[���u���A�����v�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 0, 0.0f);
		}//if fadeTimer

	}//	if measureTimeComponent->GetIsCount()
	/*
	* �S�[���p�̑J�ڃt���O
	*/

	//ExplorationScene�ɖ߂�ۂ�if��
	if (moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside)
	{
		//�v���C���[���A�X���`�b�N�V�[���ɑJ�ڂ�����
		if (fadeTimer <= 0) {
			fadeTimer = 1.0;		//�V�[���J�ڂ̏������n�߂�
			isExploration = true;	//ExplorationScene�ɔ�΂��t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;

		}//if fadeTimer
	}//	if moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside

	//Stage2�ɐi�ގ���if��
	if (moveStage2Scene->GetState() == MoveStage2Scene::MoveStage2SceneState::Inside)
	{
		//�v���C���[���A�X���`�b�N�V�[���ɑJ�ڂ�����
		if (fadeTimer <= 0) {
			fadeTimer = 1.0;			//�V�[���J�ڂ̏������n�߂�
			isAthleticStage2 = true;	//AthleticStage2�ɔ�΂��t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			// �J���[���u���A�����v�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;
		}
	}//	if moveStage2Scene->GetState() == MoveStage2Scene::MoveStage2SceneState::Inside

}

void AthleticStage1Scene::State_GameOver(Engine& engine, float deltaTime)
{
	//��Ƀv���C���[���폜����
	auto player = playerComponent->GetOwner();
	player->Destroy();

}
