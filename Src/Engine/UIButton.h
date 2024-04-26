#ifndef UIBUTTON_H_INCLUDED
 #define UIBUTTON_H_INCLUDED
 #include "UILayout.h"
 #include <functional>

/**
* UI�p�̃{�^������R���|�[�l���g
*/
class UIButton : public UILayout
 {
public:	UIButton() = default;	
	  virtual ~UIButton() = default;	
	  virtual void Update(float deltaTime) override;	

	  bool interactable = true; // �}�E�X�ɔ������Atrue=���� false=���Ȃ�
	  vec3 normalColor = { 1, 1, 1 };               // �ʏ�F	
	  vec3 highlightedColor = { 1.5f, 1.5f, 1.5f }; // �I��F	
	  vec3 pressedColor = { 0.4f, 0.4f, 0.4f };     // �����F			
	  // �f���Q�[�g		
	  using ClickEvent = std::function<void(UIButton*)>;	
	  std::vector<ClickEvent> onClick;	
private:
	bool prevButtonDown = false; // �O��X�V���̃}�E�X���{�^��������	
};

#endif // UIBUTTON_H_INCLUDED