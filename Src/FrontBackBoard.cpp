#include "FrontBackBoard.h"

void FrontBackBoard::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	//本体のオブジェクトは真ん中にある支柱に指定する
	owner->staticMesh = 
		engine->GetStaticMesh("Res/MeshData/Athletic/FrontBackBoard/Base.obj");
	owner->scale = vec3{ 1.8f,1.8f,1.8f };
	// 回転する板のオブジェクト
	frontBackBoard =
		engine->Create<GameObject>(owner->name + ".FrontBackBoard", owner->tag + "FrontBackBoard");
	frontBackBoard->staticMesh =
		engine->GetStaticMesh("Res/MeshData/Athletic/FrontBackBoard/FrontBackBoard.obj");
	frontBackBoard->rotation = { 0.0f,0.0f,0.0f };
	//ここで回転板オブジェクトを支柱の子オブジェクトに指定
	frontBackBoard->SetParent(owner);
	// 角度に対応するコライダーの形状を設定
	auto collider = frontBackBoard->AddComponent<BoxCollider>();
	collider->isStatic = true;
	collider->box.scale = vec3{ 0.8f,0.1f,0.8f };
	collider->box.position = vec3{ 0.0f,0.0f,-0.3f };
}

void FrontBackBoard::Update(float deltaTime)
{

	switch (boardstate) {
	case Boardstate::right:
		// 90度になるまで徐々に回転させる
		frontBackBoard->rotation.x += speed * deltaTime;
		if (frontBackBoard->rotation.x >= radians(0)) {
			frontBackBoard->rotation.x = radians(0); // 0度になったので回転を終了

			if (timerCount > 400)
			{
				boardstate = Boardstate::left;
				timerCount = 0;
			}
		}
		break;

	case Boardstate::left:
		// 90度になるまで徐々に回転させる
		frontBackBoard->rotation.x -= speed * deltaTime;
		if (frontBackBoard->rotation.x <  radians(-180)) {
			frontBackBoard->rotation.x = radians(-180); // 180度になったので回転を終了
			
			if (timerCount > 400)
			{
				boardstate = Boardstate::right;
				timerCount = 0;
			}

		}
		break;

	} // switch boardstate

	timer+= deltaTime;
	if (timer < 100)
	{
		timerCount++;
		timer = 0;
	}
}

