#ifndef UIBUTTON_H_INCLUDED
 #define UIBUTTON_H_INCLUDED
 #include "UILayout.h"
 #include <functional>

/**
* UI用のボタン制御コンポーネント
*/
class UIButton : public UILayout
 {
public:	UIButton() = default;	
	  virtual ~UIButton() = default;	
	  virtual void Update(float deltaTime) override;	

	  bool interactable = true; // マウスに反応を、true=する false=しない
	  vec3 normalColor = { 1, 1, 1 };               // 通常色	
	  vec3 highlightedColor = { 1.5f, 1.5f, 1.5f }; // 選択色	
	  vec3 pressedColor = { 0.4f, 0.4f, 0.4f };     // 押下色			
	  // デリゲート		
	  using ClickEvent = std::function<void(UIButton*)>;	
	  std::vector<ClickEvent> onClick;	
private:
	bool prevButtonDown = false; // 前回更新時のマウス左ボタン操作状態	
};

#endif // UIBUTTON_H_INCLUDED