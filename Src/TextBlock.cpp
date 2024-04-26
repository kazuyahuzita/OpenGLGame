#include "TextBlock.h"
#include "Engine/UILayout.h"

void TextBlock::Awake()
{
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	//owner->staticMesh = engine->GetStaticMesh("Res/MeshData/Athletic/TextBlock/TextBlock.obj");
	
	// 全てのUIObjectの設定とサイズ変更、描画順を変更
	//画面全体を覆うサイズに設定
	//
	const vec2 fbSize = engine->GetFramebufferSize();

	auto text = engine->CreateUIObject<UILayout>("Res/Image/SelectStage/Stage1.tga", "text", { 0, 0 }, 1);
	textObject = text.first;
	textObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	textObject->renderQueue = RenderQueue_overlay + 1;

	auto text1 = engine->CreateUIObject<UILayout>("Res/Image/SelectStage/Stage2.tga", "text", { 0, 0 }, 1);
	textObject1 = text1.first;
	textObject1->scale = { fbSize.x / fbSize.y, 1, 1 };
	textObject1->renderQueue = RenderQueue_overlay + 1;

	auto text2 = engine->CreateUIObject<UILayout>("Res/Image/SelectStage/Stage3.tga", "text", { 0, 0 }, 1);
	textObject2 = text2.first;
	textObject2->scale = { fbSize.x / fbSize.y, 1, 1 };
	textObject2->renderQueue = RenderQueue_overlay + 1;

	auto text3 = engine->CreateUIObject<UILayout>("Res/Image/SelectStage/TestStage.tga", "text", { 0, 0 }, 1);
	textObject3 = text3.first;
	textObject3->scale = { fbSize.x / fbSize.y, 1, 1 };
	textObject3->renderQueue = RenderQueue_overlay + 1;

	auto text4 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/NextScene.tga", "text", { 0, 0 }, 1);
	nextSceneTextObject = text4.first;
	nextSceneTextObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	nextSceneTextObject->renderQueue = RenderQueue_overlay + 1;

	auto text5 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/BackScene.tga", "text", { 0, 0 }, 1);
	backSceneTextObject = text5.first;
	backSceneTextObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	backSceneTextObject->renderQueue = RenderQueue_overlay + 1;
}

void TextBlock::Update(float deltaTime)
{
	auto owner = GetOwner();

	//触れているかいないかでテキスト表示のアルファ値を変更

	if (isHitPlayer)textObject->color.w = 1.0f;
	else textObject->color.w = 0.0f;

	if (isHitPlayer1)textObject1->color.w = 1.0f;
	else textObject1->color.w = 0.0f;

	if (isHitPlayer2)textObject2->color.w = 1.0f;
	else textObject2->color.w = 0.0f;

	if (isHitPlayer3)textObject3->color.w = 1.0f;
	else textObject3->color.w = 0.0f;

	if (isHitPlayer4)nextSceneTextObject->color.w = 1.0f;
	else nextSceneTextObject->color.w = 0.0f;

	if (isHitPlayer5)backSceneTextObject->color.w = 1.0f;
	else backSceneTextObject->color.w = 0.0f;
}
/*
* positionX,Zにプレイヤーの位置を指定し、
* rangeで指定した範囲に入ったかどうかを教えてくれる
* true:入った	false :入ってない
*/
bool TextBlock::IsPlayerInRange(float positionX, float positionZ, float range)
{
	auto owner = GetOwner();

	float deltaX = positionX - owner->position.x;
	float deltaZ = positionZ - owner->position.z;
	float distance = sqrt(deltaX * deltaX + deltaZ * deltaZ);

	// プレイヤーが範囲内にいるかどうかを判定して結果を返す
	return distance <= range;
}
