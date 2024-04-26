#include "UILayout.h"
 #include "Engine.h"

/**
* UI���C�A�E�g�̎��s���J�n����
*/
void UILayout::Start()
{
   // �ŏ��Ɏw�肳�ꂽ���W���u�J�������猩�����W�v�Ƃ���	
	basePosition = GetOwner()->position;	
}

/**
* �X�V�C�x���g
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void UILayout::Update(float deltaTime)
 {
	//��ɃJ�����̕��������悤�ɃQ�[���I�u�W�F�N�g�̌����𒲐�(�r���{�[�h�Ɠ���)
	GameObject * owner = GetOwner();
	const Engine * engine = owner->GetEngine();
	const GameObject & camera = engine->GetMainCamera();
	owner->rotation.y = camera.rotation.y;

	
	// ��ɃJ�����̐��ʂɈʒu����悤�ɃQ�[���I�u�W�F�N�g�̍��W�𒲐�
	// ��]�̌����ɂ���ăJ�����Ƃ̑��΍��W�����߂�
	vec3 pos = basePosition; // �p�x0�x�̏ꍇ�̍��W

  // Y���W��-1�`+1���A�J�����Ɏʂ�͈͂̏�[�Ɖ��[�ɂȂ�悤�ɁAZ���W�𒲐�
	pos.z = -engine->GetFovScale();

	const float s = sin(camera.rotation.y);
	const float c = cos(camera.rotation.y);
	pos = {
		pos.x * c + pos.z * s,
		pos.y,
		pos.x * -s + pos.z * c,
	};
	owner->position = camera.position + pos;
}