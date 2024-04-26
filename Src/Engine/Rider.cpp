#include "Rider.h"

void Rider::Awake()
{
	auto owner = GetOwner();
	auto engine = owner->GetEngine();

	// �R���C�_�[��ݒ�
	// ���̃R���C�_�[���㉺�ɕ��ׂĒu�����ƂŁA�c���̏Փˌ`����Č�
	for (float i = 0; i < 2; ++i) {
		auto collider = owner->AddComponent<SphereCollider>();
		collider->sphere.radius = 0.5f;
		collider->sphere.position.z = -0.5f;
		collider->sphere.position.y = -0.5f * i;
	}

	// �o�C�N��ǉ�
	auto bike = engine->Create<GameObject>("bike", "bike",{ 0.0f, -3.6f, -0.5f });
	bike->rotation.x = radians(10);
	bike->rotation.y = radians(180);
	bike->scale = vec3{ 2,2,2 };
	bike->staticMesh = engine->GetStaticMesh("Res/MeshData/DeathForest/Motor/Motor.obj");
	bike->SetParent(owner);

	//�_�b�V���G�t�F�N�g����肽��
	auto dashEffect = engine->CreateUIObject<UILayout>("Res/Effect/DashEffect.tga","Dash", {0,0}, 1);
	auto dashEffect1 = engine->CreateUIObject<UILayout>("Res/Effect/DashEffect.tga","Dash", {0,0}, 1);

	DashEffectObject = dashEffect.first;
	// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
	const vec2 fbSize = engine->GetFramebufferSize();
	DashEffectObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	DashEffectObject->color.w = 0;

	DashEffectObject1 = dashEffect1.first;
	// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
	const vec2 fbSize1 = engine->GetFramebufferSize();
	DashEffectObject1->scale = { fbSize1.x / fbSize1.y, 1, 1 };
	DashEffectObject1->color.w = 0;

}

