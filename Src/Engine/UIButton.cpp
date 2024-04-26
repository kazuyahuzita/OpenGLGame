#include "UIButton.h"
 #include "Engine.h"

/**
* 更新イベント
*
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void UIButton::Update(float deltaTime)
{
	// 基底クラスのメンバ関数を実行
	UILayout::Update(deltaTime);
	
	GameObject * owner = GetOwner();
	Engine * engine = owner->GetEngine();
	
	
	  // ボタンの有効・無効判定
	if (!interactable)
	{
		std::copy_n(&normalColor.x, 3, &owner->color.x);
		owner->color.w = 0.75f; // 無効ぽさを出すため少し透明にする
		return;

	}

	const vec2 mouse = engine->GetMousePosition();
	if (abs(mouse.x - basePosition.x) < owner->scale.x &&
		abs(mouse.y - basePosition.y) < owner->scale.y) {
		// カーソルがボタン領域内にある
		const bool buttonDown = engine->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		if (buttonDown) {
			// 左ボタンが押されている
			std::copy_n(&pressedColor.x, 3, &owner->color.x);

		}
		else {
			// 左ボタンが離されている
			std::copy_n(&highlightedColor.x, 3, &owner->color.x);
			// ボタンクリック判定
			if (prevButtonDown) {
				// デリゲートを実行
				for (auto& e : onClick) {
					e(this);

				}

			}

		} // if buttonDown

				// マウスボタンの状態を更新
		prevButtonDown = buttonDown;
	}
	else {
		    // カーソルがボタン領域外にある
		std::copy_n(&normalColor.x, 3, &owner->color.x);
		prevButtonDown = false;
	}
}