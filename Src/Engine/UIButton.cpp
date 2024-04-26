#include "UIButton.h"
 #include "Engine.h"

/**
* �X�V�C�x���g
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void UIButton::Update(float deltaTime)
{
	// ���N���X�̃����o�֐������s
	UILayout::Update(deltaTime);
	
	GameObject * owner = GetOwner();
	Engine * engine = owner->GetEngine();
	
	
	  // �{�^���̗L���E��������
	if (!interactable)
	{
		std::copy_n(&normalColor.x, 3, &owner->color.x);
		owner->color.w = 0.75f; // �����ۂ����o�����ߏ��������ɂ���
		return;

	}

	const vec2 mouse = engine->GetMousePosition();
	if (abs(mouse.x - basePosition.x) < owner->scale.x &&
		abs(mouse.y - basePosition.y) < owner->scale.y) {
		// �J�[�\�����{�^���̈���ɂ���
		const bool buttonDown = engine->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		if (buttonDown) {
			// ���{�^����������Ă���
			std::copy_n(&pressedColor.x, 3, &owner->color.x);

		}
		else {
			// ���{�^����������Ă���
			std::copy_n(&highlightedColor.x, 3, &owner->color.x);
			// �{�^���N���b�N����
			if (prevButtonDown) {
				// �f���Q�[�g�����s
				for (auto& e : onClick) {
					e(this);

				}

			}

		} // if buttonDown

				// �}�E�X�{�^���̏�Ԃ��X�V
		prevButtonDown = buttonDown;
	}
	else {
		    // �J�[�\�����{�^���̈�O�ɂ���
		std::copy_n(&normalColor.x, 3, &owner->color.x);
		prevButtonDown = false;
	}
}