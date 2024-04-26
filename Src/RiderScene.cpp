#include "RiderScene.h"
//���̃I�u�W�F�N�g�w�b�_�[����
#include "AudioSettings.h"
#include "Bike.h"
#include "FirstPersonCamera.h"
#include "GameObjectSettingsJSON.h"
#include "GoalObject.h"
#include "HorrorDeathScene.h"
#include "HorrorGoalScene.h"
#include "ImageSettings.h"
#include "MoveEnemy.h"
#include "TitleScene.h"
//Engine�o�R�̋@�\�Ɋւ���w�b�_�[����
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/Rider.h"
#include "Engine/UIButton.h"

bool RiderScene::Initialize(Engine& engine)
{
	// �Q�[���I�u�W�F�N�g�z�u�t�@�C����ǂݍ���
	auto object = engine.Create<GameObject>("object", "object", { 0,0,0 });
	gameObjectSettingsjson = object->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->RiderSceneMapLoad();

	//�g���l���̒��g���Ƃ炷�X�|�b�g���C�g0
	vec3 pos = { 24,0.8f,-75 };
	auto lightObject = engine.Create<GameObject>("light", "light", pos, { 0,120,180 });
	lightObject->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Light/Light.obj");
	auto light = lightObject->AddComponent<Light>();
	light->color = { 0.95f, 1.0f, 0.98f };
	light->intensity = 150;
	light->radius = 50;
	light->type = Light::Type::spotLight;

	//�g���l���̒��g���Ƃ炷�X�|�b�g���C�g1
	auto lightObject1 = engine.Create<GameObject>("light", "light", { 16.5,0.8f,-85 }, { 0,50,180 });
	lightObject1->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Light/Light.obj");
	auto light1 = lightObject1->AddComponent<Light>();
	light1->color = { 0.95f, 1.0f, 0.98f };
	light1->intensity = 150;
	light1->radius = 50;
	light1->type = Light::Type::spotLight;

	//�g���l���̒��g���Ƃ炷�X�|�b�g���C�g2
	auto lightObject2 = engine.Create<GameObject>("light", "light", { 10,0.8f,-60 }, { 0,30,180 });
	lightObject2->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/Light/Light.obj");
	auto light2 = lightObject2->AddComponent<Light>();
	light2->color = { 0.95f, 1.0f, 0.98f };
	light2->intensity = 150;
	light2->radius = 50;
	light2->type = Light::Type::spotLight;

	// �v���C���[��z�u
	auto rider = engine.Create<GameObject>("Rider", "Rider", { 22, 1.6f, -20 }, { 0,0,0 });
	riderComponent = rider->AddComponent<Rider>();
	rider->AddComponent<FirstPersonCamera>();
	//�v���C���[�̎��E�𐧌�����I�u�W�F�N�g
	auto PlayerSphere =
		engine.Create<GameObject>("playerspehe", "playerspehe");
	PlayerSphere->staticMesh = engine.GetStaticMesh("Res/MeshData/DeathForest/PlayerSphere/PlayerSphere.obj");
	PlayerSphere->SetParent(rider);

	// �G�l�~�[�̕\��
	auto enemy = engine.Create<GameObject>("enemy", "enemy");
	enemy->AddComponent<MoveEnemy>();
	enemyComponent = enemy->AddComponent<MoveEnemy>();

	//�S�[���I�u�W�F�N�g�̐ݒu
	auto goal = engine.Create<GameObject>("goal", "goal");
	goal->AddComponent<GoalObject>();

	// �摜��\��
	auto Target =engine.CreateUIObject<UILayout>(Image_Horror::target, "target", { -1.6f, 0.9f }, 0.07f);
	auto Search =engine.CreateUIObject<UILayout>(Image_Horror::goal, "goal", { -1.1f, 0.9f }, 0.07f);

	// �X�J�C�X�t�B�A��ݒ�
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::graysphere);

	// BGM���Đ�
	EasyAudio::Play(AudioPlayer::bgm, BGM_Horror::riderScene, engine.MusicVolume-0.2f, true);

	return true;
}

