
/**
* @file Engine.cpp
*/

#define _CRT_SECURE_NO_WARNINGS

#include "Engine.h"
#include "FramebufferObject.h"
#include "EasyAudio/EasyAudio.h"
#include "AabbCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Debug.h"
#include <fstream>
#include <filesystem>

// �}�`�f�[�^
#include "../../Res/MeshData/crystal_mesh.h"
#include "../../Res/MeshData/barrel_mesh.h"
#include "../../Res/MeshData/wall_mesh.h"
#include "../../Res/MeshData/plane_xy_mesh.h"
#include "../../Res/MeshData/skull_mesh.h"

#include "../../Res/MeshData/chest_open_mesh.h"
#include "../../Res/MeshData/chest_close_mesh.h"

#include "../../Res/MeshData/door_open_mesh.h"
#include "../../Res/MeshData/door_close_mesh.h"

#include "../../Res/MeshData/pillar_mesh.h"




/**
* �Q�[���G���W�������s����
*
* @retval 0     ����Ɏ��s����������
* @retval 0�ȊO �G���[����������
*/

/**
* OpenGL����̃��b�Z�[�W����������R�[���o�b�N�֐�
*
* @param source    ���b�Z�[�W�̔��M��(OpenGL�AWindows�A�V�F�[�_�Ȃ�)
* @param type      ���b�Z�[�W�̎��(�G���[�A�x���Ȃ�)
* @param id        ���b�Z�[�W����ʂɎ��ʂ���l
* @param severity  ���b�Z�[�W�̏d�v�x(���A���A��A�Œ�)
* @param length    ���b�Z�[�W�̕�����. �����Ȃ烁�b�Z�[�W��0�I�[����Ă���
* @param message   ���b�Z�[�W�{��
* @param userParam �R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^
*/
void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string s;
	if (length < 0) {
		s = message;

	}
	else {
		s.assign(message, message + length);

	}
	s += '\n'; // ���b�Z�[�W�ɂ͉��s���Ȃ��̂Œǉ�����
	LOG(s.c_str());
}

/**
* �V�F�[�_�t�@�C����ǂݍ���ŃR���p�C������
*
* @param type     �V�F�[�_�̎��
* @param filename �V�F�[�_�t�@�C����
*
* @return �V�F�[�_�̊Ǘ��ԍ�
*/
GLuint CompileShader(GLenum type, const char* filename)
{
	std::ifstream file(filename, std::ios::binary); // �t�@�C�����J��
	if (!file) {
		LOG_ERROR("%s���J���܂���", filename);
		return 0;

	}

	// �t�@�C����ǂݍ���
	const size_t filesize = std::filesystem::file_size(filename);
	std::vector<char> buffer(filesize);
	file.read(buffer.data(), filesize);
	file.close();

	// �\�[�X�R�[�h��ݒ肵�ăR���p�C��
	const char* source[] = { buffer.data() };
	const GLint length[] = { int(buffer.size()) };
	const GLuint object = glCreateShader(type);
	glShaderSource(object, 1, source, length);
	glCompileShader(object);
	return object;
}

int Engine::Run()
{

	const int result = Initialize();
	if (result) {
		return result;

	}
	
	  // �������C�u������������
	if (!EasyAudio::Initialize()) {
		return 1; // ���������s

	}
	while (!glfwWindowShouldClose(window))
	{
		Update();
		Render();
		RemoveDestroyedGameObject();
		// �������C�u�������X�V
		EasyAudio::Update();
	}
	
	// �������C�u�������I��
	EasyAudio::Finalize();

	glfwTerminate();
	return 0;
}


