#include "ExplorationScene.h"
//���̃I�u�W�F�N�g�w�b�_�[����
#include "AthleticScene.h"
#include "AthleticStage1Scene.h"
#include "AthleticTitleScene.h"
#include "AudioSettings.h"
#include "FirstPersonCamera.h"
#include "GameObjectSettingsJSON.h"
#include "Gate.h"
#include "GateEffect.h"
#include "HorrorTitleScene.h"
#include "ImageSettings.h"
#include "MoveAthleticScene.h"
#include "MoveHorrorScene.h"
#include "MoveStage1Scene.h"
#include "TextBlock.h"
#include "TitleScene.h"
#include "Tornado.h"
//Engine�o�R�̋@�\�Ɋւ���w�b�_�[����
#include "Engine/EasyAudio/EasyAudio.h"
#include "Engine/PlayerComponent.h"

/**
* �V�[���̏�����
*/
bool ExplorationScene::Initialize(Engine& engine)
{
	//�����̐ݒ�F�S�̓I�ɖ��邭���Ă���
	engine.ambientLight = vec3(0.50f, 0.50f, 0.50f);

	// �Q�[���I�u�W�F�N�g�z�u�t�@�C����ǂݍ���
	auto jsonobject = engine.Create<GameObject>("object", "object", {0,0,0});
	gameObjectSettingsjson = jsonobject->AddComponent<GameObjectSettingsJSON>();
	gameObjectSettingsjson->ExplorationSceneMapLoad();
	// �v���C���[��z�u
	auto player = engine.Create<GameObject>("player", "player", { 2.5f, 1.5f, 4.5f });
	playerComponent = player->AddComponent<PlayerComponent>();
	player->AddComponent<FirstPersonCamera>();

	// �Q�[�g�̕\��
	auto gate =
		engine.Create<GameObject>("gate", "gate", { 5.0f, -0.3f, -5.5f }, { 0,200,0 });
	gateComponent = gate->AddComponent<Gate>();
	auto gate1 =
		engine.Create<GameObject>("gate1", "gate1", { 4.0f, -0.3f, -19.5f }, { 0,130,0 });
	gateComponent = gate1->AddComponent<Gate>();
	auto gate2 =
		engine.Create<GameObject>("gate2", "gate2", { -7.0f, -0.3f, -23.5f }, { 0,20,0 });
	gateComponent = gate2->AddComponent<Gate>();
	auto gate3 =
		engine.Create<GameObject>("gate3", "gate3", { 17.0f, -0.3f, -42.5f }, { 0,30,0 });
	gateComponent = gate3->AddComponent<Gate>();

	//�Q�[�g�̃p�[�e�B�N���\��
	auto gateEffect =
		engine.Create<GameObject>("gateEffect", "gateEffect", { 5.0f, 1.5f, -5.5f }, { 0,10,0 });
	gateEffectComponent = gateEffect->AddComponent<GateEffect>();
	auto gateEffect1 =
		engine.Create<GameObject>("gateEffect1", "gateEffect1", { 4.0f, 1.5f, -19.8f }, { 0,120,0 });
	gateEffectComponent1 = gateEffect1->AddComponent<GateEffect>();
	auto gateEffect2 =
		engine.Create<GameObject>("gateEffect2", "gateEffect2", { -7.0f, 1.5f, -23.5f }, { 0,120,0 });
	gateEffectComponent2 = gateEffect2->AddComponent<GateEffect>();
	auto gateEffect3 =
		engine.Create<GameObject>("gateEffect3", "gateEffect3", { 17.0f, 1.5f, -42.5f }, { 0,120,0 });
	gateEffectComponent3 = gateEffect3->AddComponent<GateEffect>();

	//�A�X���`�b�N�V�[���ɑJ�ڗp�̃I�u�W�F�N�g
	auto moveathleticScene =
		engine.Create<GameObject>("athletic", "athletic", { 17.0f, 1.5f, -41.9f }, { 0,0,0 });
	moveAthleticScene = moveathleticScene->AddComponent<MoveAthleticScene>();

	//�z���[�V�[���ɑJ�ڗp�̃I�u�W�F�N�g
	auto movehorrorScene =
		engine.Create<GameObject>("horror", "horror", { 4.0f, 1.5f, -19.2f }, { 0,0,0 });
	moveHorrorScene = movehorrorScene->AddComponent<MoveHorrorScene>();

	//�Q�[���V�[���ɑJ�ڗp�̃I�u�W�F�N�g
	auto movestage1Scene =
		engine.Create<GameObject>("stage1", "stage1", { 5.0f, 1.5f, -5.2f }, { 0,0,0 });
	moveStage1Scene = movestage1Scene->AddComponent<MoveStage1Scene>();

	// �Q�[�g�O�̐����̕\��
	auto textBlock =
		engine.Create<GameObject>("stage1", "stage1", { 4.0f, 1.5f, -4.8f }, { 0,30,0 });
	textBlockComponent =textBlock->AddComponent<TextBlock>();
	// �Q�[�g�O�̐����̕\��
	auto textBlock1 =
		engine.Create<GameObject>("stage2", "stage2", {2.0f, 1.5f, -20.0f }, { 0,50,0 });
	textBlockComponent1 =textBlock1->AddComponent<TextBlock>();
	// �Q�[�g�O�̐����̕\��
	auto textBlock2 =
		engine.Create<GameObject>("stage3", "stage3", { -5.5f, 1.5f, -23.0f }, { 0,20,0 });
	textBlockComponent2 =textBlock2->AddComponent<TextBlock>();
	// �Q�[�g�O�̐����̕\��
	auto textBlock3 =
		engine.Create<GameObject>("stage4", "stage4", { 15.0f, 1.5f, -42.0f }, { 0,30,0 });
	textBlockComponent3 =textBlock3->AddComponent<TextBlock>();

	// �X�J�C�X�t�B�A��ݒ�
	skysphereMaterial = std::make_shared<Material>();
	skysphereMaterial->baseColor = { 1,1,1,1 };
	skysphereMaterial->texBaseColor = engine.GetTexture(Image_Sphere::skysphere);

	// BGM���Đ�
	EasyAudio::Play(AudioPlayer::bgm, BGM_Exploration::playScene, engine.MusicVolume - 0.2f, true);

	return true;
}

