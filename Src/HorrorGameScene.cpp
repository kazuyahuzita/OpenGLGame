#include "HorrorGameScene.h"
//���̃I�u�W�F�N�g�w�b�_�[����
#include "AudioSettings.h"
#include "Bike.h"
#include "ExplorationScene.h"
#include "FirstPersonCamera.h"
#include "FluorescentLight.h"
#include "GameObjectSettingsJSON.h"
#include "HorrorDeathScene.h"
#include "HorrorItemUI.h"
#include "ImageSettings.h"
#include "Item.h"
#include "MoveEnemy.h"
#include "RiderScene.h"
#include "TitleScene.h"
//Engine�o�R�̋@�\�Ɋւ���w�b�_�[����
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/PlayerComponent.h"
#include "Engine/UIButton.h"

/**
* �V�[���̏�����
*/
bool HorrorGameScene::Initialize(Engine& engine)
{
	//�����̐ݒ�F�S�̓I�ɈÂ������Ă���
	engine.ambientLight = vec3(0.10f,0.10f,0.10f);

	// �Q�[���I�u�W�F�N�g�z�u�t�@�C����ǂݍ���
	auto jsonobject = engine.Create<GameObject>("object", "object", { 0,0,0 });
	gameObjectSettingsjson = jsonobject->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->SubMainGameSceneMapLoad();

	// �v���C���[��z�u
	auto player = engine.Create<GameObject>("player","player", { 30, 3.0f, -30 }, {0,-90,0});
	playerComponent = player->AddComponent<PlayerComponent>();
	player->AddComponent<FirstPersonCamera>();
	//�v���C���[�̎��E��傫�ȋ��Ő���
	auto PlayerSphere = 
		engine.Create<GameObject>("playerspehe", "playerspehe");
	PlayerSphere->scale = vec3{0.5f,0.5f,0.5f};
	PlayerSphere->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/PlayerSphere/PlayerSphere.obj");
	PlayerSphere->SetParent(player);

	// �悵���̕\��
	auto enemy = engine.Create<GameObject>("enemy", "enemy",{0,1.8f,0});
	enemyComponent = enemy->AddComponent<MoveEnemy>();
	enemy->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Face/Yoshie.obj");

	// �ł������G�l�~�[��\��
	auto enemy1 = engine.Create<GameObject>("enemy1", "enemy1",{0,-30.0f,0});
	enemyComponent1 = enemy1->AddComponent<MoveEnemy>();
	enemy1->scale = vec3{ 0.5f,0.5f,0.5f };
	enemy1->staticMesh = engine.GetStaticMesh("Res/MeshData/Spawned/SpawnerEnemy.obj");

	//�o�C�N�̔z�u
	auto bike = engine.Create<GameObject>("motor", "motor", { 0, 0.4f, -28 });
	BikeComponent = bike->AddComponent<Bike>();

	//�A�C�e���p��UI�̔z�u
	auto horrorItemUI = engine.Create<GameObject>("horrorItemUI", "horrorItemUI", { 0,0,0 });
	horrorItemUIComponent = horrorItemUI->AddComponent<HorrorItemUI>();

	//�^�񒆉E�ɃA�C�e���̔z�u
	auto battery = engine.Create<GameObject>("Item", "battery");
	battery->AddComponent<GemItem>();
	battery->position = { 45,6.5f,-30 };
	battery->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Item/Battery.obj");
	//�E��ɃA�C�e���̔z�u
	auto Gas_Can = engine.Create<GameObject>("Item", "Gas_Can");
	Gas_Can->AddComponent<GemItem>();
	Gas_Can->position = { 35,6.4f,-65 };
	Gas_Can->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Item/Gas_Can.obj");
	//����ɃA�C�e���̔z�u
	auto seat = engine.Create<GameObject>("Item", "seat");
	seat->AddComponent<GemItem>();
	seat->position = { 14,0.2f,-50 };
	seat->scale = { 2,2,2 };
	seat->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Item/Seat.obj");
	//�����ɃA�C�e���̔z�u
	auto tyre = engine.Create<GameObject>("Item", "tyre");
	tyre->AddComponent<GemItem>();
	tyre->position = { -4,1.0f,-13 };
	tyre->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Item/Tyre.obj");

	// �X�J�C�X�t�B�A��ݒ�
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::graysphere);

	// �摜��\��
	//�ڕW�Ƃ���������\�����Ă���
	auto Target = engine.CreateUIObject<UILayout>(Image_Horror::target, "target", { -1.6f, 0.9f }, 0.07f);
	//�ڕW�̒��g�������Ă���.
	auto Search = engine.CreateUIObject<UILayout>(Image_Horror::search, "Search", { -0.65f, 0.9f }, 0.07f);
	// BGM���Đ�
	EasyAudio::Play(AudioPlayer::bgm, BGM_Horror::playScene, engine.MusicVolume - 0.2f, true);

	return true;
}