/**
* �Q�[���I�u�W�F�N�g�z�u�t�@�C����ǂݍ���
*/
GameObjectList Engine::LoadGameObjectMap(
	const char* filename, const FilepathMap& filepathMap)
{
	// �t�@�C�����J��
	std::ifstream ifs(filename);
	if (!ifs) {
		LOG_WARNING("%s���J���܂���", filename);
		return {};

	}

	GameObjectList gameObjectList;
	gameObjectList.reserve(1000); // �K���Ȑ���\��
	
	  // �t�@�C����ǂݍ���
	while (!ifs.eof()) {
		std::string line;
		std::getline(ifs, line);
		const char* p = line.c_str();
		int readByte = 0; // �ǂݍ��񂾃o�C�g��

		// �Q�[���I�u�W�F�N�g��`�̊J�n����(�擪�ɂ͕K��name�v�f������͂�)
		char name[256] = { 0 }; // �Q�[���I�u�W�F�N�g��
		char tag[256] = { 0 }; // �Q�[���I�u�W�F�N�g�̃^�O��
		//�������񃊃e������ǂݍ��ނ��߂̒��g�ɕς���Ă���
		// R����n�܂�A�h�i�j�h�̋�؂蕶���ɂȂ��Ă���
		//scanfk�n�̊֐��ŁA���̒���̐��l�̓o�C�g���ɂȂ�
		if (sscanf(p, R"( { "name" : "%255[^"]" %n)", name, &readByte) != 1) {
			continue; // ��`�ł͂Ȃ�

		}
		p += readByte; // �ǂݎ��ʒu���X�V

		// ���O�ȊO�̗v�f��ǂݍ���

		char parentName[256] = { 0 }; // �e�I�u�W�F�N�g��
		if (sscanf(p, R"(, "parent" : "%255[^"]" %n)", parentName, &readByte) == 1) {
			p += readByte;

		}

		char meshName[256] = { 0 }; // ���b�V����
		if (sscanf(p, R"(, "mesh" : "%255[^"]" %n)", meshName, &readByte) == 1) {
			p += readByte;

		}

		char renderType[256] = { 0 }; // �`��^�C�v
		if (sscanf(p, R"(, "renderType" : "%255[^"]" %n)", renderType, &readByte) == 1) {
			p += readByte;

		}

		vec3 t(0); // ���s�ړ�
		if (sscanf(p, R"(, "translate" : [ %f, %f, %f ] %n)",
			&t.x, &t.y, &t.z, &readByte) == 3) {
			p += readByte;

		}

		vec3 r(0); // ��]
		if (sscanf(p, R"(, "rotate" : [ %f, %f, %f ] %n)",
			&r.x, &r.y, &r.z, &readByte) == 3) {
			p += readByte;

		}

		vec3 s(1); // �g�嗦
		if (sscanf(p, R"(, "scale" : [ %f, %f, %f ] %n)",
			&s.x, &s.y, &s.z, &readByte) == 3) {
			p += readByte;

		}

		// �Q�[���I�u�W�F�N�g���쐬
		// �E����W�͉��������[Z�A������W�͉��������{Z
		// Z���̕������]�ƁAX���AY���̕������]���K�v
		// Unity�͍�����W�n�Ȃ̂ŁA���s�ړ��Ɖ�]�������E����W�n�ɕϊ�����
		auto gameObject = Create<GameObject>(name,tag);
		gameObject->position = vec3(t.x, t.y, -t.z);
		gameObject->rotation =
			vec3(radians(-r.x), radians(-r.y), radians(r.z));
		gameObject->scale = s;

		// ���b�V����ǂݍ���
		if (meshName[0]) {
			// ���b�V�����ɑΉ�����t�@�C���p�X������
			// �Ή�����t�@�C���p�X��������΃��b�V����ǂݍ���
			const auto itr = filepathMap.find(meshName);
			if (itr != filepathMap.end()) {
				const auto& path = itr->second.c_str();
				gameObject->staticMesh = meshBuffer->LoadOBJ(path);
			}

		} // if meshName[0]
		
		// �{�b�N�X�R���C�_�[���쐬
		//Unity�̂Ƃ���ō���Ă����������蔻���
		//JSON�`�����Ɠǂݍ��߂�̂ŁA�ǂݍ���

		const char strBoxCollider[] = R"(, "BoxCollider" : [)";
		if (strncmp(p, strBoxCollider, sizeof(strBoxCollider) - 1) == 0) {
			p += sizeof(strBoxCollider) - 1;
			Box box = { vec3(0), { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } }, vec3(1) };
			for (;;) {
				if (sscanf(p, R"( { "center" : [ %f, %f, %f ], "size" : [ %f, %f, %f ] } %n)",
					&box.position.x, &box.position.y, &box.position.z,
					&box.scale.x, &box.scale.y, &box.scale.z, &readByte) != 6)
				{
					break;
				}
				auto collider = gameObject->AddComponent<BoxCollider>();
				collider->box = box;
				collider->box.scale *= 0.5f; // Unity��BoxCollider��1x1m(0.5m�l��)
				collider->isStatic = true;
				p += readByte;

				// �����ɃJ���}������ꍇ�͔�΂�
				if (*p == ',') {
					++p;

				}

			}

		} // if strBoxCollider		

		// �Q�[���I�u�W�F�N�g�����X�g�ɒǉ�
		gameObjectList.push_back(gameObject);

	} // while


	// �쐬�����Q�[���I�u�W�F�N�g�z���Ԃ�
	LOG("�Q�[���I�u�W�F�N�g�z�u�t�@�C��%s��ǂݍ��݂܂���", filename);
	return gameObjectList;
}

/**
* �Q�[���G���W������S�ẴQ�[���I�u�W�F�N�g��j������
*/
void Engine::ClearGameObjectAll()
{
	for (auto& e : gameObjects)
	{
		e->OnDestroy();
	}	 gameObjects.clear();

	

}
/**
* ��������p��ݒ肷��
*
* @param fovY �ݒ肷�鐂������p(�x���@)
*/
void Engine::SetFovY(float fovY)
{
	degFovY = fovY;
	radFovY = degFovY * 3.1415926535f / 180; // �ʓx�@�ɕϊ�
	fovScaleY = 1 / tan(radFovY / 2); // ����p�ɂ��g�嗦
}

/**
* �}�E�X�J�[�\���̍��W���擾����
*
* @return �J�������W�n�̃J�[�\�����W
*/
vec2 Engine::GetMousePosition() const
{
	// �X�N���[�����W�n�̃J�[�\�����W���擾
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	// �߂�^�ɍ��킹�邽�߂�float�ɃL���X�g
	const vec2 pos = { static_cast<float>(x), static_cast<float>(y) };

	// UILayer�̍��W�n�ɍ��킹�邽�߂ɁA�X�N���[�����W�n����J�������W�n(Z=-1)�ɕϊ�����
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	const vec2 framebufferSize = { static_cast<float>(w), static_cast<float>(h) };
	const float aspectRatio = framebufferSize.x / framebufferSize.y;
	return {
	(pos.x / framebufferSize.x * 2 - 1) * aspectRatio,
	(pos.y / framebufferSize.y * 2 - 1) * -1 };
}

/**
* �}�E�X�{�^���̏�Ԃ��擾����
*
* @param button ���ׂ�{�^���������}�N���萔(GLFW_MOUSE_BUTTON_LEFT��)
*
* @retval true  ������Ă���
* @retval false ������Ă��Ȃ�
*/
bool Engine::GetMouseButton(int button) const
{
	return glfwGetMouseButton(window, button) == GLFW_PRESS;
}
/**
* �}�E�X�{�^���̃N���b�N��Ԃ��擾����
*/
bool Engine::GetMouseClick(int button) const
{
	// �͈͊O�̃{�^���ԍ��͖���
	if (button < GLFW_MOUSE_BUTTON_LEFT || button > GLFW_MOUSE_BUTTON_MIDDLE) {
		return false;

	}
	return mouseButtons[button].click;
}
/**
* �t���[���o�b�t�@�̑傫�����擾����
*
* @return �t���[���o�b�t�@�̏c�Ɖ��̃T�C�Y
*/
vec2 Engine::GetFramebufferSize() const
 {	
	int w, h;	
	glfwGetFramebufferSize(window, &w, &h);	
	return 
	{ 
		static_cast<float>(w), 
		static_cast<float>(h) 
	};	
}

/**
* �t���[���o�b�t�@�̃A�X�y�N�g����擾����
*
* @return �t���[���o�b�t�@�̃A�X�y�N�g��
*/
float Engine::GetAspectRatio() const
{
	const vec2 size = GetFramebufferSize();
	return size.x / size.y;
}

/**
* �e�N�X�`���̎擾
*/
TexturePtr Engine::GetTexture(const char* name)
 {
	  // �L���b�V���ɂ���΁A�L���V�����ꂽ�e�N�X�`����Ԃ�
	auto itr = textureCache.find(name);
	if (itr != textureCache.end()) {
		return itr->second; // �L���b�V�����ꂽ�e�N�X�`����Ԃ�
		
	}
	
	 // �L���b�V���ɂȂ���΁A�e�N�X�`�����쐬���ăL���b�V���ɓo�^
	auto tex = std::make_shared<TexHelper>(name);
	textureCache.emplace(name, tex);
	return tex; // �쐬�����e�N�X�`����Ԃ�
	}
/**
  * ���C�g�z�������������
  */
void Engine::InitializeLight()
{
	// �w�肳�ꂽ���̃��C�g�𐶐�
	//usedLights�z��̓��U�[�u�ɂ���ď�����
	lights.resize(lightResizeCount);
	//freeLights�z��̓��T�C�Y�ɂ���ď�����
	usedLights.reserve(lightResizeCount);

	// ���ׂẴ��C�g�𖢎g�p���C�g�z��Ɂu�t���v�Œǉ�
	//AllocateLight�֐��ŐV�������C�g���擾����Ƃ��A���C�g�z��̐擪����擾�����邽��
	freeLights.resize(lightResizeCount);
	for (int i = 0; i < lightResizeCount; ++i) {
		freeLights[i] = static_cast<int>(lightResizeCount - i - 1);

	} // for i
}


/*
* 
* ���g�p�̃��C�g���ЂƂ��o���ď�Ԃ��u�g�p���v�ɕύX���A���̃C���f�b�N�X��Ԃ��܂�
* 
* @return 
*/
int Engine::AllocateLight()
{
	// ���g�p�̃��C�g���Ȃ���΃��C�g�z����g������
	if (freeLights.empty())
	{
		const int oldSize = lights.size();
		lights.resize(oldSize + lightResizeCount);
		// �g���������C�g�𖢎g�p���C�g�z��Ɂu�t���v�Œǉ�
		for (int i = lights.size() - 1; i >= oldSize; --i)
		{
			freeLights.push_back(static_cast<int>(i));

		}

	}
	
	// ���g�p���C�g�z��̖�������C���f�b�N�X�����o��
	const int index = freeLights.back();
	freeLights.pop_back();
	
	//���o�����C���f�b�N�X���g�p�����C�g�z��ɒǉ�
	//�z��̃|�C���^�ł��邽�߁A�{���̓A�h���X���ω��������_�Ŏg���Ȃ��Ȃ�
	//�������A���C�g���C���f�b�N�X�ŊǗ����Ă��邽�߁A�A�h���X���ω����Ă�
	//�ړI�̃��C�g��������悤�ɂȂ��Ă���

	usedLights.push_back(index);
	
	  // ���C�g�̏�Ԃ��u�g�p���v�ɂ���
	lights[index].used = true;
	
	  // ���o�����C���f�b�N�X��Ԃ�
	return index;
}

/*
* 
* ���蓖�Ă��������A��������
* 
* AllocateLight�Ŏ擾�����C���f�b�N�X���G���W���ɕԋp���A���C�g�𖢎g�p��Ԃɂ���
*/
void Engine::DeallocateLight(int index)
 {
	if (index >= 0 && index < lights.size()) 
	{
		    // �C���f�b�N�X�𖢎g�p���C�g�z��ɒǉ�
		freeLights.push_back(index);
		
		    // ���C�g�̏�Ԃ��u���g�p�v�ɂ���
		lights[index].used = false;
		
	} // if
}

//���C�g�f�[�^���擾����z
LightData* Engine::GetLight(int index)
 {	
	if (index >= 0 && index < lights.size() && lights[index].used) {
		return &lights[index];		
	}	
	return nullptr;	
}

const LightData* Engine::GetLight(int index) const
 {
	return const_cast<Engine*>(this)->GetLight(index);
}

/**
* �}�E�X���W���甭�˂����������擾����
* ���[���h���W�n���~�����̂ŁA�X�N���[�����W�n���烏�[���h���W�n�ɕϊ�����
* �X�N���[�����W�n
*		��
* NDC���W�n
*		��
* �N���b�v���W�n
*		��
* �r���[���W�n
*		��
* ���[���h���W�n
*/
Ray Engine::GetRayFromMousePosition() const
{
	// �X�N���[�����W�n�̃}�E�X�J�[�\�����W���擾
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	// �X�N���[�����W�n����NDC���W�n�ɕϊ�
	//  NDC���W�n : -1�`+1�͈̔͂̍��W�n
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	vec3 nearPos = {
	static_cast<float>(x / w * 2 - 1),
	-static_cast<float>(y / h * 2 - 1),
	-1 };
	vec3 farPos = { nearPos.x, nearPos.y, 1 };

	// �[�x�l�̌v�Z���ʂ�-1�`+1�ɂȂ�悤�ȃp�����[�^A, B���v�Z
	// ���_�V�F�[�_�̒l����v�����邱��
	const float near = 0.35f;
	const float far = 1000;
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);
	
	// NDC���W�n����N���b�v���W�n�ɕϊ�
	//w�ɂ�鏜�Z�v���t�Ɏ��s���邱�ƂŎ����ł���
	nearPos *= near;
	farPos *= far;
	nearPos.z = (nearPos.z - A) / B;
	farPos.z = (farPos.z - A) / B;

	
	// �N���b�v���W�n����r���[���W�n�ɕϊ�
	//FOV�̔䗦�ƃA�X�y�N�g��𗘗p����
	const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
	const float invFovScale = 1.0f / GetFovScale();
	nearPos.x *= invFovScale * aspectRatio;
	nearPos.y *= invFovScale;
	farPos.x *= invFovScale * aspectRatio;
	farPos.y *= invFovScale;

	//�J�����̉�]��f���ɓK�p�������W�ϊ�
	//�J�������W�����Z���Ă����]�������߁A�t�ɉ�]���Ă���J�����̍��W�����Z
	  // �r���[���W�n���烏�[���h���W�n�ɕϊ�
	const float cameraSinY = std::sin(camera.rotation.y);
	const float cameraCosY = std::cos(camera.rotation.y);
	nearPos = {
	nearPos.x * cameraCosY - near * cameraSinY,
	nearPos.y,
	nearPos.x * -cameraSinY - near * cameraCosY };
	nearPos += camera.position;
	
	farPos = {
	farPos.x * cameraCosY - far * cameraSinY,
	farPos.y,
	farPos.x * -cameraSinY - far * cameraCosY };
	farPos += camera.position;


	  // �ߕ��ʂ̍��W�Ɖ����ʂ̍��W��������̌����x�N�g�������߂�
	//�����x�N�g���͐��K������K�v�����邽�߁A�x�N�g���̒����Ŋ����Ă���
	vec3 direction = farPos - nearPos;
	const float length = sqrt(
	direction.x * direction.x +
	direction.y * direction.y +
	direction.z * direction.z);
	direction *= 1.0f / length;
	
	return Ray{ nearPos, direction };

}

/**
* �����ƃR���C�_�[�̌�������
*/
bool Engine::Raycast(const Ray& ray, RaycastHit& hitInfo,
	const RaycastPredicate& pred) const
{
	// ��_�̏���������
	hitInfo.collider = nullptr;
	hitInfo.distance = FLT_MAX;

	for (const auto& go : gameObjects) {
		for (const auto& collider : go->colliders) {
			// AABB�����[���h���W�n�ɕϊ�
			// �R���C�_�[�����[���h���W�n�ɕϊ�
			const auto worldCollider =
			collider->GetTransformedCollider(go->GetTransformMatrix());

			// �����Ƃ̌�������
			float d;
			bool hit = false;
			switch (collider->GetType()) {
			case Collider::Type::AABB:
				hit = Intersect(static_cast<AabbCollider&>(*worldCollider).aabb, ray, d);
				break;
			case Collider::Type::Sphere:
				hit = Intersect(static_cast<SphereCollider&>(*worldCollider).sphere, ray, d);
				break;
			case Collider::Type::Box:
				hit = Intersect(static_cast<BoxCollider&>(*worldCollider).box, ray, d);
				break;

			
			}

			if (!hit) {
				continue;

			}

			// ��������̑ΏۂłȂ���Δ�΂�
			if (!pred(collider, d)) {
				continue;

			}

			// ��蔭�˓_�ɋ߂���_�����R���C�_�[��I��
			if (d < hitInfo.distance) {
				hitInfo.collider = collider;
				hitInfo.distance = d;

			}

		} // for colliders

	} // for gameObjects

		  // ��������R���C�_�[��������true�A�Ȃ����false��Ԃ�
	if (hitInfo.collider) {
		// ��_�̍��W���v�Z
		hitInfo.point = ray.start + ray.direction * hitInfo.distance;
		return true;

	}
	return false;
}
/**
* �Q�[���G���W��������������
*
* @retval 0     ����ɏ��������ꂽ
* @retval 0�ȊO �G���[����������
*/
int Engine::Initialize()
{
	//GLFW�̏�����
	if (glfwInit() != GLFW_TRUE) {
		return 1;	//���������s
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	window = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		return 1;		//�E�B���h�E�쐬���s
	}

	//OpenGL�R���e�L�X�g�̍쐬
	glfwMakeContextCurrent(window);

	//OpenGL�֐��̃A�h���X���擾
	//���C�u�����Ԃ̕s�������������邽�߂�reinterpret_cast���g��
	//�|�C���^�^�̖߂�l���g���֐����g�p���邽��Cast���K�v	
	//��΂����C�ӂ̃|�C���^�^�ɕԂ����߂�reinterpret_cast���g��
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwTerminate();
		return 1;	//�A�h���X�擾���s
	}

	// ���b�Z�[�W�R�[���o�b�N�̐ݒ�::nullptr�̂Ƃ����userParam�֐��̈����ɓn�����l�j
	glDebugMessageCallback(DebugCallback, nullptr);

	//���C�g������������
	InitializeLight();

	// �V�F�[�_��ǂݍ���ŃR���p�C��
	vs = CompileShader(GL_VERTEX_SHADER, "Res/standard.vert");
	fs = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");

	// 2�̃V�F�[�_�������N
	prog3D = glCreateProgram();
	glAttachShader(prog3D, vs);
	glAttachShader(prog3D, fs);
	glLinkProgram(prog3D);
	
	// �A�����b�g�E�V�F�[�_���쐬
	// ��̃V�F�[�_�������N
	vsUnlit = CompileShader(GL_VERTEX_SHADER, "Res/unlit.vert");
	fsUnlit = CompileShader(GL_FRAGMENT_SHADER, "Res/unlit.frag");
	progUnlit = glCreateProgram();
	glAttachShader(progUnlit, vsUnlit);
	glAttachShader(progUnlit, fsUnlit);
	glLinkProgram(progUnlit);

	// �X�P���^���E�V�F�[�_���쐬
	vsSkeletal = CompileShader(GL_VERTEX_SHADER, "Res/skeletal.vert");
	fsSkeletal = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");
	progSkeletal = glCreateProgram();
	glAttachShader(progSkeletal, vsSkeletal);
	glAttachShader(progSkeletal, fsSkeletal);
	glLinkProgram(progSkeletal);

	// �V���h�E�E�V�F�[�_���쐬
	vsShadow = CompileShader(GL_VERTEX_SHADER, "Res/shadow.vert");
	fsShadow = CompileShader(GL_FRAGMENT_SHADER, "Res/shadow.frag");
	progShadow = glCreateProgram();
	glAttachShader(progShadow, vsShadow);
	glAttachShader(progShadow, fsShadow);
	glLinkProgram(progShadow);
	glProgramUniform1f(progShadow, locAlphaCutoff, 0.5f); // �J�b�g�I�t�l��ݒ肵�Ă���
	
	// FBO���쐬
	//�[�x�o�b�t�@�����K�v�Ȃ̂ŁA�[�x�e�N�X�`�����쐬
	//GPU��h���C�o�ɂ���Đ��x������������
	//�����GL_DEPTH_COMPONENT32�iGeForce(Nvidia)�͐����I�j
	//24,32�����܂ł͖���炵��
	//
	auto texShadow = std::make_shared<TexHelper>(
		"FBO(depth)", 2048, 2048, GL_DEPTH_COMPONENT32);
	fboShadow = std::make_shared<FramebufferObject>(nullptr, texShadow);
	// �֐ߗp��SSBO���쐬
	ssboJointMatrices = BufferObject::Create(jointMatricesBytes);

	// �}�`�f�[�^�̏��
	struct MeshData {
		const char* name;            // ���b�V����
		const char* textureFilename; // �e�N�X�`���t�@�C����

		size_t vertexSize;      // ���_�f�[�^�̃o�C�g��
		size_t indexSize;       // �C���f�b�N�X�f�[�^�̃o�C�g��
		const void* vertexData; // ���_�f�[�^�̃A�h���X
		const void* indexData;  // �C���f�b�N�X�f�[�^�̃A�h���X

	};
	const MeshData meshes[] =
	{ 
		{"crystal", "Res/MeshData/crystal_blue.tga", sizeof(crystal_vertices), sizeof(crystal_indices), crystal_vertices, crystal_indices },
		{"barrel", "Res/MeshData/barrel.tga", sizeof(barrel_vertices), sizeof(barrel_indices), barrel_vertices, barrel_indices },
		{ "plane_xy", nullptr,sizeof(plane_xy_vertices), sizeof(plane_xy_indices), plane_xy_vertices, plane_xy_indices },
		{ "wall", "Res/MeshData/barrel.tga",sizeof(wall_vertices), sizeof(wall_indices),wall_vertices, wall_indices},
		{"skull", "Res/MeshData/skull.tga", sizeof(skull_vertices), sizeof(skull_indices), skull_vertices, skull_indices },
		{"chest", "Res/MeshData/chest.tga", sizeof(chest_close_vertices), sizeof(chest_close_indices), chest_close_vertices, chest_close_indices },
		{"pillar", "Res/MeshData/pillar.tga", sizeof(pillar_vertices), sizeof(pillar_indices), pillar_vertices, pillar_indices },
	};
	meshBuffer = MeshBuffer::Create(32'000'000);
	meshBuffer->SetTextureCallback(
		[this](const char* filename) { return GetTexture(filename); });

	for (const auto& e : meshes)
	{
		// �@����ݒ肷�邽�߂ɐ}�`�f�[�^�̃R�s�[�����
		auto pVertex = static_cast<const Vertex*>(e.vertexData);
		auto pIndex = static_cast<const uint16_t*>(e.indexData);
		std::vector<Vertex> v(pVertex, pVertex + e.vertexSize / sizeof(Vertex));

		// �R�s�[�����}�`�f�[�^�ɖ@����ݒ�
		for (auto& e : v) {
			e.normal = { 0, 0, 0 };

		}
		FillMissingNormals(v.data(), v.size(), pIndex, e.indexSize / sizeof(uint16_t));

		// �@����ݒ肵���}�`�f�[�^��GPU�������ɃR�s�[
		meshBuffer->AddVertexData(v.data(), e.vertexSize, pIndex, e.indexSize);


		// ���O��AddVertexData�ō쐬�����`��p�����[�^���擾
		const DrawParams& drawParams =
			meshBuffer->GetDrawParams(meshBuffer->GetDrawParamsCount() - 1);

		// �e�N�X�`�����쐬
		TexturePtr texBaseColor;
		if (e.textureFilename) {
			texBaseColor = GetTexture(e.textureFilename);
		}

		// �`��p�����[�^�ƃe�N�X�`������X�^�e�B�b�N���b�V�����쐬
		meshBuffer->CreateStaticMesh(e.name, drawParams, texBaseColor);

	}
	
	 // OBJ�t�@�C����ǂݍ���
	 skySphere = meshBuffer->LoadOBJ("Res/MeshData/sky_sphere/sky_sphere.obj");
	 cloud = meshBuffer->LoadOBJ("Res/MeshData/Athletic/SlidingBlock/SlidingBlock.obj");

	 //
	 meshBuffer->LoadOBJ("Res/MeshData/Stair/Stair.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Door/MainDoor.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Door/MainDoorFrame.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Spawned/SpawnerEnemy.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/ghost/ghost.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Bullet/Bullet.obj");

	 //AthleticScene�p�̃Q�[���I�u�W�F�N�g�̐ݒ�
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Block/Block.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Enemy/Enemy.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Face/Face.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/FrontBackBoard/Base.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/FrontBackBoard/FrontBackBoard.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Gate/Gate.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Gems/Gem.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/JumpingPlatforms/JumpingPlatforms.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Pendulum/Basis.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/Pendulum/Hammer.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/TextBlock/TextBlock.obj");
	// meshBuffer->LoadOBJ("Res/MeshData/Athletic/RotationBlock/RotationBlock.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/RotateBar/Center/RotateBar_Center.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/Athletic/RotateBar/Wide/RotateBar_Wide.obj");

	 //DeathForest�V�[���p�̃Q�[���I�u�W�F�N�g�̐ݒ�
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/GoalBlock/GoalBlock.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Light/Light.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/PlayerSphere/PlayerSphere.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Motor/Motor.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Rock/Rock.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Face/Yoshie.obj");

	 //�W�߂�p�[�c
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Item/Battery.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Item/Gas_Can.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Item/Seat.obj");
	 meshBuffer->LoadOBJ("Res/MeshData/DeathForest/Item/Tyre.obj");

	// �J�����̏����ݒ�
	camera.position = { 3, 1, 3 };
	camera.rotation.x = 3.14159265f;
	camera.rotation.y = 3.14159265f;
	return 0; // ����ɏ��������ꂽ
}

/**
* �Q�[���G���W���̏�Ԃ��X�V����
*/
void Engine::Update()
{

	//=============���Ԃ̌v��====================//
	  // �f���^�^�C��(�O��̍X�V����̌o�ߎ���)���v�Z
	const double currentTime = glfwGetTime(); // ���ݎ���
	deltaTime = static_cast<float>(currentTime - previousTime);
	previousTime = currentTime;
	
	if (deltaTime > 0.5f) {
		deltaTime = 1.0f / 60.0f;
	}
	//===========================================//
	
	  // �}�E�X�{�^���̏�Ԃ��擾
	for (size_t i = 0; i < std::size(mouseButtons); ++i) {
		MouseButton& e = mouseButtons[i];
		e.previous = e.current;
		e.current = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT + i);

		// ������Ă��鎞�Ԃ��X�V
		if (e.previous) {
			e.timer += deltaTime;

		}
		else {
			e.timer = 0;

		}

		// �ȉ��̏�����S�Ė������ꍇ�̓N���b�N�Ɣ���
		// - �O�t���[���ŉ�����Ă���
		// - ���݃t���[���ŉ�����Ă��Ȃ�
		// - ������Ă��鎞�Ԃ�0.3�b�ȉ�
		if (e.previous && !e.current && e.timer <= mouseClickSpeed) {
			e.click = true;

		}
		else {
			e.click = false;

		}

	} // for i

	// �V�[���̐؂�ւ�
	if (nextScene)
	{
		if (scene)
		{
			scene->Finalize(*this);

		}
		nextScene->Initialize(*this);
		scene = std::move(nextScene);

	}

	// �V�[�����X�V
	if (scene) {
		scene->Update(*this, deltaTime);

	}

	UpdateGameObject(deltaTime);
	HandleGameObjectCollision();
}


