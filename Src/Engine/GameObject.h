/**
* @file GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include"Component.h"
#include "Texture.h"
#include"VecMath.h"
#include "AabbCollider.h"
#include<string>
#include<vector>
#include<memory>
#include <type_traits>

// ��s�錾
class Engine;
class Controller;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>; // �Q�[���I�u�W�F�N�g�̃|�C���^
using GameObjectList = std::vector<GameObjectPtr>; // �Q�[���I�u�W�F�N�g�|�C���^�̔z��
class GameObjectSettingsJson;
struct StaticMesh;
using StaticMeshPtr = std::shared_ptr<StaticMesh>; // �X�^�e�B�b�N���b�V���̃|�C���^
struct SkeletalMesh;
using SkeletalMeshPtr = std::shared_ptr<SkeletalMesh>; // �X�P���^�����b�V���̃|�C���^
struct Material;
using MaterialPtr = std::shared_ptr<Material>; // �}�e���A���̃|�C���^
using MaterialList = std::vector<MaterialPtr>; // �}�e���A���z��

/**
* �`��̏���
*/
enum RenderQueue
{
	RenderQueue_geometry = 2000, // ��ʓI�Ȑ}�`
	/*
	*�`�揇�̎��s�ŁA���S�ȓ����̕��ł��J���[�o�b�t�@�Ɛ[�x�o�b�t�@�ɏ������܂�Ă��܂����߁A
	* ��Ɋ��S�ɕs�����ȃ��f����`�悵�A���̌�Ŕ������ȕ����������f����`�悷��B
	*/
	RenderQueue_transparent = 3000, // �������Ȑ}�`
	RenderQueue_overlay = 4000, // UI, �S��ʃG�t�F�N�g�Ȃ�
	RenderQueue_max = 5000, // �L���[�̍ő�l
};

/**
* �Q�[���ɓo�ꂷ�邳�܂��܂ȃI�u�W�F�N�g��\����{�N���X
*/
class GameObject
{
	friend Engine;
	friend GameObjectSettingsJson;
public:
	GameObject() = default;
	virtual ~GameObject();

	//�f�X�g���N�^���ŁA�e�q�K�w�̉������s���Ă��邽�߁A
	// �|�C���^�̏��L���𑼂̐l�Ɏ������Ƃ܂����̂�,
	  // �R�s�[�Ƒ�����֎~
	GameObject(GameObject&) = delete;
	GameObject& operator=(GameObject&) = delete;

	// �Q�[���G���W�����擾
	Engine* GetEngine() const { return engine; }
	// �R���g���[���[�w�b�_�[���擾
	Controller* GetController() const { return controller; }

	// �Q�[���I�u�W�F�N�g���G���W������폜����
	void Destroy() { isDestroyed = true; }
	// �Q�[���I�u�W�F�N�g���j�󂳂�Ă�����true��Ԃ�
	bool IsDestroyed() const { return isDestroyed; }

	// �Q�[���I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�����
	template<typename T>
	std::shared_ptr<T> AddComponent()
	{
		auto p = std::make_shared<T>();
		p->owner = this;
		if constexpr (std::is_base_of_v<Collider, T>) {
			colliders.push_back(p);
		}
		components.push_back(p);
		p->Awake();
		return p;
	}


