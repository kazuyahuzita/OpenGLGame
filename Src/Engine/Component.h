/**
* @file Component.h
*/
#ifndef COMOPNENT_H_INCLUDED
 #define COMOPNENT_H_INCLUDED
 #include <memory>

// ��s�錾
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;
class Component;
using ComponentPtr = std::shared_ptr<Component>; // �R���|�[�l���g�̃|�C���^

/**
* �R���|�[�l���g�̊��N���X
*/
class Component
 {
	friend GameObject;
public:
	Component() = default;
	virtual ~Component() = default;
	
	  // �R���|�[�l���g�̏��L�҂��擾
	GameObject * GetOwner() const { return owner; }
	
	  // �R���|�[�l���g���Q�[���I�u�W�F�N�g����폜����
	void Destroy() { isDestroyed = true; }
	
	  // �R���|�[�l���g���j�󂳂�Ă�����true��Ԃ�
	bool IsDestroyed() const { return isDestroyed; }
	
	  // �Q�[���I�u�W�F�N�g�ɒǉ����ꂽ���ɌĂяo�����
	virtual void Awake() {}
	
	  // �ŏ���Update�̒��O�ŌĂяo�����
	virtual void Start() {}
	
	  // ���t���[��1��Ăяo�����
	virtual void Update(float deltaTime) {}
	
	//�G���v���C���[��ǐՂ���
	virtual void Tracking(float positionX, float positionZ, float deltaTime) {}

	  // �Փ˂��N�����Ƃ��ɌĂяo�����
	virtual void OnCollision(
	const ComponentPtr & self, const ComponentPtr & other) {}
	
	  // �Q�[���I�u�W�F�N�g���G���W������폜�����Ƃ��ɌĂяo�����
	virtual void OnDestroy() {}
	
private:
	GameObject * owner = nullptr; // ���̃R���|�[�l���g�̏��L��
	bool isStarted = false;      // Start�����s���ꂽ��true�ɂȂ�
	bool isDestroyed = false;    // Destroy�����s���ꂽ��true�ɂȂ�
	bool isTouched = false;    // Destroy�����s���ꂽ��true�ɂȂ�
};

#endif // COMOPNENT_H_INCLUDED