/**
* �Q�[���I�u�W�F�N�g�̏Փ˂���������
*/
void Engine::HandleGameObjectCollision()
{
	// ���[���h���W�n�̏Փ˔�����쐬
	std::vector<WorldColliderList> colliders;
	colliders.reserve(gameObjects.size());
	for (const auto& e : gameObjects) {
		if (e->colliders.empty())
		{
			continue;
		}

		// �u�ڒn���Ă��Ȃ��v��Ԃɂ���
		e->isGrounded = false;

		//�Փ˔�����쐬
		WorldColliderList list(e->colliders.size());
		for (int i = 0; i < e->colliders.size(); ++i)
		{
			// �I���W�i���̃R���C�_�[���R�s�[
			list[i].origin = e->colliders[i];
			list[i].world = e->colliders[i]->
				GetTransformedCollider(e->GetTransformMatrix());
		}
		colliders.push_back(list);
	}


	if (colliders.size() >= 2) {
		// �Q�[���I�u�W�F�N�g�P�ʂ̏Փ˔���
		for (auto a = colliders.begin(); a != colliders.end() - 1; ++a)
		{
			const GameObject* goA = a->at(0).origin->GetOwner();
			if (goA->IsDestroyed())
			{
				continue; // �폜�ς݂Ȃ̂Ŕ�΂�

			}
			for (auto b = a + 1; b != colliders.end(); ++b)
			{
				const GameObject* goB = b->at(0).origin->GetOwner();
				if (goB->IsDestroyed())
				{
					continue; // �폜�ς݂Ȃ̂Ŕ�΂�

				}
				HandleWorldColliderCollision(&*a, &*b); // �R���C�_�[�P�ʂ̏Փ˔���
			} // for b

		} // for a

	}
} // HandleGameObjectCollision

