#ifndef RIDERSCENE_H_INCLUDED
#define RIDERSCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/Scene.h"
#include <functional>

// ��s�錾
class Rider;
class MoveEnemy;
class GameObjectSettingsJSON;
/**
* �t�B�[���h�T���V�[��
*/
class RiderScene : public Scene
{
public:
	RiderScene() = default;
	virtual ~RiderScene() = default;

	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;

	// �Q�[���̏��
	using State = std::function<void(RiderScene*, Engine&, float)>;
	State state = &RiderScene::State_Playing;

	// ��ԂɑΉ�����X�V�֐�
	void State_Playing(Engine& engine, float deltaTime);
	void State_GameOver(Engine& engine, float deltaTime);

private:
	GameObjectPtr FadeObject;		//	��ʂ̃V�[���J�ڗp�̃I�u�W�F�N�g

	std::shared_ptr<Rider> riderComponent;							//Rider�w�b�_�[�����锠
	std::shared_ptr<MoveEnemy> enemyComponent;						//MoveEnemy�w�b�_�[�����锠
	std::shared_ptr<GameObjectSettingsJSON> gameObjectSettingsjson;	//GameObjectSettingsJSON�w�b�_�[�����锠

	float fadeTimer = 0;		//�t�F�[�h�I�u�W�F�N�g�̕ω��̃X�s�[�h��ύX���Ă����^�C�}�[
	bool isBack = false;		//�Q�[�����I��������t���O
};

#endif // RIDERSCENE_H_INCLUDED