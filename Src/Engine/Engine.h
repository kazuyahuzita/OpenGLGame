/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED
#include "glad/glad.h"
#include "GameObject.h"
#include "Scene.h"
#include "Mesh.h"
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>

class FramebufferObject;
using FramebufferObjectPtr = std::shared_ptr<FramebufferObject>;

// �V�F�[�_�̃��P�[�V�����ԍ�
constexpr GLint locTransformMatrix = 0;
constexpr GLint locNormalMatrix = 1;
constexpr GLint locViewProjectionMatrix = 2;
constexpr GLint locShadowTextureMatrix = 10;
constexpr GLint locShadowNormalOffset = 11;

// �V�F�[�_�Ŏg���郉�C�g�̐�
constexpr size_t maxShaderLightCount = 16;

// ���j�t�H�[���ϐ��̃��P�[�V�����ԍ�
constexpr GLint locCloudColor = 100;

// �V�F�[�_�̃��C�g�z��̃��P�[�V�����ԍ�
constexpr GLint locAlphaCutoff = 102;
//����
constexpr GLint locAmbientLight = 107;
//���s�����p�̃��P�[�V�����ԍ�
constexpr GLint locDirectionalLightColor = 108;
constexpr GLint locDirectionalLightDirection = 109;

constexpr GLint locLightCount = 110;
constexpr GLint locLightColorAndFalloffAngle = 111;

constexpr GLint locLightPositionAndRadius =
locLightColorAndFalloffAngle + maxShaderLightCount;

constexpr GLint locLightDirectionAndConeAngle =
locLightColorAndFalloffAngle + maxShaderLightCount * 2;

// �_����
struct LightData
{
	vec3 color;      // �F
	float intensity; // ���邳
	vec3 position;   // �ʒu
	float radius;    // ���C�g���͂��ő唼�a

	
	  // �X�|�b�g���C�g�p�̕ϐ�
	vec3 direction;     // ���C�g�̌���
	float coneAngle;    // �X�|�b�g���C�g���Ƃ炷�p�x
	float falloffAngle; // �X�|�b�g���C�g�̌����J�n�p�x

	bool used = false; // �g�p�����ǂ���
};

// ���s����
struct DirectionalLight
{
	vec3 color = { 1.00f, 0.98f, 0.95f }; // �F
	float intensity = 1.0f;   // ���邳
	vec3 direction = { 0.58f, -0.58f, 0.58f }; // ����
};

// ���b�V���ԍ�
// Initialize�����o�֐��ɂ���meshes�z��Ə��Ԃ����킹�邱��
// meshes�z��ɕύX���������ꍇ�AMeshid�̏��������l�ɕύX���邱�ƁB
/**
* �Q�[���G���W��
*/
class Engine
{
public:
	Engine() = default;
	~Engine() = default;

	int Run();

