#include "MeasureTime.h"

void MeasureTime::Awake()
{
	auto owner = GetOwner();
	auto engine = GetOwner()->GetEngine();
	//owner->staticMesh = engine->GetStaticMesh("Res/MeshData/Athletic/TextBlock/TextBlock.obj");

	//背景にあるグレーの背景画像
	auto back = engine->CreateUIObject<UILayout>("Res/Image/Athletic/Time_bg.tga", "text", { 1.3f, 0.8f }, 0.12f);
	backObject = back.first;
	backObject->color.w = 0.5f;
	backObject->renderQueue = RenderQueue_overlay;

	//タイムリミットが分かりやすくするための時計の画像
	auto clock = engine->CreateUIObject<UILayout>("Res/Image/Athletic/Clock.tga", "text", { 1.15f, 0.8f }, 0.08f);
	clockObject = clock.first;
	clockObject->renderQueue = RenderQueue_overlay + 1;

	//制限時間の1の桁用の画像の設定
	auto zeroText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/0.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit0 = zeroText.first;
	singledigit0->renderQueue = RenderQueue_overlay + 1;

	auto oneText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/1.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit1 = oneText.first;
	singledigit1->renderQueue = RenderQueue_overlay + 1;

	auto twoText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/2.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit2 = twoText.first;
	singledigit2->renderQueue = RenderQueue_overlay + 1;

	auto threeText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/3.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit3 = threeText.first;
	singledigit3->renderQueue = RenderQueue_overlay + 1;

	auto fourText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/4.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit4 = fourText.first;
	singledigit4->renderQueue = RenderQueue_overlay + 1;

	auto fiveText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/5.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit5 = fiveText.first;
	singledigit5->renderQueue = RenderQueue_overlay + 1;

	auto sixText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/6.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit6 = sixText.first;
	singledigit6->renderQueue = RenderQueue_overlay + 1;

	auto sevenText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/7.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit7 = sevenText.first;
	singledigit7->renderQueue = RenderQueue_overlay + 1;

	auto eightText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/8.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit8 = eightText.first;
	singledigit8->renderQueue = RenderQueue_overlay + 1;

	auto nineText = engine->CreateUIObject<UILayout>("Res/Image/Athletic/9.tga", "text", { 1.4f, 0.8f }, 0.1f);
	singledigit9 = nineText.first;
	singledigit9->renderQueue = RenderQueue_overlay + 1;

	//制限時間の10の桁用の設定
	auto zeroText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/0.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit0 = zeroText1.first;
	doubledigit0->renderQueue = RenderQueue_overlay + 1;

	auto oneText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/1.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit1 = oneText1.first;
	doubledigit1->renderQueue = RenderQueue_overlay + 1;

	auto twoText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/2.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit2 = twoText1.first;
	doubledigit2->renderQueue = RenderQueue_overlay + 1;

	auto threeText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/3.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit3 = threeText1.first;
	doubledigit3->renderQueue = RenderQueue_overlay + 1;

	auto fourText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/4.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit4 = fourText1.first;
	doubledigit4->renderQueue = RenderQueue_overlay + 1;

	auto fiveText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/5.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit5 = fiveText1.first;
	doubledigit5->renderQueue = RenderQueue_overlay + 1;

	auto sixText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/6.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit6 = sixText1.first;
	doubledigit6->renderQueue = RenderQueue_overlay + 1;

	auto sevenText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/7.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit7 = sevenText1.first;
	doubledigit7->renderQueue = RenderQueue_overlay + 1;

	auto eightText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/8.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit8 = eightText1.first;
	doubledigit8->renderQueue = RenderQueue_overlay + 1;

	auto nineText1 = engine->CreateUIObject<UILayout>("Res/Image/Athletic/9.tga", "text", { 1.3f, 0.8f }, 0.1f);
	doubledigit9 = nineText1.first;
	doubledigit9->renderQueue = RenderQueue_overlay + 1;

	//Vector配列に後ろから格納
	//1の桁のVector配列
	SingledigitVector.push_back(singledigit0);
	SingledigitVector.push_back(singledigit1);
	SingledigitVector.push_back(singledigit2);
	SingledigitVector.push_back(singledigit3);
	SingledigitVector.push_back(singledigit4);
	SingledigitVector.push_back(singledigit5);
	SingledigitVector.push_back(singledigit6);
	SingledigitVector.push_back(singledigit7);
	SingledigitVector.push_back(singledigit8);
	SingledigitVector.push_back(singledigit9);

	//10の桁のVector配列
	DoubledigitVector.push_back(doubledigit0);
	DoubledigitVector.push_back(doubledigit1);
	DoubledigitVector.push_back(doubledigit2);
	DoubledigitVector.push_back(doubledigit3);
	DoubledigitVector.push_back(doubledigit4);
	DoubledigitVector.push_back(doubledigit5);
	DoubledigitVector.push_back(doubledigit6);
	DoubledigitVector.push_back(doubledigit7);
	DoubledigitVector.push_back(doubledigit8);
	DoubledigitVector.push_back(doubledigit9);


}