void ExplorationScene::Update(Engine& engine, float deltaTime)
{

	//case���ŕ�����Ƃ����Ⴒ���Ⴕ�Ă��܂����߁A�����ď���
	// ��ԂɑΉ�����X�V�֐������s
	state(this, engine, deltaTime);

	if (fadeTimer > 0) {
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		fadeTimer -= deltaTime;
		//�p�b�ƈÓ]���āA�����҂��Ă���V�[���Ɉړ�����`��
		FadeObject->color[3] = std::min((1 - fadeTimer)*10, 1.0f);
		 
		// �^�C�}�[��0�ȉ��ɂȂ�����Q�[���V�[���ɐ؂�ւ���
		//bool�łǂ̃V�[���ɍs�����͂킩��Ă���.

		if (fadeTimer <= 0.0f&&isAthletic) {
			engine.SetNextScene<AthleticScene>();
		}
		else if (fadeTimer <= 0.0f&&isHorror) {
			engine.SetNextScene<HorrorTitleScene>();
		}
		else if (fadeTimer <= 0.0f&&isStage1) {
			engine.SetNextScene<AthleticTitleScene>();
		}

	} // if FadeTimer
}

void ExplorationScene::Finalize(Engine& engine)
{
	//�����Ő��������I�u�W�F�N�g�̍폜
	//�����̃X�g�b�v������A���ꂼ��Ńv���C���[�ԍ����Ⴄ�̂ł��ꂼ�����������w�肷��
	EasyAudio::Stop(AudioPlayer::bgm);
	EasyAudio::Stop(AudioPlayer::se);
	EasyAudio::Stop(AudioPlayer::se1);

	engine.ClearGameObjectAll();

}