	/**
	* �Q�[���I�u�W�F�N�g���쐬����
	*
	* @param name     �I�u�W�F�N�g�̖��O
	* @param tag      �I�u�W�F�N�g��tag��
	* @param position �I�u�W�F�N�g��z�u������W
	* @param rotation �I�u�W�F�N�g�̉�]�p�x
	* @param rotation �I�u�W�F�N�g�̐F
	*/
	template<typename T>
	std::shared_ptr<T> Create(const std::string& name,
		const std::string& tag,
		const vec3& position = { 0, 0, 0 },
		const vec3& rotation = { 0, 0, 0 })
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->tag = tag;
		p->position = position;
		p->rotation = rotation;
		gameObjects.push_back(p); // �G���W���ɓo�^
		return p;
	}


	/**
	* UI�I�u�W�F�N�g���쐬����
	*
	* @tparam T �Q�[���I�u�W�F�N�g�Ɋ��蓖�Ă�UILayout�܂��͂��̔h���N���X
	*
	* @param filename UI�I�u�W�F�N�g�ɕ\������摜
	* @param position UI�I�u�W�F�N�g�̍��W
	* @param scale    UI�I�u�W�F�N�g�̑傫��
	*
	* @return �쐬����UI�I�u�W�F�N�g
	*/
	template<typename T>
	std::pair<GameObjectPtr, std::shared_ptr<T>> CreateUIObject(
		const char* filename, const char* tagname, const vec2& position, float scale)
	{
		auto object = Create<GameObject>(filename,tagname, { position.x, position.y, 0 });
		object->renderQueue = RenderQueue_overlay;
		object->staticMesh = GetStaticMesh("plane_xy");
		
		// �ŗL�}�e���A�����쐬���A�e�N�X�`���������ւ���
		auto texBaseColor = GetTexture(filename);
		object->materials = CloneMaterialList(object->staticMesh);
		object->materials[0]->texBaseColor = texBaseColor;

		// �摜�T�C�Y�ɉ����Ċg�嗦�𒲐�
		const float aspectRatio = texBaseColor->GetAspectRatio();
		object->scale = { scale * aspectRatio, scale, 1 };

		// �R���|�[�l���g��ǉ�
		auto component = object->AddComponent<T>();

		return { object, component };
	}

	
	  /**
	  * �Q�[���I�u�W�F�N�g�z�u�t�@�C���Ɋ܂܂�郁�b�V�����Ǝ��ۂ̃t�@�C�����̑Ή��\
	  *
	  * first=���b�V����, second=���ۂ̃t�@�C����
	  */
	using FilepathMap = std::unordered_map<std::string, std::string>;

	
	  /**
	  * �Q�[���I�u�W�F�N�g�z�u�t�@�C����ǂݍ���
	  *
	  * @param filename �Q�[���I�u�W�F�N�g�z�u�t�@�C����
	  *
	  * @return �t�@�C������ǂݍ��񂾃Q�[���I�u�W�F�N�g�̔z��
	  */
	GameObjectList LoadGameObjectMap(const char* filename, const FilepathMap&);

	// ���ׂẴQ�[���I�u�W�F�N�g���폜����
	void ClearGameObjectAll();

	// ����p�̊Ǘ�
	void SetFovY(float fovY);
	float GetFovY() const { return degFovY; }
	float GetFovScale() const { return fovScaleY; }

	// ���̃V�[����ݒ肷��
	template<typename T>
	void SetNextScene() { nextScene = std::make_shared<T>(); }

	// �J�������擾����
	GameObject& GetMainCamera() { return camera; }
	const GameObject& GetMainCamera() const { return camera; }

	// �L�[��������Ă�����true, ������ĂȂ����false��Ԃ�
	bool GetKey(int key) const
	{
		return glfwGetKey(window, key) == GLFW_PRESS;

	}

	// �L�[��������Ă�����true, ������ĂȂ����false��Ԃ�
	// �}�E�X�J�[�\���̍��W���擾����
	vec2 GetMousePosition() const;

	// �}�E�X�̃{�^����������Ă�����true, ������Ă��Ȃ����false��Ԃ�
	bool GetMouseButton(int button) const;
	
	/**
	* �}�E�X�{�^���̃N���b�N��Ԃ��擾����
	*
	* @param button ���ׂ�{�^���������}�N���萔(GLFW_MOUSE_BUTTON_LEFT��)
	*
	* @retval true  ������Ă���
	* @retval false ������Ă��Ȃ�
	*/
	bool GetMouseClick(int button) const;

	double& GetPreviousTime() { return previousTime; }
	// �t���[���o�b�t�@�̑傫�����擾����
	vec2 GetFramebufferSize() const;
	// �t���[���o�b�t�@�̃A�X�y�N�g����擾����
	float GetAspectRatio() const;

	/**
	* �X�^�e�B�b�N���b�V���̎擾
	*
	* @param name ���b�V����
	*
	* @return ���O��name�ƈ�v����X�^�e�B�b�N���b�V��
	*/
	StaticMeshPtr GetStaticMesh(const char* name)
	{
		return meshBuffer->GetStaticMesh(name);
	}
	
	//�X�P���^�����b�V���ƃX�^�e�B�b�N���b�V���̕`��̈Ⴂ��
	//�V�F�[�_�[�X�g���[�W�u���b�N�̗L���ŁA�`��p�����[�^��`�悷�镔���ɈႢ�͂Ȃ�
	/**
	* �X�P���^�����b�V���̎擾
	*
	* @param name ���b�V����
	*
	* @return ���O��name�ƈ�v����X�P���^�����b�V��
	*/
	SkeletalMeshPtr GetSkeletalMesh(const char* name)
	{
		return meshBuffer->GetSkeletalMesh(name);
	}
	
	/**
	* �e�N�X�`���̎擾
	*
	* @param name �e�N�X�`���t�@�C����
	*
	* @return ���O��name�ƈ�v����e�N�X�`��
	*/
	TexturePtr GetTexture(const char* name);

	/**
	* ���C�g�z�������������
	*/
	void InitializeLight();
	
	/**
	* �V�������C�g���擾����
	*
	* @return ���C�g�̃C���f�b�N�X
	*/
	int AllocateLight();
	
	/**
	* ���C�g���������
	*
	* @param index ������郉�C�g�̃C���f�b�N�X
	*/
	void DeallocateLight(int index);
	
	/**
	* �C���f�b�N�X�ɑΉ����郉�C�g�f�[�^���擾����
	*
	* @param index ���C�g�̃C���f�b�N�X
	*
	* @retval nullptr�ȊO index�ɑΉ����郉�C�g�f�[�^�̃A�h���X
	* @retval nullptr     �Ή����郉�C�g�f�[�^�����݂��Ȃ�
	*/
	LightData* GetLight(int index);
	const LightData* GetLight(int index) const;

	/**
	* �}�E�X���W���甭�˂����������擾����
	*
	* @return �}�E�X���W���甭�˂�������
	*/
	Ray GetRayFromMousePosition() const;

	// �����̌������茋��
	struct RaycastHit
	{
		ColliderPtr collider;     // �ŏ��Ɍ����ƌ��������R���C�_�[
		vec3 point;               // �ŏ��̌�_�̍��W
		float distance;           // �ŏ��̌�_�܂ł̋���
	};

	//�����Ƃ̌���������s���R���C�_�[���ǂ����𔻒肷��u�q��v
	//�u�q��v�́u��������������𖞂������ǂ�����bool�ŕԂ��֐��v�̂���
	//��{�I�ɂ͕ʂ̊֐��Ɉ����Ƃ��ēn����܂��B
	//�q�ꂪ�P�̂̊֐��Ƃ��Ďg���邱�Ƃ͂Ȃ�
	//std::find_if��std::sort�ɓn�������_������
	/**
	* ��������̑ΏۂɂȂ邩�ǂ����𒲂ׂ�q��^
	*
	* @param collider ���肷��R���C�_�[
	* @param distance �����ƃR���C�_�[�̌�_�܂ł̋���
	*
	* @retval true  ��������̑ΏۂɂȂ�
	* @retval false ��������̑ΏۂɂȂ�Ȃ�
	*/
	using RaycastPredicate =
		std::function<bool(const ColliderPtr& collider, float distance)>;


	/**
	* �����ƃR���C�_�[�̌�������
	*
	* @param ray     ����
	* @param hitInfo �����ƍŏ��Ɍ��������R���C�_�[�̏��
	* @param pred    ����������s���R���C�_�[��I�ʂ���q��
	*
	* @retval true  �R���C�_�[�ƌ�������
	* @retval false �ǂ̃R���C�_�[�Ƃ��������Ȃ�����
	*/
	bool Raycast(const Ray& ray, RaycastHit& hitInfo,
		const RaycastPredicate& pred) const;



