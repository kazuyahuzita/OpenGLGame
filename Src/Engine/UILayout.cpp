#include "UILayout.h"
 #include "Engine.h"

/**
* UIレイアウトの実行を開始する
*/
void UILayout::Start()
{
   // 最初に指定された座標を「カメラから見た座標」とする	
	basePosition = GetOwner()->position;	
}

/**
* 更新イベント
*
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void UILayout::Update(float deltaTime)
 {
	//常にカメラの方を向くようにゲームオブジェクトの向きを調整(ビルボードと同じ)
	GameObject * owner = GetOwner();
	const Engine * engine = owner->GetEngine();
	const GameObject & camera = engine->GetMainCamera();
	owner->rotation.y = camera.rotation.y;

	
	// 常にカメラの正面に位置するようにゲームオブジェクトの座標を調整
	// 回転の公式によってカメラとの相対座標を求める
	vec3 pos = basePosition; // 角度0度の場合の座標

  // Y座標の-1～+1が、カメラに写る範囲の上端と下端になるように、Z座標を調整
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