void RiderScene::Update(Engine& engine, float deltaTime)
{
	//case���ŕ�����Ƃ����Ⴒ���Ⴕ�Ă��܂����߁A�����ď���
	// ��ԂɑΉ�����X�V�֐������s
	state(this, engine, deltaTime);

	//�v���C���[���N���A�����璼���ɃV�[�����ړ�
	if (fadeTimer > 0)
	{
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		fadeTimer -= deltaTime;
		FadeObject->color[3] = 1 - fadeTimer ;

		// �^�C�}�[��0�ȉ��ɂȂ�����Q�[���V�[���ɐ؂�ւ���
		if (fadeTimer <= 0.94f) {
			engine.SetNextScene<HorrorGoalScene>();

		}

	} // if fadeTimer

	//esc�L�[�ŏI��
	if (isBack)
	{
		glfwTerminate();
	}

}

void RiderScene::Finalize(Engine& engine)
{
	//�����Ő��������I�u�W�F�N�g�̍폜
	//�����̃X�g�b�v������A���ꂼ��Ńv���C���[�ԍ����Ⴄ�̂ł��ꂼ�����������w�肷��
	EasyAudio::Stop(AudioPlayer::bgm);
	EasyAudio::Stop(AudioPlayer::se);
	EasyAudio::Stop(AudioPlayer::se1);
	engine.ClearGameObjectAll();
}

void RiderScene::State_Playing(Engine& engine, float deltaTime)
{
	//�����ݒ�
	auto rider = riderComponent->GetOwner();
	auto enemy = enemyComponent->GetOwner();

	//�G�l�~�[�Ƀv���C���[�̈ʒu��n��
	enemyComponent->Tracking(rider->position, deltaTime);

	// �v���C���[������ł�����A�Q�[���I�[�o�[�V�[���Ɉړ�
	if (riderComponent->GetState() == Rider::RiderState::dead)
	{
		// �Q�[���I�[�o�[�摜��\��
		engine.SetNextScene<HorrorDeathScene>();
	}

	// �v���C���[���S�[��������A�N���A�V�[���Ɉړ�
	if (riderComponent->GetState() == Rider::RiderState::goal)
	{
		if (fadeTimer <= 0)
		{
			//�d�Ȃ�Ȃ��悤�Ɉʒu��߂�.
			enemy->position = { 12,0,-25 };
			fadeTimer = 1;		//�V�[���J�ڂ̏������n�߂�

			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			// �J���[���u���A�����v�ɐݒ�
			auto goalfade = engine.CreateUIObject<UILayout>("Res/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = goalfade.first;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 4, 0.0f);
		}// if fadeTimer
	}//if riderComponent->GetState() == Rider::RiderState::goal

	//esc�L�[�������ꂽ�Ƃ��̑Ώ��@
	if (riderComponent->GetESC())
	{
		riderComponent->SetState(Rider::RiderState::stop);

		// �x���摜��\��
		engine.CreateUIObject<UILayout>("Res/Image/BackWindow.tga", "Back", { 0, 0 }, 0.25f);

		// �͂��{�^����\��
		auto Yesbutton = engine.CreateUIObject<UIButton>(
			"Res/Image/Yes.tga", "Yes", { -0.5f,  -0.5f }, 0.1f);
		Yesbutton.second->onClick.push_back([this](UIButton* button)
			{
				EasyAudio::PlayOneShot(SE::buttonClick);
				isBack = true;		//�Q�[�����I�����鏈����i�߂�
			});
		// �������{�^����\��
		auto Nobutton = engine.CreateUIObject<UIButton>(
			"Res/Image/No.tga", "No", { 0.5f,  -0.5f }, 0.1f);
		Nobutton.second->onClick.push_back([this](UIButton* button)
			{
				button->interactable = false; // �{�^���𖳌���
				EasyAudio::PlayOneShot(SE::missClick);

			});

	}//if riderComponent->GetESC() == true

}

void RiderScene::State_GameOver(Engine& engine, float deltaTime)
{
	//��Ƀv���C���[���폜����
	auto rider = riderComponent->GetOwner();
	rider->Destroy();

}