/**
* �^�ɂ���Č�������֐����Ăѕ����邽�߂̊֐��e���v���[�g
*/
template<typename T, typename U>
bool CallIntersect(const ColliderPtr& a, const ColliderPtr& b, vec3& p)
{
	return Intersect(static_cast<T&>(*a).GetShape(),
		static_cast<U&>(*b).GetShape(), p);
}


/**
* �^�ɂ���Č�������֐����Ăѕ����邽�߂̊֐��e���v���[�g
*
* ��������֐��ɓn���������t�ɂ���o�[�W����
*/
//AABB�Ƌ��̂̔���͂��邪�A���̂�AABB�̔���͖����̂�
//�t�o�[�W��������`
template<typename T, typename U>
bool CallIntersectReverse(const ColliderPtr& a, const ColliderPtr& b, vec3& p)
{
	if (Intersect(static_cast<U&>(*b).GetShape(),
		static_cast<T&>(*a).GetShape(), p)) {
		p *= -1; // �ђʃx�N�g�����t�����ɂ���
		//�ђʃx�N�g���̌����ƁA����a,b���֌W���Ă��邽��
		//�t�����𔻒肷��Ȃ�A�ђʃx�N�g�����t��
		return true;
		
	}
	return false;
}


/**
* ���false��Ԃ��֐��e���v���[�g
*
* �������܂��͎����\��̂Ȃ��g�ݍ��킹�p
*/
template<typename T, typename U>
bool NotImplemented(const ColliderPtr& a, const ColliderPtr& b, vec3& p)
{
	return false;
}