void ExplorationScene::State_Playing(Engine& engine, float deltaTime)
{
	//�����ݒ�
	auto player = playerComponent->GetOwner();
	auto gate = gateEffectComponent->GetOwner();
	auto gate1 = gateEffectComponent1->GetOwner();
	auto gate2 = gateEffectComponent2->GetOwner();
	auto gate3 = gateEffectComponent3->GetOwner();
	auto movestage1Scene = moveStage1Scene->GetOwner();
	auto movehorrorScene = moveHorrorScene->GetOwner();
	auto moveathleticScene = moveAthleticScene->GetOwner();
	auto textBlock = textBlockComponent->GetOwner();
	auto textBlock1 = textBlockComponent1->GetOwner();
	auto textBlock2 = textBlockComponent2->GetOwner();
	auto textBlock3 = textBlockComponent3->GetOwner();

	//�Q�[�g�̃G�t�F�N�g�Ƃ��Ĕ������q���΂��Ă���
	gateEffectComponent->SetState(GateEffect::GateEffectState::alive);
	gateEffectComponent1->SetState(GateEffect::GateEffectState::alive);
	gateEffectComponent2->SetState(GateEffect::GateEffectState::alive);
	gateEffectComponent3->SetState(GateEffect::GateEffectState::alive);

	//��̑O�ɐݒu���Ă���������I�u�W�F�N�g
	if(textBlockComponent->IsPlayerInRange(player->position.x, player->position.z, 2.0f))
		textBlockComponent->SetIsHitPlayer(true);
	else
		textBlockComponent->SetIsHitPlayer(false);

	if(textBlockComponent1->IsPlayerInRange(player->position.x, player->position.z, 2.0f))
		textBlockComponent1->SetIsHitPlayer1(true);
	else
		textBlockComponent1->SetIsHitPlayer1(false);

	if(textBlockComponent2->IsPlayerInRange(player->position.x, player->position.z, 2.0f))
		textBlockComponent2->SetIsHitPlayer2(true);
	else
		textBlockComponent2->SetIsHitPlayer2(false);

	if(textBlockComponent3->IsPlayerInRange(player->position.x, player->position.z, 2.0f))
		textBlockComponent3->SetIsHitPlayer3(true);
	else
		textBlockComponent3->SetIsHitPlayer3(false);

	/*
	* ���K�V�[���J�ڂ�������
	*/
	if (moveAthleticScene->GetState() == MoveAthleticScene::MoveAthleticSceneState::Inside)
	{
		//�v���C���[���A�X���`�b�N�V�[���ɑJ�ڂ�����
		if (fadeTimer <= 0) 
		{
			fadeTimer = 1.0f;		//�V�[���J�ڂ̏������n�߂�
			isAthletic = true;		//�A�X���`�b�N�V�[���̃t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			// �J���[���u���A�����v�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>("Res/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 0, 0.0f);
		}//if fadeTimer
	}//if moveAthleticScene->GetState() == MoveAthleticScene::MoveAthleticSceneState::Inside
	/*
	* �z���[�Q�[���V�[���ɑJ�ڂ�����
	*/
	if (moveHorrorScene->GetState() == MoveHorrorScene::MoveHorrorSceneState::Inside)
	{
		//�v���C���[���z���[�V�[���ɑJ�ڂ�����
		if (fadeTimer <= 0) {
			fadeTimer = 1.0f;	//�V�[���J�ڂ̏������n�߂�
			isHorror = true;	//�z���[�V�[���̃t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			// �J���[���u���A�����v�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>("Res/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 0, 0.0f);
		}//if fadeTimer

	}//if moveHorrorScene->GetState() == MoveHorrorScene::MoveHorrorSceneState::Inside
	/*
	* Stage1�V�[���J�ڂ�������
	*/
	if (moveStage1Scene->GetState() == MoveStage1Scene::MoveStage1SceneState::Inside)
	{
		//�v���C���[��Stage1�̃V�[���ɑJ�ڂ�����
		if (fadeTimer <= 0) {
			fadeTimer = 1.0f;		//�V�[���J�ڂ̏������n�߂�
			isStage1 = true;		//Stage1�̃V�[���̃t���O��On��
			// �t�F�[�h�A�E�g�pUI�I�u�W�F�N�g
			// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
			// �J���[���u���A�����v�ɐݒ�
			auto fade = engine.CreateUIObject<UILayout>("Res/white.tga", "fade", { 0, 0 }, 1);
			FadeObject = fade.first;
			FadeObject->renderQueue = RenderQueue_overlay + 10;
			const vec2 fbSize = engine.GetFramebufferSize();
			FadeObject->scale = { fbSize.x / fbSize.y, 1, 1 };
			std::fill_n(&FadeObject->color.x, 0, 0.0f);
		}//if fadeTimer
	}//if moveStage1Scene->GetState() == MoveStage1Scene::MoveStage1SceneState::Inside

}

void ExplorationScene::State_GameOver(Engine& engine, float deltaTime)
{
	//���Player���폜����
	auto player = playerComponent->GetOwner();
	player->Destroy();

}