/**
* �V�[�����X�V����
*/
void HorrorGameScene::Update(Engine& engine, float deltaTime)
{
	//case���ŕ�����Ƃ����Ⴒ���Ⴕ�Ă��܂����߁A�����ď���
	// ��ԂɑΉ�����X�V�֐������s
	state(this, engine, deltaTime);

	if (fadeTimer > 0) {
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		fadeTimer -= deltaTime;
		FadeObject->color[3] = 1 - fadeTimer;

		// �^�C�}�[��0�ȉ��ɂȂ�����Q�[���V�[���ɐ؂�ւ���
		//bool�łǂ̃V�[���ɍs�����͂킩��Ă���.
		if (fadeTimer <= 0.0f&&isRiderScene) {
			engine.SetNextScene<RiderScene>();

		}
		else if (fadeTimer <= 0.0f && isExploration)
		{
			engine.SetNextScene<ExplorationScene>();

		}
		else if (fadeTimer <= 0.0f && isHorrorGameScene)
		{
			engine.SetNextScene<HorrorGameScene>();

		}

	} // if fadeTimer

	//�v���C���[���G�ɐG�ꂽ�瑦���Ɏ��S�V�[���ɐ؂�ւ��Ă���
	if (playerComponent->GetState() == PlayerComponent::PlayerState::dead)
	{
		engine.SetNextScene<HorrorDeathScene>();
	}

	//��߂�{�^������������Q�[�����I������悤�ɂ��Ă���
	if (isBack)
	{
		glfwTerminate();
	}
}

