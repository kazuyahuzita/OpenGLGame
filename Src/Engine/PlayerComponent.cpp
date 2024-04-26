#include "PlayerComponent.h"

void PlayerComponent::Awake()
{
	auto owner = GetOwner();
	auto engine = owner->GetEngine();

	//�X�|�b�g���C�g
	//auto lightObject = engine->Create<GameObject>("light", "light");
	auto light = GetOwner()->AddComponent<Light>();
	light->color = { 0.95f, 1.0f, 0.98f };
	light->intensity = 15;
	light->radius = 20;
	light->type = Light::Type::spotLight;

	// �R���C�_�[��ݒ�
	// ���̃R���C�_�[���㉺�ɕ��ׂĒu�����ƂŁA�c���̏Փˌ`����Č�
	for (float i = 0; i < 2; ++i) {
		auto collider = owner->AddComponent<SphereCollider>();
		collider->sphere.radius = 0.5f;
		collider->sphere.position.y = -0.5f * i;

	}
	//�d�͂̒ǉ�
	owner->AddComponent<CharacterMovement>();
	//���鏰�ɐG�ꂽ�Ƃ��̏���
	owner->AddComponent<SlidingMovement>();

	const vec2 fbSize = engine->GetFramebufferSize();
	//��_���[�W�G�t�F�N�g�i�t�@�C�A�o�[�W�����j����肽��
	auto damageEffect = 
		engine->CreateUIObject<UILayout>("Res/Effect/Damage.tga", "Damage", { 0,0 }, 1);

	DamageEffectObject = damageEffect.first;
	// ��ʑS�̂𕢂��T�C�Y�ɐݒ�
	DamageEffectObject->scale = { fbSize.x / fbSize.y, 1, 1 };
	//�n�߂ɐݒ肵�Ă����Ȃ��Ɖ�ʂɂ�������N����̂Œ�`
	DamageEffectObject->color.x = 1;
	DamageEffectObject->color.y = 0;
	DamageEffectObject->color.z = 0;
	DamageEffectObject->color.w = 0;

}