/**
* �ђʃx�N�g�����Q�[���I�u�W�F�N�g�ɔ��f����
*/
void Engine::ApplyPenetration(WorldColliderList* worldColliders,
	GameObject* gameObject, const vec3& penetration)
{
	// �ڒn����
	// �Փ˃x�N�g���������ɋ߂��ꍇ�ɁA���ɐG�ꂽ�Ƃ݂Ȃ�
	static const float cosGround = cos(radians(30)); // ���Ƃ݂Ȃ��p�x
	static const float sinGround = sin(radians(30)); // ���Ƃ݂Ȃ��p�x
	//�ђʃx�N�g�������炩�ɔ��肪����Ƃ���
	//���炩���ߏ��O���Ă���
	if (penetration.y > 0) {
		// �Ώۂ��P�ʐ����x�N�g���ł��邱�Ƃ𗘗p���āA���ςɂ��p�x�̔�r��P����
		const float d = length(penetration);
		if (penetration.y >= d * cosGround) {
			gameObject->isGrounded = true; // �ڒn����

		}

	}// if penetration.y
	if (penetration.x > 0) {
		// �Ώۂ��P�ʐ����x�N�g���ł��邱�Ƃ𗘗p���āA���ςɂ��p�x�̔�r��P����
		const float d = length(penetration);
		if (penetration.x >= d * sinGround) {
			gameObject->isGrounded = true; // �ڒn����

		}

	}// if penetration.x

// �Q�[���I�u�W�F�N�g���ړ�
	gameObject->position += penetration;

	// �S�Ẵ��[���h�R���C�_�[���ړ�
	for (auto& e : *worldColliders) {
		e.world->AddPosition(penetration);

	}
}

/**
* �R���C�_�[�P�ʂ̏Փ˔���
*
* @param a ����Ώۂ̃��[���h�R���C�_�[�z�񂻂�1
* @param b ����Ώۂ̃��[���h�R���C�_�[�z�񂻂�2
*/
void Engine::HandleWorldColliderCollision(WorldColliderList* a, WorldColliderList* b)
{
	  // �֐��|�C���^�^���`
	using FuncType = bool(*)(const ColliderPtr&, const ColliderPtr&, vec3&);
	
	  // �g�ݍ��킹�ɑΉ������������֐���I�Ԃ��߂̔z��
	//�܂�����Ă��Ȃ��g�ݍ��킹�ɑ΂��ẮA�K��false���������悤�ɂ���
	static const FuncType funcList[3][3] = {
		{
		CallIntersect<AabbCollider, AabbCollider>,
		CallIntersect<AabbCollider, SphereCollider>,
		NotImplemented<AabbCollider, BoxCollider>,
		},
		{
		CallIntersectReverse<SphereCollider, AabbCollider>,
		CallIntersect<SphereCollider, SphereCollider>,
		CallIntersectReverse<SphereCollider, BoxCollider>,
		},
		{
		NotImplemented<BoxCollider, AabbCollider>,
		CallIntersect<BoxCollider, SphereCollider>,
		NotImplemented<BoxCollider, BoxCollider>,
		},
	};
	
	// �R���C�_�[�P�ʂ̏Փ˔���
	for ( auto& colA : *a)
	{
		for ( auto& colB : *b)
		{
			// �X�^�e�B�b�N�R���C�_�[���m�͏Փ˂��Ȃ�
			if (colA.origin->isStatic && colB.origin->isStatic)
			{
				continue;

			}

			// �Փ˔�����s��
			vec3 penetration;
			const int typeA = static_cast<int>(colA.origin->GetType());
			const int typeB = static_cast<int>(colB.origin->GetType());

			//�֐��|�C���^�̓񎟌��z��
			//�R���C�_�[�̌^�ɂ���āA�֐����Ăѕ����Ă���
			if (funcList[typeA][typeB](colA.world, colB.world, penetration)) {
				
				GameObject* goA = colA.origin->GetOwner();
				GameObject* goB = colB.origin->GetOwner();

				// �R���C�_�[���d�Ȃ�Ȃ��悤�ɍ��W�𒲐�
				if (!colA.origin->isTrigger && !colB.origin->isTrigger)
				{
					if (colA.origin->isStatic) {
						// A�͓����Ȃ��̂�B���ړ�������
						
						ApplyPenetration(b, goB, penetration);
					}
					else if (colB.origin->isStatic) {
						//VecMath.h�̕��Œ�`���Ă��������Z�q�ł������肵���R�[�h��
						// B�͓����Ȃ��̂�A���ړ�������
						ApplyPenetration(a, goA, -penetration);
					}
					else {
						// A��B���ϓ��Ɉړ�������
						ApplyPenetration(b, goB, penetration * 0.5f);
						ApplyPenetration(a, goA, penetration * -0.5f);

					}

				}


				// �C�x���g�֐��̌Ăяo��
				goA->OnCollision(colA.origin, colB.origin);
				goB->OnCollision(colB.origin, colA.origin);

				// �C�x���g�̌��ʁA�ǂ��炩�̃Q�[���I�u�W�F�N�g���j�����ꂽ�烋�[�v�I��
				if (goA->IsDestroyed() || goB->IsDestroyed()) {
					return; // �֐����I��
					
				}
			}

		} // for colB

	} // for colA

}