void Rider::Update(float deltaTime)
{
	auto owner = GetOwner();
	Engine* engine = GetOwner()->GetEngine();
	GameObject& camera = *GetOwner();

	double& previousTime = engine->GetPreviousTime();

	// drive�ȊO�̏�Ԃ͈ړ��ł��Ȃ�
	if (riderState != RiderState::drive)return;

	// ���쐧��p�^�C�}�[���X�V
	timer += deltaTime;
	const float cameraCosY = cos(camera.rotation.y);
	const float cameraSinY = sin(camera.rotation.y);
	//��ɐi��ł���悤�ɂ���
	camera.position.z -= 0.1f;

	//�}�N����`�́A�����ɈӖ����������Ă���̂Ɠ��`
	//�߂�ǂ������Ƃ��́A�����ł�����

	/*
	* �Q�[���R���g���[���[�ł̑���
	*/

	if (glfwGetGamepadState(0, &gamepadstate))
	{

		//�ړ�
		//���̈ړ�
		camera.position.x += gamepadstate.axes[0] * cameraSpeed  * cameraCosY * deltaTime;
		camera.position.z += gamepadstate.axes[0] * cameraSpeed  * -cameraSinY * deltaTime;
		//�c
		camera.position.x += gamepadstate.axes[1] * cameraSpeed  * cameraSinY  * deltaTime;
		camera.position.z += gamepadstate.axes[1] * cameraSpeed  * cameraCosY  * deltaTime;

		//���x���オ���Ă���̂𕪂���₷������
		if (gamepadstate.axes[1] <  -0.5f)
		{
			DashEffectObject1->color.w = 1;

		}
		else {

			DashEffectObject1->color.w = 0;
		}
		

		//����
		//��
		// �L�����N�^�[��X���W�𐧌�����i��ʏ㕔�Ɖ����ɃL�����N�^�[���o�Ȃ��悤�ɂ���j
		if (camera.rotation.y < -0.4f) {
			camera.rotation.y = -0.4f;
		}
		else if (camera.rotation.y > 0.8f) {
			camera.rotation.y = 0.8f;
		}

		//�o�C�N�̉E����
		if (gamepadstate.axes[2] > 0.2f)
		{
			owner->GetChild(0)->rotation.y -= bikerotate*deltaTime ;
		}
		//�o�C�N�̍�����
		else if (gamepadstate.axes[2] < -0.2f)
		{
			owner->GetChild(0)->rotation.y += bikerotate*deltaTime;
		}
		else 
		{
			//3.1415926535
			owner->GetChild(0)->rotation.y = radians(180)/** cameraCosY*/;
		}

		camera.rotation.y -= gamepadstate.axes[2] * cameraSpeed * deltaTime;

		//�c
		// �L�����N�^�[��Y���W�𐧌�����i��ʏ㕔�Ɖ����ɃL�����N�^�[���o�Ȃ��悤�ɂ���j
		if (camera.rotation.x < -0.1f) {
			camera.rotation.x = -0.1f;
		}
		else if (camera.rotation.x > 0.2f) {
			camera.rotation.x = 0.2f;
		}
		camera.rotation.x -= gamepadstate.axes[3] * cameraSpeed * deltaTime;
	}

	/*
	* �L�[�{�[�h�ł̑���
	*/

	//�J�����݂̂̎��_�ړ�
	// �J������Y���ړ�
	//��]
	
	//�E�Ɍ�����
	if (engine->GetKey(GLFW_KEY_RIGHT)) {
		//����
		if (camera.rotation.y < -0.2f)
			camera.rotation.y = -0.2f;

		camera.rotation.y -= cameraSpeed * deltaTime;
	}
	//���Ɍ�����
	if (engine->GetKey(GLFW_KEY_LEFT)) {
		//���
		if (camera.rotation.y > 0.6f)
			camera.rotation.y = 0.6f;
		owner->GetChild(0)->rotation.y += bikerotate * deltaTime;

		camera.rotation.y += cameraSpeed * deltaTime;
	}
	//��Ɍ�����
	if (engine->GetKey(GLFW_KEY_UP)) {
		//���
		if (camera.rotation.x > 0.1f) camera.rotation.x = 0.2f;

		camera.rotation.x += cameraSpeed * deltaTime;
	}
	//���Ɍ�����
	if (engine->GetKey(GLFW_KEY_DOWN)) {
		//����
		if (camera.rotation.x < -0.08f) camera.rotation.x = -0.08f;

		camera.rotation.x -= cameraSpeed * deltaTime;
	}

	// ========= ��l�̎��_�ł̈ړ� ============= //
	// �J������Y���ړ�
	  // �J�����̌��ړ�
	if (engine->GetKey(GLFW_KEY_S)) {
		camera.position.x += cameraSpeed  * cameraSinY * deltaTime;
		camera.position.z += cameraSpeed  * cameraCosY * deltaTime;

	}
	//�J�����̑O�ړ�
	if (engine->GetKey(GLFW_KEY_W)) {
		camera.position.x -= cameraSpeed  * cameraSinY * deltaTime;
		camera.position.z -= cameraSpeed  * cameraCosY * deltaTime;
		DashEffectObject->color.w = 1;

	}
	else {

		DashEffectObject->color.w = 0;
	}

	
	// �J������X���ړ�			
	// �J�����̍��ړ�		 
	if (engine->GetKey(GLFW_KEY_A)) {
		camera.position.x -= cameraSpeed  * cameraCosY * deltaTime;
		camera.position.z -= cameraSpeed  * -cameraSinY * deltaTime;
	}
	//�J�����̉E�ړ�
	if (engine->GetKey(GLFW_KEY_D)) {
		camera.position.x += cameraSpeed  * cameraCosY * deltaTime;
		camera.position.z += cameraSpeed  * -cameraSinY * deltaTime;
	}
	//�Q�[���̋����I��
	if (engine->GetKey(GLFW_KEY_ESCAPE))
	{
		if (!isESC)isESC = true;
	}

}

void Rider::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	const auto& name = other->GetOwner()->name;
	if (name == "stop")
	{
		//Stop�I�u�W�F�N�g�ɐG�ꂽ��~�܂�悤��
		riderState = RiderState::stop;
	}
	else if (name == "goal") {
		//�S�[���I�u�W�F�N�g�ɐG�ꂽ��S�[���V�[���ɑJ�ڂ�����悤��
		riderState = RiderState::goal;

	}
	else
	{
		//�Ȃɂ����̂��̂ɐG�ꂽ��Q�[���I�[�o�[�ɂȂ�悤��
		riderState = RiderState::dead;
	}

}
