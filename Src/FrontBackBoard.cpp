#include "FrontBackBoard.h"

void FrontBackBoard::Awake()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	//�{�̂̃I�u�W�F�N�g�͐^�񒆂ɂ���x���Ɏw�肷��
	owner->staticMesh = 
		engine->GetStaticMesh("Res/MeshData/Athletic/FrontBackBoard/Base.obj");
	owner->scale = vec3{ 1.8f,1.8f,1.8f };
	// ��]����̃I�u�W�F�N�g
	frontBackBoard =
		engine->Create<GameObject>(owner->name + ".FrontBackBoard", owner->tag + "FrontBackBoard");
	frontBackBoard->staticMesh =
		engine->GetStaticMesh("Res/MeshData/Athletic/FrontBackBoard/FrontBackBoard.obj");
	frontBackBoard->rotation = { 0.0f,0.0f,0.0f };
	//�����ŉ�]�I�u�W�F�N�g���x���̎q�I�u�W�F�N�g�Ɏw��
	frontBackBoard->SetParent(owner);
	// �p�x�ɑΉ�����R���C�_�[�̌`���ݒ�
	auto collider = frontBackBoard->AddComponent<BoxCollider>();
	collider->isStatic = true;
	collider->box.scale = vec3{ 0.8f,0.1f,0.8f };
	collider->box.position = vec3{ 0.0f,0.0f,-0.3f };
}

void FrontBackBoard::Update(float deltaTime)
{

	switch (boardstate) {
	case Boardstate::right:
		// 90�x�ɂȂ�܂ŏ��X�ɉ�]������
		frontBackBoard->rotation.x += speed * deltaTime;
		if (frontBackBoard->rotation.x >= radians(0)) {
			frontBackBoard->rotation.x = radians(0); // 0�x�ɂȂ����̂ŉ�]���I��

			if (timerCount > 400)
			{
				boardstate = Boardstate::left;
				timerCount = 0;
			}
		}
		break;

	case Boardstate::left:
		// 90�x�ɂȂ�܂ŏ��X�ɉ�]������
		frontBackBoard->rotation.x -= speed * deltaTime;
		if (frontBackBoard->rotation.x <  radians(-180)) {
			frontBackBoard->rotation.x = radians(-180); // 180�x�ɂȂ����̂ŉ�]���I��
			
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