/**
* �j���\��̃Q�[���I�u�W�F�N�g���폜����
*/
void Engine::RemoveDestroyedGameObject()
{
	if (gameObjects.empty()) {
		return; // �Q�[���I�u�W�F�N�g�������Ă��Ȃ���Ή������Ȃ�
	}

	// �j���\��̗L���ŃQ�[���I�u�W�F�N�g�𕪂���
	const auto iter = std::partition(
		gameObjects.begin(), gameObjects.end(),
		[](const GameObjectPtr& e) { return !e->IsDestroyed(); });

	// �j���\��̃I�u�W�F�N�g��ʂ̔z��Ɉړ�
	GameObjectList destroyList(
		std::make_move_iterator(iter),
		std::make_move_iterator(gameObjects.end()));

	// �z�񂩂�ړ��ς݃I�u�W�F�N�g���폜
	gameObjects.erase(iter, gameObjects.end());

	// �j���\��̃I�u�W�F�N�g��OnDestroy�����s
	for (auto& e : destroyList) {
		e->OnDestroy();

	}

	// �����Ŏ��ۂɃQ�[���I�u�W�F�N�g���폜�����(destoyList�̎������I��邽��)
}


/**
* �J�����ɋ߂����C�g��I���GPU�������ɃR�s�[����
*/
void Engine::UpdateShaderLight()
{
	// ������GPU�������ɃR�s�[
	glProgramUniform3fv(prog3D, locAmbientLight, 1, &ambientLight.x);
	
	// ���s�����̃p�����[�^��GPU�������ɃR�s�[
	const vec3 color = directionalLight.color * directionalLight.intensity;
	glProgramUniform3fv(prog3D, locDirectionalLightColor, 1, &color.x);
	glProgramUniform3fv(prog3D, locDirectionalLightDirection,
		1, &directionalLight.direction.x);

		// �R�s�[���郉�C�g���Ȃ���΃��C�g����0�ɐݒ�
	if (usedLights.empty()) {
		glProgramUniform1i(prog3D, locLightCount, 0);
		return;
	}
		
	
		//���g�p�ɂȂ������C�g	�E�E�E�E
		// usedLights�z��Ɋ܂܂�郉�C�g�̂���
		// �uused�����o�ϐ���false�ɂȂ��Ă��郉�C�g�v
		// �g�p�����C�g�̔z�񂩂�A���g�p�ɂȂ������C�g�����O����
		
		const auto itrUnused = std::remove_if(usedLights.begin(), usedLights.end(),
		[&](int i) { return !lights[i].used; });
		//vector�I�u�W�F�N�g�������̏����𖞂����f�[�^���폜���邽�߂�
		//remove_if�֐��@�Ɓ@erase�����o�֐���g�ݍ��킹��
		usedLights.erase(itrUnused, usedLights.end());

		//�d������ԍ�����������
		//�������ꂽ�z�����j�[�N�ɂ�����
		std::sort(usedLights.begin(), usedLights.end());
		//�A��(1,2,3,4,,,)�ŕ���ł��Ȃ��ƍs���Ȃ����߁A
		//���炩���߃\�[�g���Ă���
		
		auto itr = std::unique(usedLights.begin(), usedLights.end());
		usedLights.erase(itr, usedLights.end());
			
		// �J�����̐��ʃx�N�g�����v�Z
		const vec3 front = { -sin(camera.rotation.y), 0, -cos(camera.rotation.y) };

		  // �J�������烉�C�g�܂ł̋������v�Z
		struct Distance 
		{
			float distance;      // �J�������烉�C�g�̔��a�܂ł̋���
			const LightData* p; // ���C�g�̃A�h���X
			
		};
		std::vector<Distance> distanceList;
		distanceList.reserve(lights.size());
		for (auto index : usedLights) 
		{
			const auto& light = lights[index];
			const vec3 v = light.position - camera.position;
			// �J�����̌��ɂ��郉�C�g�����O
			//dot�Z�ŕ��ɂȂ�Ƃ��̓��C�g���J�����̌��ɂ��锻��
			
			if (dot(front, v) <= -light.radius)
			{
				continue;

			}
			const float d = length(v) - light.radius; // �J�������烉�C�g�̔��a�܂ł̋���
			distanceList.push_back({ d, &light });

		}
		
		  // ��ʂɉe�����郉�C�g���Ȃ���΃��C�g����0�ɐݒ�
		if (distanceList.empty())
		{
			glProgramUniform1i(prog3D, locLightCount, 0);
			return;
		
		}
		  // �J�����ɋ߂����C�g��D�悷��
		//���C�g�̋���������ёւ���
		
		std::stable_sort(
			distanceList.begin(), distanceList.end(),

			[&](const auto& a, const auto& b)
			{
				return a.distance < b.distance;
			});

		  // ���C�g�f�[�^��GPU�������ɃR�s�[
		const int lightCount = static_cast<int>(
		std::min(distanceList.size(), maxShaderLightCount)); // �R�s�[���郉�C�g��
		
		std::vector<vec4> colorAndFalloffAngle(lightCount);
		std::vector<vec4> positionAndRadius(lightCount);
		std::vector<vec4> directionAndConeAngle(lightCount);
		
		for (int i = 0; i < lightCount; ++i) {
			const LightData* p = distanceList[i].p;
			const vec3 color = p->color * p->intensity;
			colorAndFalloffAngle[i] = {
			color.x, color.y, color.z, p->falloffAngle 
			};
			positionAndRadius[i] = {
			p->position.x, p->position.y, p->position.z, p->radius 
			};
			directionAndConeAngle[i] = {
			p->direction.x, p->direction.y, p->direction.z, p->coneAngle 
			};

		}
		glProgramUniform4fv(prog3D, locLightColorAndFalloffAngle,
			lightCount, &colorAndFalloffAngle[0].x);

		glProgramUniform4fv(prog3D, locLightPositionAndRadius,
			lightCount, &positionAndRadius[0].x);

		glProgramUniform4fv(prog3D, locLightDirectionAndConeAngle,
			lightCount, &directionAndConeAngle[0].x);

		glProgramUniform1i(prog3D, locLightCount, lightCount);

		// UpdateShaderLight�����o�֐��������ł��B
}


/**
* �X�J�C�X�t�B�A��`�悷��
*/
void Engine::DrawSkySphere()
{
	// �V�[���ɃX�J�C�X�t�B�A���ݒ肳��Ă��Ȃ��ꍇ�͕`�悵�Ȃ�
	if (!skySphere || !scene->skysphereMaterial) {
		return;

	}

	// �A�����b�g�V�F�[�_�ŕ`��
	glUseProgram(progUnlit);
	glBindVertexArray(*meshBuffer->GetVAO());
	glDepthMask(GL_FALSE); // �[�x�o�b�t�@�ւ̏������݂��֎~

	// �X�J�C�X�t�B�A���f���̔��a��0.5m�Ɖ��肵�A�`��͈͂�95%�̑傫���Ɋg��
	static const float far = 1000; // �`��͈͂̍ő�l
	static const float scale = far * 0.95f / 0.5f;
	static const mat4 transformMatrix ={
	vec4(scale, 0,    0,  0),
	vec4(0, scale,    0,  0),
	vec4(0,     0, scale, 0),
	vec4(0,     0,    0,  1),
	};
	glProgramUniformMatrix4fv(progUnlit,
		locTransformMatrix, 1, GL_FALSE, &transformMatrix[0].x);

	// �F�̓}�e���A���J���[�Œ�������̂ŃI�u�W�F�N�g�J���[�͔��ɐݒ�
	static const vec4 color = { 1, 1, 1, 1 };
	glProgramUniform4fv(progUnlit, 100, 1, &color.x);
	
	// �X�J�C�X�t�B�A�̓J���������_�Ƃ��ĕ`��
	static const vec3 skySphereCameraPosition = { 0, 0, 0, };
	glProgramUniform3fv(progUnlit, 4, 1, &skySphereCameraPosition.x);
	
	// �X�J�C�X�t�B�A��`��
	const MaterialList materials(1, scene->skysphereMaterial);
	Draw(*skySphere, progUnlit, materials);
	
	glProgramUniform3fv(progUnlit, 4, 1, &camera.position.x); // �J�������W�����ɖ߂�
	glDepthMask(GL_TRUE); // �[�x�o�b�t�@�ւ̏������݂�����

	//�����Ŗ߂��Ă����Ȃ��ƁA�������`�悪�o���Ȃ��Ȃ��Ă����܂��̂�
	//�����Ɩ߂��Ă���
	glUseProgram(prog3D);

}

