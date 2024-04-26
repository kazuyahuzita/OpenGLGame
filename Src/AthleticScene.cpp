#define _CRT_SECURE_NO_WARNINGS

#include "AthleticScene.h"
//���̃I�u�W�F�N�g�w�b�_�[����
#include "AudioSettings.h"
#include "DartSmoke.h"
#include "ExplorationScene.h"
#include "FireEmitter.h"
#include "FirstPersonCamera.h"
#include "FrontBackBoard.h"
#include "GameObjectSettingsJSON.h"
#include "Gate.h"
#include "GateEffect.h"
#include "GemsUI.h"
#include "ImageSettings.h"
#include "Item.h"
#include "ItemBlock.h"
#include "MoveBlock.h"
#include "MoveEnemy.h"
#include "MoveExplorationScene.h"
#include "NormalEnemy.h"
#include "TitleScene.h"

//Engine�o�R�̋@�\�Ɋւ���w�b�_�[����
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/PlayerComponent.h"
#include "Engine/UIButton.h"


/**
* �V�[���̏�����
*/
bool AthleticScene::Initialize(Engine& engine)
{
	engine.ambientLight = vec3(0.50f, 0.50f, 0.50f);

	// Json�t�@�C���ŃQ�[���I�u�W�F�N�g�z�u�t�@�C����ǂݍ���
	auto jsonobject = engine.Create<GameObject>("jsonObject", "jsonObject", {0,0,0});
	gameObjectSettingsjson = jsonobject->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->AthleticSceneMapLoad();
		
	/*
	* ���ۂ̃v���C��ʂŐݒu�����Object�̐ݒu
	*/
	
	//�W�F���p��UI�̔z�u
	auto gemsUI = engine.Create<GameObject>("gemUI", "gemUI",{0,0,0});
	gemsUIComponent = gemsUI->AddComponent<GemsUI>();

	//=== �Q�[�g���o�̍ۂɕK�v�ȃI�u�W�F�N�g�B ��===//
	
	//�y���̕\��
	auto dartEffect =
		engine.Create<GameObject>("dart", "dart", { 62.0f, 3.5f, -72.5f },{0,0,0});
	dartSmokeEffectComponent =dartEffect->AddComponent<DartSmokeEffect>();

	// �Q�[�g�̕\��
	auto gate = 
		engine.Create<GameObject>("gate", "gate", { 62.0f, -0.3f, -72.5f },{0,30,0});
	gateComponent =gate->AddComponent<Gate>();

	//�Q�[�g�̃G�t�F�N�g�̕\��
	auto gateEffect =
		engine.Create<GameObject>("gateEffect", "gateEffect", { 61.5f, 1.5f, -72.5f }, { 0,320,0 });
	gateEffectComponent= gateEffect->AddComponent<GateEffect>();

	//�T���V�[���ɖ߂��I�u�W�F�N�g
	auto meScene =
		engine.Create<GameObject>("exploration", "exploration", { 60.5f, 12.0f, -72.5f }, { 0,30,0 });
	moveExplorationScene = meScene->AddComponent<MoveExplorationScene>();

	//=== �Q�[�g���o�̍ۂɕK�v�ȃI�u�W�F�N�g�B ��===//

	//=== �ǐՂ���G�l�~�[�̕\����s���ɕK�v�ȃI�u�W�F�N�g�B ��===//

	//�A�C�e���u���b�N��@�����ƂŁA�G�l�~�[������
	auto itemBlock = engine.Create<GameObject>("itemBlock", "itemBlock", { 4.0f, 3.0f, -4.5f });
	itemBlockComponent = itemBlock->AddComponent<ItemBlock>();

	// �ǂ������Ă���G�l�~�[�̕\��
	auto trackingEnemy =
		engine.Create<GameObject>("trackingEnemy", "enemy", { 2.5f, -3.0f, -12.5f });
	trackingEnemyComponent = trackingEnemy->AddComponent<MoveEnemy>();
	trackingEnemy->scale = {1.5f,1.5f,1.5f};
	trackingEnemy->staticMesh = engine.GetStaticMesh("Res/MeshData/Athletic/Face/Face.obj");

	//=== �Q�[�g���o�̍ۂɕK�v�ȃI�u�W�F�N�g�B ��===//

	// �����������G�l�~�[�̕\��
	auto normalEnemy = 
		engine.Create<GameObject>("KnockbackEnemy", "KnockbackEnemy", { 2.5f, 0.5f, -4.5f });
	normalEnemyComponent =normalEnemy->AddComponent<NormalEnemy>();

	// �v���C���[��z�u
	auto player = engine.Create<GameObject>("player", "player", { 0.0f, 2.0f, 0.0f });
	playerComponent = player->AddComponent<PlayerComponent>();
	player->AddComponent<FirstPersonCamera>();

	// �X�J�C�X�t�B�A��ݒ�
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::skysphere);

	// BGM���Đ�
	EasyAudio::Play(AudioPlayer::bgm, BGM_Athletic::playScene, engine.MusicVolume - 0.2f, true);

	return true;
}
/**
* �V�[�����X�V����
*/
void AthleticScene::Update(Engine& engine, float deltaTime)
{
	//case���ŕ�����Ƃ����Ⴒ���Ⴕ�Ă��܂����߁A�����ď���
	// ��ԂɑΉ�����X�V�֐������s
	state(this, engine, deltaTime);

	if (fadeTimer > 0) {
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		fadeTimer -= deltaTime;
		FadeObject->color[3] = 1 - fadeTimer;

		// �^�C�}�[��0�ȉ��ɂȂ��Ă��Ή�����t���O�������΃V�[����؂�ւ���
		//bool�łǂ̃V�[���ɍs�����͂킩��Ă���.
		if (fadeTimer <= 0.0f && isExploration) {
			engine.SetNextScene<ExplorationScene>();

		}
		else if (fadeTimer <= 0.0f && isAthleticScene)
		{
			engine.SetNextScene<AthleticScene>();

		}

	} // if fadeTimer

	//�v���C���[�����񂾂璼��ExplorationScene�ɖ߂�悤�ɂ��Ă���
	if (playerComponent->GetState() == PlayerComponent::PlayerState::dead)
	{
		engine.SetNextScene<TitleScene>();
	}

}
/**
* �V�[�����I������
*
*/
void AthleticScene::Finalize(Engine& engine)
{
	//�����Ő��������I�u�W�F�N�g�̍폜
	//�����̃X�g�b�v������A���ꂼ��Ńv���C���[�ԍ����Ⴄ�̂ł��ꂼ�����������w�肷��
	EasyAudio::Stop(AudioPlayer::bgm);
	EasyAudio::Stop(AudioPlayer::se);
	EasyAudio::Stop(AudioPlayer::se1);
	engine.ClearGameObjectAll();
}