void PlayerComponent::Update(float deltaTime)
{
	Engine* engine = GetOwner()->GetEngine();
	GameObject& camera = *GetOwner();
	double& previousTime = engine->GetPreviousTime();

	// dead��goal,stop,drive�̏�ԂȂ甽�����Ȃ�
	if (state == PlayerState::dead || state == PlayerState::goal || 
		state == PlayerState::stop || state == PlayerState::drive)
		return;
	

	//�J�����̉�]���X���[�Y�ɂ��邽�߂ɔz�u
	const float cameraCosY = cos(camera.rotation.y);
	const float cameraSinY = sin(camera.rotation.y);
	/*
	* �Q�[���R���g���[���[�ł̑���
	*/
	if (glfwGetGamepadState(0, &gamepadstate))
	{
		//�W�����v�̔���
		if (gamepadstate.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS)
		{
			camera.position.y += JumpPower * deltaTime;
			if (!isJump)
			{
				EasyAudio::PlayOneShot(SE::Jump);
				JumpTimer = 2.0f;
				isJump = true;
			}


		}
		//�_�b�V���̔���
		if (gamepadstate.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS)
		{
			addDashPower = 2.0f;

		}
		else {
			addDashPower = 1.0f;
		}

		//�ړ�
		//��
		camera.position.x +=
			gamepadstate.axes[0] * cameraSpeed * addDashPower * cameraMoveSpeed * cameraCosY * deltaTime;
		camera.position.z +=
			gamepadstate.axes[0] * cameraSpeed * addDashPower * cameraMoveSpeed * -cameraSinY * deltaTime;
		//�c
		camera.position.x +=
			gamepadstate.axes[1] * cameraSpeed * addDashPower * cameraMoveSpeed * cameraSinY * deltaTime;
		camera.position.z +=
			gamepadstate.axes[1] * cameraSpeed * addDashPower * cameraMoveSpeed * cameraCosY * deltaTime;

		//����
		camera.rotation.y -= gamepadstate.axes[2] * cameraSpeed * deltaTime;
		//�c
		// �L�����N�^�[��Y���W�𐧌�����i��ʏ㕔�Ɖ����ɃL�����N�^�[���o�Ȃ��悤�ɂ���j
		if (camera.rotation.x < -0.6f) {
			camera.rotation.x = -0.6f;
		}
		else if (camera.rotation.x > 0.6f) {
			camera.rotation.x = 0.6f;
		}
		camera.rotation.x -= gamepadstate.axes[3] * cameraSpeed * deltaTime;

	}

	/*
	* �L�[�{�[�h�ł̑���
	*/

	//����
	if (engine->GetKey(GLFW_KEY_LEFT_SHIFT)) {
		addDashPower = 2.0f;

	}
	else {
		addDashPower = 1.0f;
	}
	//���
	//�㉺�̈ړ�
	if (engine->GetKey(GLFW_KEY_SPACE)) {
		camera.position.y += JumpPower * deltaTime;
		if (!isJump)
		{
			EasyAudio::PlayOneShot(SE::Jump);
			JumpTimer = 2.0f;
			isJump = true;
		}
	}
	//�J�����݂̂̎��_�ړ�
	// �J������Y���ړ�
	//��]

	//�E�ɉ�]����
	if (engine->GetKey(GLFW_KEY_RIGHT)) {

		camera.rotation.y -= cameraSpeed * deltaTime;
	}
	//���ɉ�]����
	if (engine->GetKey(GLFW_KEY_LEFT)) {

		camera.rotation.y += cameraSpeed * deltaTime;
	}
	//��ɉ�]����
	if (engine->GetKey(GLFW_KEY_UP)) {
		//���
		if (camera.rotation.x > 0.3f) camera.rotation.x = 0.3f;

		camera.rotation.x += cameraSpeed * deltaTime;
	}
	//���ɉ�]����
	if (engine->GetKey(GLFW_KEY_DOWN)) {
		//����
		if (camera.rotation.x < -0.1f) camera.rotation.x = -0.1f;

		camera.rotation.x -= cameraSpeed * deltaTime;
	}

	// ========= ��l�̎��_�ł̈ړ� ============= //
	  // �J������Y���ړ�
	if (engine->GetKey(GLFW_KEY_S)) {
		camera.position.x += cameraSpeed * cameraMoveSpeed * addDashPower * cameraSinY * deltaTime;
		camera.position.z += cameraSpeed * cameraMoveSpeed * addDashPower * cameraCosY * deltaTime;
	}//if GLFW_KEY_S(��ړ�)

	if (engine->GetKey(GLFW_KEY_W)) {
		camera.position.x -= cameraSpeed * cameraMoveSpeed * addDashPower * cameraSinY * deltaTime;
		camera.position.z -= cameraSpeed * cameraMoveSpeed * addDashPower * cameraCosY * deltaTime;
	}//if GLFW_KEY_W(�O�ړ�)

	// �J������X���ړ�
	if (engine->GetKey(GLFW_KEY_A)) {
		camera.position.x -= cameraSpeed * cameraMoveSpeed * addDashPower * cameraCosY * deltaTime;
		camera.position.z -= cameraSpeed * cameraMoveSpeed * addDashPower * -cameraSinY * deltaTime;
	}// if GLFW_KEY_A(���ړ�)

	if (engine->GetKey(GLFW_KEY_D)) {
		camera.position.x += cameraSpeed * cameraMoveSpeed * addDashPower * cameraCosY * deltaTime;
		camera.position.z += cameraSpeed * cameraMoveSpeed * addDashPower * -cameraSinY * deltaTime;
	}// if GLFW_KEY_D(�E�ړ�)

	//�Q�[���I��
	if (engine->GetKey(GLFW_KEY_ESCAPE))
	{
		if (!isESC)isESC = true;
	}
	if (isESC)
	{
		glfwTerminate();
	}// if isESC

	//�t�@�C�A�G�t�F�N�g�ɓ���������H����Ă�悤�ɂ���
	if (isFire)
	{
		DamageEffectObject->color.w += 0.1f * deltaTime;

	}
	else
	{
		if (DamageEffectObject->color.w >= 0.0f)
			DamageEffectObject->color.w -= 0.2f * deltaTime;
		else
			DamageEffectObject->color.w = 0.0f;

	}// if isFire

	//���鏰�ɐG�ꂽ���ǂ����𔻒f���Ă����
	if (isPlayerSlide)
	{
		GetOwner()->isSlided = true;

	}
	else
	{
		GetOwner()->isSlided = false;
	} // if isPlayerSlide

	//�W�����v�̍ۂɉ����d�Ȃ�Ȃ��悤�ɂ���
	if (JumpTimer > 0) {
		// �^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
		JumpTimer -= deltaTime * 2;
		// �^�C�}�[��0�ȉ��ɂȂ����烁�C���Q�[���V�[���ɐ؂�ւ���//ExplorationScene
		if (JumpTimer <= 0.0f) {
			isJump = false;
		}

	} // if JumpTimer

}