/**
* �f�v�X�V���h�E�}�b�v���쐬
*/
void Engine::CreateShadowMap(
	GameObjectList::iterator begin,
	GameObjectList::iterator end)
{
	// �`���t���[���o�b�t�@��ύX
	glBindFramebuffer(GL_FRAMEBUFFER, *fboShadow);

	// �r���[�|�[�g���t���[���o�b�t�@�̃T�C�Y�ɍ��킹��
	const auto& texShadow = fboShadow->GetDepthTexture();
	glViewport(0, 0, texShadow->GetWidth(), texShadow->GetHeight());

	//�J���[�o�b�t�@�������Ȃ����ߐ[�x�����ł���
	// �[�x�o�b�t�@���N���A
	glClear(GL_DEPTH_BUFFER_BIT);

	// VAO�ƃV�F�[�_��OpenGL�R���e�L�X�g�Ɋ��蓖�Ă�
	glBindVertexArray(*meshBuffer->GetVAO());
	glUseProgram(progShadow);


	// �e�̕`��p�����[�^
	const float shadowAreaSize = 100; // �e�̕`��͈�(XY����)
	const float shadowNearZ = 1;      // �e�̕`��͈�(��Z����)
	const float shadowFarZ = 200;     // �e�̕`��͈�(��Z����)
	const float shadowCenterZ = (shadowNearZ + shadowFarZ) * 0.5f; // �`��͈͂̒��S
	const vec3 target = { 0, 0, -50 }; // �J�����̒����_
	const vec3 eye =
		target - directionalLight.direction * shadowCenterZ; // �J�����̈ʒu

	// �r���[�v���W�F�N�V�����s����v�Z
	const mat4 matShadowView = LookAt(eye, target, vec3(0, 1, 0));
	const mat4 matShadowProj = Orthogonal(
		-shadowAreaSize / 2, shadowAreaSize / 2,
		-shadowAreaSize / 2, shadowAreaSize / 2,
		shadowNearZ, shadowFarZ);
	const mat4 matShadow = matShadowProj * matShadowView;

	// �r���[�v���W�F�N�V�����s���GPU�������ɃR�s�[
	glProgramUniformMatrix4fv(progShadow,
		locViewProjectionMatrix, 1, GL_FALSE, &matShadow[0].x);

	// ���b�V����`��
	for (GameObjectList::iterator i = begin; i != end; ++i) {
		const auto& e = *i;
		if (!e->staticMesh) {
			continue;

		}

		glProgramUniformMatrix4fv(progShadow,
			locTransformMatrix, 1, GL_FALSE, &e->GetTransformMatrix()[0].x);
		if (e->materials.empty()) {
			Draw(*e->staticMesh, progShadow, e->staticMesh->materials);

		}
		else {
			Draw(*e->staticMesh, progShadow, e->materials);

		}

	} // for

	// �[�x�e�N�X�`�������蓖�Ă�
	glBindTextureUnit(2, *texShadow);

	// �[�x�e�N�X�`�����W�ւ̕ϊ��s����쐬
	static const mat4 matTexture = {
	{ 0.5f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.5f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.5f, 0.0f },
	{ 0.5f, 0.5f, 0.5f, 1.0f } };

	// �V���h�E�e�N�X�`���s���GPU�������ɃR�s�[
	const mat4 matShadowTexture = matTexture * matShadowProj * matShadowView;
	glProgramUniformMatrix4fv(prog3D,
		locShadowTextureMatrix, 1, GL_FALSE, &matShadowTexture[0].x);

	  // �@�������̕␳�l��ݒ�
	const float texelSize = shadowAreaSize / static_cast<float>(texShadow->GetWidth());
	glProgramUniform1f(prog3D, locShadowNormalOffset, texelSize);

}
/**
* �Q�[���G���W���̏�Ԃ�`�悷��
*/
void Engine::Render()
{

	//�o�b�N�o�b�t�@���N���A
	//�F���w�肷���
	glClearColor(0.1f, 0.2f, 0.5f, 1.0f);
	//�F��`�悵�Ȃ�����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//GPU�������쓮���邽�߂ɂ���

	// �t���[���o�b�t�@�̑傫�����擾
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	const GLuint programs[] = { prog3D, progUnlit,progSkeletal };
	for (auto prog : programs)
	{
		// �A�X�y�N�g��Ǝ���p��ݒ�
		const float aspectRatio =
			static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
		glProgramUniform2f(prog, 3, fovScaleY / aspectRatio, fovScaleY);

		// �J�����p�����[�^��ݒ�
		glProgramUniform3fv(prog, 4, 1, &camera.position.x);
		glProgramUniform2f(prog, 5, sin(-camera.rotation.y), cos(-camera.rotation.y));
		glProgramUniform2f(prog, 7, sin(-camera.rotation.x), cos(-camera.rotation.x));

	}

	// �[�x�e�X�g��L����
	glEnable(GL_DEPTH_TEST);
	
	 // ������������L����
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	
	UpdateShaderLight();
	  // �Q�[���I�u�W�F�N�g�������_�[�L���[���ɕ��בւ���
	std::stable_sort(
		gameObjects.begin(), gameObjects.end(),
		[](const GameObjectPtr& a, const GameObjectPtr& b) {return a->renderQueue < b->renderQueue; }
	);
	
	//���������m�ł̐[�x�o�b�t�@�̏������݂����s���Ă��܂��̂�
	// �[�x�o�b�t�@�̏������݂��֎~�ɂ���
	// 
	// transparent�L���[�̐擪������
	const auto transparentBegin = std::lower_bound(
		gameObjects.begin(), gameObjects.end(), RenderQueue_transparent,
		[](const GameObjectPtr& e, int value) { return e->renderQueue < value; });

	  // overlay�L���[�̐擪������
	const auto overlayBegin = std::lower_bound(
		transparentBegin, gameObjects.end(), RenderQueue_overlay,
		[](const GameObjectPtr& e, int value) { return e->renderQueue < value; });
	
	// �f�v�X�V���h�E�}�b�v���쐬
	CreateShadowMap(gameObjects.begin(), transparentBegin);
	
	  // �`�����f�t�H���g�t���[���o�b�t�@�ɖ߂�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, fbWidth, fbHeight);
	glUseProgram(prog3D);

	//RenderQueue_overlay��UI���ݒ肳��Ă��邽��
	//overlay�ȍ~���A�����b�g�V�F�[�_�ɐ؂�ւ���
	//overlay�����_�[�L���[�ł�3D���f���`������Ȃ��悤��
	glProgramUniform1f(prog3D, locAlphaCutoff, 0.5f);

	// transparent�ȑO�̃L���[��`��
	DrawGameObject(prog3D,gameObjects.begin(), transparentBegin);
	//�V�F�[�_�[�ł̃J�b�g�I�t������
	glProgramUniform1f(prog3D, locAlphaCutoff, 0);
	//�X�J�C�X�t�B�A�̕`��
	DrawSkySphere();

	// transparent����overlay�܂ł̃L���[��`��
	//�[�x�o�b�t�@�ւ̏������݂��֎~�A�܂��͋�����ɂ�
	// glDepthMask(�W�[�G���E�f�v�X�E�}�X�N)�֐����g���܂�
	//
	glDepthMask(GL_FALSE); // �[�x�o�b�t�@�ւ̏������݂��֎~
	DrawGameObject(prog3D, transparentBegin, overlayBegin);
	glDepthMask(GL_TRUE); // �[�x�o�b�t�@�ւ̏������݂�����

	  // overlay�ȍ~�̃L���[��`��
	glDisable(GL_DEPTH_TEST); // �[�x�e�X�g�𖳌���
	glUseProgram(progUnlit);
	DrawGameObject(progUnlit,overlayBegin, gameObjects.end());
	
	//�[�x�o�b�t�@���ǂ̂悤�Ȃ��̂����m�F������A
	//��̃v���O������#if 1��#if 0�ɕύX���āA
	//�V���h�E�}�b�v�̃`�F�b�N�v���O�����𖳌������Ă��������B
	#if 0
	  // �V���h�E�}�b�v�̃`�F�b�N�p
	{
	glBindVertexArray(*meshBuffer->GetVAO());
	MaterialList materials(1, std::make_shared<Material>());
	materials[0]->texBaseColor = fboShadow->GetDepthTexture();
	mat4 m = GetTransformMatrix(vec3(1), vec3(0), vec3(0, 1.5f, -4));
	glProgramUniformMatrix4fv(progUnlit, locTransformMatrix, 1, GL_FALSE, &m[0].x);
	Draw(*meshBuffer->GetStaticMesh("plane_xy"), progUnlit, materials);
	glBindVertexArray(0);
	}
	 #endif
	glfwSwapBuffers(window);//�\���ƕ`����J��Ԃ�
	glfwPollEvents();		//�}�E�X��L�[�{�[�h��Ԃ������Ă����


}