private:
	// �R���X�g���N�^�A�f�X�g���N�^���Ăׂ�悤�ɂ��邽�߂̕⏕�N���X
	//��̃e�N�X�`�����쐬���邽��
	struct TexHelper : public Texture {
		TexHelper(const char* p) : Texture(p) {}
		TexHelper(const char* p, int w, int h, GLenum f) : Texture(p, w, h, f) {}

	};
	// ���[���h���W�n�̃R���C�_�[��\���\����
	struct WorldCollider
	{
		//�R���C�_�[���m���d�Ȃ�Ȃ��悤�ɂ���
		// ���W��ύX����
		void AddPosition(const vec3& v) {
			origin->GetOwner()->position += v;
			world->AddPosition(v);
		}

		ColliderPtr origin;
		ColliderPtr world;
	};
	using WorldColliderList = std::vector<WorldCollider>;
	void ApplyPenetration(WorldColliderList*, GameObject*, const vec3&);

	int Initialize();
	void Update();
	void Render();
	void DrawGameObject(GLuint prog,
		GameObjectList::iterator begin,
		GameObjectList::iterator end);

	void UpdateGameObject(float deltaTime);
	void HandleGameObjectCollision();
	void HandleWorldColliderCollision(
		WorldColliderList* a, WorldColliderList* b);
	void RemoveDestroyedGameObject();
	void UpdateShaderLight();
	void DrawSkySphere();
	void CreateShadowMap(
		GameObjectList::iterator begin,
		GameObjectList::iterator end);


	GLFWwindow* window = nullptr;				//�E�B���h�E�I�u�W�F�N�g
	const std::string title = "OpenGLGame";		//�E�B���h�E�^�C�g��
	float degFovY = 60; // ��������p(�x���@)
	float radFovY = degFovY * 3.1415926535f / 180;
	float fovScaleY = 1 / tan(radFovY / 2); // ����p�ɂ��g�嗦�̋t��

	
	// �A�����b�g�E�V�F�[�_
	GLuint vsUnlit = 0;
	GLuint fsUnlit = 0;
	GLuint progUnlit = 0;

	// �X�P���^�����b�V���E�V�F�[�_
	GLuint vsSkeletal = 0;
	GLuint fsSkeletal = 0;
	GLuint progSkeletal = 0;

	  // �V���h�E�E�V�F�[�_
	GLuint vsShadow = 0;
	GLuint fsShadow = 0;
	GLuint progShadow = 0;
	FramebufferObjectPtr fboShadow; // �f�v�X�V���h�E�pFBO

	GLuint vs = 0;			// ���_�V�F�[�_�̊Ǘ��ԍ�
	GLuint fs = 0;			// �t���O�����g�V�F�[�_�̊Ǘ��ԍ�
	GLuint prog3D = 0;		// �V�F�[�_�v���O�����̊Ǘ��ԍ�
	MeshBufferPtr meshBuffer;   // �}�`�f�[�^�Ǘ��I�u�W�F�N�g
	
	// �֐ߍs��̔z��̍ő�o�C�g��
	static constexpr size_t jointMatricesBytes = 1024 * 1024;
	BufferObjectPtr ssboJointMatrices; // �֐ߍs��̔z��p�̃o�b�t�@


	std::unordered_map<std::string, TexturePtr> textureCache;

	GameObjectList gameObjects; // �Q�[���I�u�W�F�N�g�z��
	double previousTime = 0; // �O��X�V���̎���
	float deltaTime = 0;     // �O��X�V����̌o�ߎ���

	ScenePtr scene;     // ���s���̃V�[��
	ScenePtr nextScene; // ���̃V�[��

	//�{�^����������Ă���A��莞�Ԉȓ��Ƀ{�^���������ꂽ�ꍇ�́u�N���b�N�v�Ƃ��Ĕ��肷��
	// �N���b�N�Ɣ��肷�鑬�x
	static constexpr float mouseClickSpeed = 0.3f;

	// �}�E�X�{�^���̏��
	struct MouseButton {
		bool current = false;  // ���݃t���[���̃{�^���̏��
		bool previous = false; // �O�t���[���̃{�^���̏��
		bool click = false;    // �N���b�N�̏��
		float timer = 0;       // �{�^����������Ă��鎞��

	};
	MouseButton mouseButtons[3];

	//loc��location�̗��Ƃ��Ďg���Ă���
	std::vector<LightData> lights; // ���C�g�f�[�^�̔z��
	std::vector<int> usedLights;    // �g�p���̃��C�g�̃C���f�b�N�X�z��
	std::vector<int> freeLights;  // ���g�p�̃��C�g�̃C���f�b�N�X�z��
	
	// ��x�ɑ��₷���C�g��
	static constexpr size_t lightResizeCount = 100;
	
	// ���s����
	DirectionalLight directionalLight;

	// �X�J�C�X�t�B�A�p���f��
	StaticMeshPtr skySphere;
	StaticMeshPtr cloud;

	//�J�����I�u�W�F�N�g
	GameObject camera;
	GLsizei indexCount = 0;
public:
	// ����
	 vec3 ambientLight = vec3(0.50f, 0.50f, 0.50f);
	//���ʂ̒���
	float MusicVolume = 0.4f;

private:
	int move = 0;
	int range = 128;

};

#endif // ENGINE_H_INCLUDED