void MeasureTime::Update(float deltaTime)
{
	if (measureTimeState != MeasureTimeState::alive)return;

	countTime -= deltaTime;


	if (countTime1 > 0 && countTime < 0.0f)
	{
		countTime = 10.0f;
		countTime1 -= 1;

	}
	else if (countTime1 == 0 && countTime < 0.0f)
	{
		isCount = true;
	}

	//if (countTime1 < 0)
	//{
	//	countTime1 = 0;
	//	
	//}


	//触れているかいないかでテキスト表示のアルファ値を変更
	if (countTime < 1.0f)SingledigitVector.at(0)->color.w = 1.0f;
	else SingledigitVector.at(0)->color.w = 0.0f;

	if (countTime >= 1.0f && countTime < 2.0f)SingledigitVector.at(1)->color.w = 1.0f;
	else SingledigitVector.at(1)->color.w = 0.0f;

	if (countTime >= 2.0f && countTime < 3.0f)SingledigitVector.at(2)->color.w = 1.0f;
	else SingledigitVector.at(2)->color.w = 0.0f;

	if (countTime >= 3.0f && countTime < 4.0f)SingledigitVector.at(3)->color.w = 1.0f;
	else SingledigitVector.at(3)->color.w = 0.0f;

	if (countTime >= 4.0f && countTime < 5.0f)SingledigitVector.at(4)->color.w = 1.0f;
	else SingledigitVector.at(4)->color.w = 0.0f;

	if (countTime >= 5.0f && countTime < 6.0f)SingledigitVector.at(5)->color.w = 1.0f;
	else SingledigitVector.at(5)->color.w = 0.0f;

	if (countTime >= 6.0f && countTime < 7.0f)SingledigitVector.at(6)->color.w = 1.0f;
	else SingledigitVector.at(6)->color.w = 0.0f;

	if (countTime >= 7.0f && countTime < 8.0f)SingledigitVector.at(7)->color.w = 1.0f;
	else SingledigitVector.at(7)->color.w = 0.0f;

	if (countTime >= 8.0f && countTime < 9.0f)SingledigitVector.at(8)->color.w = 1.0f;
	else SingledigitVector.at(8)->color.w = 0.0f;

	if (countTime >= 9.0f && countTime < 10.0f)SingledigitVector.at(9)->color.w = 1.0f;
	else SingledigitVector.at(9)->color.w = 0.0f;

	//10の桁用のON/OFF処理
	if (countTime1 == 0)DoubledigitVector.at(0)->color.w = 1.0f;
	else DoubledigitVector.at(0)->color.w = 0.0f;

	if (countTime1 == 1)DoubledigitVector.at(1)->color.w = 1.0f;
	else DoubledigitVector.at(1)->color.w = 0.0f;

	if (countTime1 == 2)DoubledigitVector.at(2)->color.w = 1.0f;
	else DoubledigitVector.at(2)->color.w = 0.0f;

	if (countTime1 == 3)DoubledigitVector.at(3)->color.w = 1.0f;
	else DoubledigitVector.at(3)->color.w = 0.0f;

	if (countTime1 == 4)DoubledigitVector.at(4)->color.w = 1.0f;
	else DoubledigitVector.at(4)->color.w = 0.0f;

	if (countTime1 == 5)DoubledigitVector.at(5)->color.w = 1.0f;
	else DoubledigitVector.at(5)->color.w = 0.0f;

	if (countTime1 == 6)DoubledigitVector.at(6)->color.w = 1.0f;
	else DoubledigitVector.at(6)->color.w = 0.0f;

	if (countTime1 == 7)DoubledigitVector.at(7)->color.w = 1.0f;
	else DoubledigitVector.at(7)->color.w = 0.0f;

	if (countTime1 == 8)DoubledigitVector.at(8)->color.w = 1.0f;
	else DoubledigitVector.at(8)->color.w = 0.0f;

	if (countTime1 == 9)DoubledigitVector.at(9)->color.w = 1.0f;
	else DoubledigitVector.at(9)->color.w = 0.0f;

}