void PlayerComponent::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	// dead��goal,stop�̏�Ԃ͏Փ˂ɔ������Ȃ�
	if (state == PlayerState::dead || state == PlayerState::goal || 
		state == PlayerState::stop) 
		return;
	
	//name�ɉ����ď����𕪂��Ă���
	const auto& name = other->GetOwner()->name;
	if (name == "goal") 
	{
		// goal�̃I�u�W�F�N�g�ƏՓ˂�����goal��Ԃɂ���
		state = PlayerState::goal;
	}
	else if (name == "athletic")
	{
		//athletic�V�[���ɑJ�ڂ�����Ƃ��Ƀv���C���[�̏�Ԃ�ς���
		state = PlayerState::athletic;
	}
	else if (name == "Item")
	{
		//�A�C�e���I�u�W�F�N�g�ɐG�ꂽ��|�C���g��������
		ItemPoint++;
	}
	else if (name == "jumpingPlat")
	{
		//��ԍۂɈړ������Ɩ��܂�or�I�u�W�F�N�g���ђʂ��Ă��܂���������Ȃ�
		//�̂ŁA��������~�߂Ă��瓮����悤�ɂ��Ă���
		state = PlayerState::jumpinghit;
	}
	else if (name == "KnockbackEnemy")
	{
		//�v���C���[�𐁂���΂������ɂȂ�
		state = PlayerState::nolmalhit;
	}
	else if (name == "SlidingFloor")
	{
		//�G�ꂽ��v���C���[�����点��
		isPlayerSlide = true;
	}
	else if (name != "SlidingFloor")
	{
		//���ꂽ��v���C���[�����点��̂��~�߂�

		isPlayerSlide = false;
	}
	else if (name == "motor")
	{
		//���[�^�[�I�u�W�F�N�g�ɐG�ꂽ���Ԃ�ς���
		state = PlayerState::drive;
	}
	else if (name == "fire")
	{
		//�_���[�W�G�t�F�N�g�̃t���O��On��
		isFire = true;
	}
	else if (name != "fire")
	{
		//�_���[�W�G�t�F�N�g�̃t���O��Off��
		isFire = false;
	}

	//�^�O�ɂ���ď����𕪂��Ă���
	const auto& tag = other->GetOwner()->tag;

	if (tag == "Gas_Can")
	{
		//���ʂȃI�u�W�F�N�g�ׂ̈̃t���O��On�ɂ��Ă���
		isKeyItem = true;
	}
	if (tag == "Battery")
	{
		//�o�b�e���[�p�̃t���O��On�ɂ��Ă���
		isBattery = true;
	}
	if (tag == "Seat")
	{
		//�V�[�g�p�̃t���O��On�ɂ��Ă���
		isSeat = true;
	}
	if (tag == "Tyre")
	{
		//�^�C���p�̃t���O��On�ɂ��Ă���
		isTyre = true;
	}
	else if (tag == "enemy")
	{
		// �^�O��enemy�̃I�u�W�F�N�g�ƏՓ˂�����dead��Ԃɂ���
		state = PlayerState::dead;
	}
	else if (tag == "goal")
	{
		//�S�[���p�̃^�O���\�ߐݒu���Ă���
		state = PlayerState::goal;
	}

}

void PlayerComponent::Jumping(float deltaTime)
{
	GameObject& camera = *GetOwner();
	auto owner = GetOwner();
	//�܂�������ɔ�΂��Ă���
	camera.position.y += 2.0f*deltaTime;
}

void PlayerComponent::Knockback(float deltaTime)
{
	GameObject& camera = *GetOwner();
	auto owner = GetOwner();
	const vec3 front = { cos(owner->rotation.y), 0, -sin(owner->rotation.y) };
	constexpr float powner = 0.01f;

	//�v���C���[�����ɐ�����΂��`�ɂ���
	camera.position.z = -front.z +(sin(powner)* 0.01f) * deltaTime;
	camera.position.y +=  1.2f * deltaTime;

}