	/**
	* �R���|�[�l���g����������
	*
	* �����̃R���|�[�l���g�����Q�[���I�u�W�F�N�g�ɑ΂��Ďg�p����ƁA
	*  �����Ɏ��Ԃ�������ꍇ������̂Œ��ӁB
	*
	* @tparam T ��������R���|�[�l���g�̌^
	*
	* @retval nullptr�ȊO T�^�̍ŏ��̃R���|�[�l���g�ւ̃|�C���^
	* @retval nullptr    T�^�̃R���|�[�l���g�������Ă��Ȃ�
	*/
	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		for (auto& e : components) {
			// shared_ptr�̏ꍇ��dynamic_cast�ł͂Ȃ�dynamic_pointer_cast���g��
			//shared_ptr�͒ʏ�̃|�C���^�ł͂Ȃ��A�N���X�Ȃ̂�dynamic_cast�ł͌ĂׂȂ�
			auto p = std::dynamic_pointer_cast<T>(e);
			if (p) {
				return p; // ���������R���|�[�l���g��Ԃ�

			}

		}
		//�|�C���^��������Ȃ�������A��̃|�C���^��Ԃ�
		return nullptr; // ������Ȃ������̂�nullptr��Ԃ�
	}
	//���I�L���X�g�͎��Ԃ������邽�ߔ�����ׂ������A�I�u�W�F�N�g�̃|�C���^����A
	//���̃I�u�W�F�N�g������̃N���X���p�����Ă��邩�ǂ�����,�m�肽���Ƃ��ɂ͎d�����Ȃ�


	// �Q�[���I�u�W�F�N�g����R���|�[�l���g���폜����
	void RemoveDestroyedComponent();

	// �C�x���g����
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void OnCollision(
		const ComponentPtr& self, const ComponentPtr& other);
	virtual void OnDestroy();

	/*
	* �e�I�u�W�F�N�g���擾����
	*
	* @retval nullptr     �e�����Ȃ�
	* @retval nullptr�ȊO �e�I�u�W�F�N�g�̃A�h���X
	*/
	GameObject* GetParent() const { return parent; }

	/**
	* �e�I�u�W�F�N�g��ݒ肷��
	*
	* @param parent �e�ɂ���Q�[���I�u�W�F�N�g
	*               nullptr���w�肷��Ɛe�q�֌W����������
	*/
	void SetParent(GameObject* parent);
	void SetParent(const GameObjectPtr& parent);

	/**
	* �q�I�u�W�F�N�g�̐����擾����
	*
	* @return �q�I�u�W�F�N�g�̐�
	*/
	size_t GetChildCount() const { return children.size(); }

	/**
	* �q�I�u�W�F�N�g���擾����
	*
	* @param index �q�I�u�W�F�N�g�z��̓Y��
	*              GetChildCount���Ԃ��ȏ�̒l��n���ƃG���[�ɂȂ�̂Œ���
	*
	* @return index�Ԗڂ̎q�I�u�W�F�N�g
	*/
	GameObject* GetChild(size_t index) const { return children[index]; }
	
	// ���W�ϊ��s����擾����
	const mat4 & GetTransformMatrix() const { return transformMatrix; }
	
	  // �@���ϊ��s����擾����
	const mat3 & GetNormalMatrix() const { return normalMatrix; }

	std::string name;                // �I�u�W�F�N�g��
	std::string tag;				 // �I�u�W�F�N�g��
	vec3 position = { 0, 0, 0 };     // ���̂̈ʒu
	vec3 rotation = { 0, 0, 0 };     // ���̂̉�]�p�x
	vec3 scale = { 1, 1, 1 };		 // ���̂̊g�嗦
	vec4 color = { 1, 1, 1, 1 };	 // ���̂̐F
	StaticMeshPtr staticMesh;        // �\������X�^�e�B�b�N���b�V��
	SkeletalMeshPtr skeletalMesh;    // �\������X�P���^�����b�V��
	MaterialList materials;          // �Q�[���I�u�W�F�N�g�ŗL�̃}�e���A���z��
	int renderQueue = RenderQueue_geometry; // �`�揇
	bool isGrounded = false;  // �n�ʂɂ��Ă��邩�ǂ���
	bool isSlided = false;  // ���鏰�ɂ��邩�ǂ���

private:
	Engine* engine = nullptr;				// �G���W���̃A�h���X
	Controller* controller = nullptr;        // �R���g���[���[�w�b�_�[�̃A�h���X
	bool isDestroyed = false;        // ���S�t���O
	//�s����Ǘ����郁���o�ϐ��ł͂��邪�A
	//�v�Z�p�Ƃ��ł͂Ȃ��A��x�v�Z�����l���A�o���Ă��炤���߂Ɏg��
	mat4 transformMatrix = mat4(1);  // ���W�ϊ��s��
	mat3 normalMatrix = mat3(1);     // �@���ϊ��s��

	GameObject* parent = nullptr;      // �e�I�u�W�F�N�g
	std::vector<GameObject*> children; // �q�I�u�W�F�N�g

	std::vector<ComponentPtr> components;	// �R���|�[�l���g�z��
	std::vector<ColliderPtr> colliders;   // �R���C�_�[�z��
};

#endif // GAMEOBJECT_H_INCLUDED