/**
* �V�[�����I������
* 
*/
void HorrorGameScene::Finalize(Engine& engine)
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
void HorrorGameScene::State_Playing(Engine& engine, float deltaTime)
{

	/*			 *
	 * �����ݒ�	 * 
	 *			 */	
	auto bike = BikeComponent->GetOwner();
	auto enemy = enemyComponent->GetOwner();
	auto enemy1 = enemyComponent1->GetOwner();
	auto horrorItemUI = horrorItemUIComponent->GetOwner();
	auto player = playerComponent->GetOwner();

	//�W�����v�������E�ςƍ���Ȃ�����
	//�v���C���[�̃W�����v��������
	playerComponent->SetJump(true);

	//�v���C���[�̃��X�^�[�g
	if (playerComponent->GetPosition().y <= -2.0f)
	{

		if (fadeTimer <= 0)
		{
			//player�̓����������Ȃ����Ă���
			playerComponent->SetState(PlayerComponent::PlayerState::stop);
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
					fadeTimer = 1;
					isHorrorGameScene = true;
					button->interactable = false; // �{�^���𖳌���
					EasyAudio::PlayOneShot(SE::buttonClick);
				});

			//�o�b�N�{�^����ݒ肵�Ă���
			auto backButton =
				engine.CreateUIObject<UIButton>(Image_Athletic::back_button, "Title", { 0.0f, -0.7f }, 0.12f);
			backButton.second->onClick.push_back(
				[this](UIButton* button)
				{
					fadeTimer = 1;
					isExploration = true;
					button->interactable = false; // �{�^���𖳌���
					EasyAudio::PlayOneShot(SE::buttonClick);

				});

		}//if fadeTimer
	}//if playerComponent->GetPosition().y <= -2.0f

	//�n�߂����̒ǐՃL������ǉ�
	enemyComponent->Tracking(player->position,deltaTime);

	//�v���C���[�Ƃ̋������߂��Ȃ����特����悤�ɂ��Ă���
	if (enemyComponent->IsPlayerInRange(player->position.x, player->position.z, 12.0f)) 
	{
		//���x�������Đ������Ȃ��悤�ɂ��Ă���
		if (EasyAudio::IsPlaying(AudioPlayer::se))
		{
			return;
		}
		//�ʂ̒ǐ�SE������Ă����痬���Ȃ�
		if (!isTrackingSE1)
		{
			// �ǐ�SE���Đ�
			EasyAudio::Play(AudioPlayer::se, SE::Stoke, engine.MusicVolume - 0.2f, true);
			isTrackingSE = true;
		}

	}
	else
	{
		isTrackingSE = false;
		EasyAudio::Stop(AudioPlayer::se);
	}//if enemyComponent->IsPlayerInRange(player->position.x, player->position.z, 12.0f)


	/*										 *
	 * �v���C���[�̏󋵂ɂ��ω�����ݒ�	 *
	 *										 */
	//�Ή�����A�C�e�������ꂽ�炻�̕����̃A�C�e���摜��alpha�l��1��
	if(playerComponent->GetSeatItem())horrorItemUIComponent->SetIsHitSeat(true);
	if(playerComponent->GetBatteryItem())horrorItemUIComponent->SetIsHitBattery(true);
	if(playerComponent->GetTyreItem())horrorItemUIComponent->SetIsHitTyre(true);
	//�L�[�A�C�e������������̑Ώ��@
	if (playerComponent->GetKeyItem())
	{
		horrorItemUIComponent->SetIsHitGasCan(true);
		//�G�l�~�[�o��
		enemy1->position.y = 1.5f;
		//�ǐՃG�l�~�[�𑝂₷
		enemyComponent1->Tracking(player->position, deltaTime);
	}//if playerComponent->GetKeyItem()

	//�v���C���[�Ƃ̋������߂��Ȃ����特����悤�ɂ��Ă���
	if (enemyComponent1->IsPlayerInRange(player->position.x, player->position.z, 12.0f))
	{
		if (EasyAudio::IsPlaying(AudioPlayer::se1))
		{
			return;
		}
		//�ʂ̒ǐ�SE������Ă����痬���Ȃ�
		if (!isTrackingSE)
		{
			isTrackingSE1 = true;
			EasyAudio::Play(AudioPlayer::se1, SE::Stoke, engine.MusicVolume - 0.2f, true);
		}

	}
	else
	{
		isTrackingSE1 = false;
		EasyAudio::Stop(AudioPlayer::se1);
	}//if enemyComponent1->IsPlayerInRange(player->position.x, player->position.z, 12.0f)

	////�A�C�e������萔�l��������A�S�[���Ƃ���
	if (playerComponent->GetPoint() == 8)
	{
		if (!isDrive) {
		isDrive = true;

		}
	}

	/*			�@ *
	 * �V�[���J�ځ@*
	 *			�@ */
	// �o�C�N�ɂӂ�ď��
	if (isDrive)
	{
		if (fadeTimer <= 0) {
			//�G�l�~�[���΂��ăo�O�点�Ȃ��悤�ɏ���
			enemy->position = { 12,0,-25 };
			enemy1->position = { 24,0,-25 };
			fadeTimer = 1;			 //�V�[���J�ڂ̏������n�߂�
			isRiderScene = true;	 //RiderScene�ɑJ�ڂ�����t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			// �J���[���u���A�����v�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>("Res/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay+10;

			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 4, 0.0f);
		}//if fadeTimer
	}//if isDrive
	
	/*			�@	�@ *
	 * �Q�[���I�������@*
	 *			�@ �@�@*/

	//esc�L�[�������ꂽ�Ƃ��̑Ώ��@
	//���̂Ƃ����߂邱�Ƃ����ł��Ȃ�
	if (playerComponent->GetESC())
	{
		//�v���C���[�������Ȃ��悤��
		playerComponent->SetState(PlayerComponent::PlayerState::stop);

		// �x���摜��\��
		engine.CreateUIObject<UILayout>("Res/Image/BackWindow.tga", "Back", { 0, 0 }, 0.25f);

		// �͂��{�^����\��
		auto Yesbutton = engine.CreateUIObject<UIButton>(
			"Res/Image/Yes.tga", "Yes", { -0.5f, -0.5f }, 0.1f);
		Yesbutton.second->onClick.push_back([this](UIButton* button)
			{
				EasyAudio::PlayOneShot(SE::buttonClick);
				isBack = true;
			});
		// �������{�^����\��
		auto Nobutton = engine.CreateUIObject<UIButton>(
			"Res/Image/No.tga", "No", { 0.5f,  -0.5f }, 0.1f);
		Nobutton.second->onClick.push_back([this](UIButton* button)
			{
				button->interactable = false; // �{�^���𖳌���
				EasyAudio::PlayOneShot(SE::missClick);

			});

	}//if playerComponent->GetESC()
}

/**
* �u�Q�[���I�[�o�[�v��Ԃ̃V�[�����X�V����
*
* @param engine    �Q�[���G���W��
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void HorrorGameScene::State_GameOver(Engine& engine, float deltaTime)
{
	//���player���폜���Ă���
	auto player = playerComponent->GetOwner();
	player->Destroy();

}