/**
* �u�v���C���v��Ԃ̃V�[�����X�V����
*
* @param engine    �Q�[���G���W��
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void AthleticScene::State_Playing(Engine& engine, float deltaTime)
{
	// �����ݒ�	 
	auto dartEffect = dartSmokeEffectComponent->GetOwner();
	auto gateEffect = gateEffectComponent->GetOwner();
	auto gate = gateComponent->GetOwner();
	auto gemsUI = gemsUIComponent->GetOwner();
	auto itemBlock = itemBlockComponent->GetOwner();
	auto trackingEnemy = trackingEnemyComponent->GetOwner();
	auto normalEnemy = normalEnemyComponent->GetOwner();
	auto meScene = moveExplorationScene->GetOwner();
	auto player = playerComponent->GetOwner();

	//�v���C���[�̃��X�^�[�g
	//�v���C���[�̃|�W�V���������l�������ɂȂ����烊�X�^�[�g����悤��
	if (playerComponent->GetPosition().y <= -2.0f)
	{

		if (fadeTimer <= 0)
		{
			//player�̓������~�߂Ă���
			playerComponent->SetState(PlayerComponent::PlayerState::stop);
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>(Image_Fade::white, "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;

			//�Q�[���I�[�o�[�̕����\��
			auto gameOverLogo =
				engine.CreateUIObject<UILayout>(Image_Athletic::over_logo, "Title", { 0, 0.4f }, 0.2f);
			gameOverLogo.second->color.w = 0;
			//���g���C�̕����\��
			auto retryButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::retry_button, "Title", { 0.0f, -0.4f }, 0.12f);
			// �{�^���������ꂽ�烁�C���Q�[���V�[���ɐ؂�ւ��郉���_����ݒ�
			retryButton.second->onClick.push_back(
				[this](UIButton* button) {
					fadeTimer = 1;			//�V�[���J�ڂ̏������n�߂�
					isAthleticScene = true;	//������x�n�߂�ۂ̃t���O��On��
					button->interactable = false; // �{�^���𖳌���
					EasyAudio::PlayOneShot(SE::buttonClick);
				});
			//�o�b�N�̕����\��
			auto backButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::back_button, "Title", { 0.0f, -0.7f }, 0.12f);
			backButton.second->onClick.push_back(
				[this](UIButton* button)
				{
					fadeTimer = 1;			//�V�[���J�ڂ̏������n�߂�
					isExploration = true;	//Exploration�ɖ߂�ۂ̃t���O��On��
					button->interactable = false; // �{�^���𖳌���
					EasyAudio::PlayOneShot(SE::buttonClick);

				});

		}// if fadeTimer
	}	//if playerComponent->GetPosition().y <= -2.0f


	//itemBlock�ɐG�ꂽ�画�肪On�ɂȂ�
	//�ǐՌ^�̃G�l�~�[�̒a������
	if (itemBlockComponent->isActive)
	{
		trackingEnemy->position.y = 1.0f;
		trackingEnemyComponent->Tracking(player->position, deltaTime);
	}

	//�v���C���[���W�����v��ɐG�ꂽ�������ɐ�����΂�����
	if (playerComponent->GetState() == PlayerComponent::PlayerState::jumpinghit)
	{
		for (int i = 0; i < 5; i++)
		{
			playerComponent->Jumping(deltaTime);
		}
		if (player->isGrounded)
			playerComponent->SetState(PlayerComponent::PlayerState::alive);
	}//if playerComponent->GetState() == PlayerComponent::PlayerState::jumpinghit


	//UI�Ŏ������ǂꂾ�����Ă��邩��\��
	//�E���珇�ɃA�C�e�����ǉ�����Ă���悤��
	if (playerComponent->GetPoint() == 2)gemsUIComponent->SetIsHitPlayer(true);
	if (playerComponent->GetPoint() == 4)gemsUIComponent->SetIsHitPlayer1(true);
	if (playerComponent->GetPoint() == 6)gemsUIComponent->SetIsHitPlayer2(true);
	if (playerComponent->GetPoint() == 8)gemsUIComponent->SetIsHitPlayer3(true);

	//�v���C���[���W�F�����Q�b�g���ă|�C���g��10�ɂȂ�����
	//�S�[���̖傪�o������d�l��
	
	if (playerComponent->GetPoint() >= 10)
	{
		//�W�F��UI�̉摜��alpha�l��1��
		gemsUIComponent->SetIsHitPlayer4(true);
		//�Q�[�g�{�̂̐ݒ�
		gateComponent->SetState(Gate::GateState::alive);
		//�Q�[�g�������ɂł�X���[�N�̐ݒ�
		dartSmokeEffectComponent->SetState(DartSmokeEffect::DartSmokeEffectState::alive);
		gateEffectComponent->SetState(GateEffect::GateEffectState::alive);

		//�^�C�~���O�̌v��
		timer++;
		if (timer > 60)
		{
			timerCount++;
			timer = 0;
		}

		//�^�C�~���O�����x�����^�C�~���O�Ŕ����ł���悤�ɂ��Ă���
		if (timerCount >= 15)
		{
			gateEffect->position.y = 5.5f;
			meScene->position.y = 5.5f;
			dartSmokeEffectComponent->SetState(DartSmokeEffect::DartSmokeEffectState::dead);
		}

	}//if playerComponent->GetPoint() >= 10


	//ExplorationScene�ɑJ�ڂ�����ۂ̃t���O�ƃt�F�[�h�^�C�}�[��ݒ�

	if (moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside)
	{
		//�v���C���[���A�X���`�b�N�V�[���ɑJ�ڂ�����
		if (fadeTimer <= 0) {
			fadeTimer = 1.0;		//�V�[���J�ڂ̏������n�߂�
			isExploration = true;	//Exploration�ɖ߂�ۂ̃t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			// �J���[���u���A�����v�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>("Res/Image/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			FadeObject->color.w = 0.0f;
		}//if fadeTimer
	}//	if moveExplorationScene->GetState() == MoveExplorationScene::MoveExplorationSceneState::Inside

}
/**
* �u�Q�[���I�[�o�[�v��Ԃ̃V�[�����X�V����
*
* @param engine    �Q�[���G���W��
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void AthleticScene::State_GameOver(Engine& engine, float deltaTime)
{
	//��Ƀv���C���[���폜����
	auto player = playerComponent->GetOwner();
	player->Destroy();

}