/**
* �Q�[���I�u�W�F�N�g�z���`�悷��
* @param prog  �`��Ɏg���v���O�����p�C�v���C���I�u�W�F�N�g 
* @param begin �`�悷��Q�[���I�u�W�F�N�g�͈͂̐擪
* @param end   �`�悷��Q�[���I�u�W�F�N�g�͈͂̏I�[
*/
void Engine::DrawGameObject(
	GLuint prog,
	GameObjectList::iterator begin,
	GameObjectList::iterator end)
{
	glBindVertexArray(*meshBuffer->GetVAO());

	for (GameObjectList::iterator i = begin; i != end; ++i)
	{
		const auto& e = *i;		// �}�`�ԍ������X�g�ɂȂ��ꍇ�͕`�悵�Ȃ�
		// �X�^�e�B�b�N���b�V�����ݒ肳��Ă��Ȃ��ꍇ�͕`�悵�Ȃ�
		if (!e->staticMesh && !e->skeletalMesh) {
			continue;
		}
		// ���j�t�H�[���ϐ��Ƀf�[�^���R�s�[
		glProgramUniform4fv(prog, 100, 1, &e->color.x);
		
		// ���W�ϊ��x�N�g���̔z���GPU�������ɃR�s�[
		glProgramUniformMatrix4fv(prog, 0, 1, GL_FALSE, &e->GetTransformMatrix()[0].x);
		if (prog != progUnlit) {
			glProgramUniformMatrix3fv(prog, 1, 1, GL_FALSE, &e->GetNormalMatrix()[0].x);
		}
		//2�̕ϊ��x�N�g���̔z���GPU�������ɃR�s�[���邾���ŁA
		//�e�Ǝq�����K�w���낤�Ƃ��������@�\����悤�ɂȂ���

		if (e->staticMesh) {
			// �}�`��`��::glDrawElementsInstanced(GL_TRIANGLES, indexData, GL_UNSIGNED_SHORT, indexData��0�Ԗڂ���i0���Ɓj, 1);
			if (e->materials.empty())
			{
				Draw(*e->staticMesh, prog, e->staticMesh->materials);

			}
			else
			{
				Draw(*e->staticMesh, prog, e->materials);

			}
		}
		
		    // �X�P���^�����b�V����`��
		if (e->skeletalMesh) {
			// ���̊֐ߍs���GPU�������ɃR�s�[
			std::vector<mat4> jointMatrices(4);
			jointMatrices[0] = GetTransformMatrix(vec3(1), { 0.0f, 0, 0 }, vec3(0));
			jointMatrices[1] = GetTransformMatrix(vec3(1), { 0.2f, 0, 0 }, vec3(0));
			jointMatrices[2] = GetTransformMatrix(vec3(1), { -0.2f, 0, 0 }, vec3(0));
			jointMatrices[3] = GetTransformMatrix(vec3(1), { 0.0f, 0, 0 }, vec3(0));
			ssboJointMatrices->CopyData(0, jointMatrices.data(), sizeof(mat4) * 4);

			// �X�P���^�����b�V���p�̃V�F�[�_��VAO�����蓖�Ă�
			glUseProgram(progSkeletal);
			glBindVertexArray(*meshBuffer->GetVAOSkeletal());

			// SSBO���o�C���h
			//GPU���̃V�F�[�_�[�X�g���[�W�u���b�N�Ɋ��蓖��
			glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
				*ssboJointMatrices, 0, sizeof(mat4) * 4);

			// ���W�ϊ��s���GPU�������ɃR�s�[
			glProgramUniformMatrix4fv(progSkeletal, locTransformMatrix,
				1, GL_FALSE, &e->GetTransformMatrix()[0].x);

			// �ŗL�}�e���A��������ΌŗL�}�e���A�����A�Ȃ���΋��L�}�e���A�����g���ĕ`��
			const MaterialList* materials = &e->skeletalMesh->materials;
			if (!e->materials.empty()) {
				materials = &e->materials;

			}
			Draw(e->skeletalMesh->drawParamsList, progSkeletal, *materials, &e->color);

			// ���̃V�F�[�_��VAO�ɖ߂�
			glBindVertexArray(*meshBuffer->GetVAO());
			glUseProgram(prog);

		} // if e->skeletalMesh
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(0);
}

/**
* �Q�[���I�u�W�F�N�g�̏�Ԃ��X�V����
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void Engine::UpdateGameObject(float deltaTime)
{
	// �v�f�̒ǉ��ɑΉ����邽�ߓY����I��
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		GameObject* e = gameObjects[i].get();
		if (!e->IsDestroyed())
		{
			e->Start();
			e->Update(deltaTime);

		}

	}
	
	//�e�q�K�w���ł������߁A�e�������Ǝq���������悤�ɂȂ���
	//����ɂ��A�I�u�W�F�N�g���ƂɑS�Ă̍��W�ϊ��s����v�Z���Ă������߁A������I
	//���P�ĂƂ��āu�����̍��W�ϊ��s����Ɍv�Z���Ă����v
	//�����āA�u�e�̍��W�ϊ��s���K�v�ɂȂ邽�тɌv�Z���Ă����v�Ƃ������ʂ��Ȃ�

	  // ���[�J�����W�ϊ��s����v�Z
	for (int i = 0; i < gameObjects.size(); i++)
	{
		//// ���[�J�����W�ϊ��s��̍쐬
		GameObject* e = gameObjects[i].get();
		e->transformMatrix = GetTransformMatrix(e->scale, e->rotation, e->position);
		e->normalMatrix = GetRotationMatrix(e->rotation);

	}
	
	  // ���[���h���W�ϊ��s����v�Z
	std::vector<mat4> worldTransforms(gameObjects.size());
	std::vector<mat3> worldNormals(gameObjects.size());
	for (int i = 0; i < gameObjects.size(); ++i) {
		//��Ɍv�Z�������[�J�����W�ϊ��s����g���Ă���
		GameObject* e = gameObjects[i].get();
		//�v�Z���ꂽ���[���h���W�ϊ��s��������o�ϐ��ɑ�����Ă��Ȃ�
		//�v�Z�̍Œ��Ƀ��[�J�����W�ϊ��s��㏑������̂͂�NG
		//�����e�������̃Q�[���I�u�W�F�N�g�̍��W�ϊ��s���
		//�v�Z�ł��Ȃ��Ȃ邽�߂ł�����
		mat4 m = e->transformMatrix;
		mat3 n = e->normalMatrix;
		for (e = e->parent; e; e = e->parent) {
			m = e->transformMatrix * m;
			n = e->normalMatrix * n;

		}
		worldTransforms[i] = m;
		worldNormals[i] = n;

	}
	
	  // ���[���h���W�ϊ��s����Q�[���I�u�W�F�N�g�ɐݒ�
	for (int i = 0; i < gameObjects.size(); ++i) {
		gameObjects[i]->transformMatrix = worldTransforms[i];
		gameObjects[i]->normalMatrix = worldNormals[i];

	}
} // UpdateGameObject



