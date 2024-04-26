/**
* @file GameObject.cpp
*/
#include "GameObject.h"
 #include <algorithm>


/**
* �f�X�g���N�^
*/
GameObject::~GameObject()
{
	// ���ʑO�ɐe�q�֌W����������
	SetParent(nullptr);
	for (auto child : children) 
	{
		child->parent = nullptr;

	}
}
/**
* �Q�[���I�u�W�F�N�g����폜�\��̃R���|�[�l���g���폜����
*/
void GameObject::RemoveDestroyedComponent()
{
	if (components.empty()) {
		return; // �R���|�[�l���g�������Ă��Ȃ���Ή������Ȃ�

	}

	// �j���\��̗L���ŃR���|�[�l���g�𕪂���
	const auto iter = std::stable_partition(
		components.begin(), components.end(),
		[](const auto& p) { return !p->IsDestroyed(); });

	// �j���\��̃R���|�[�l���g��ʂ̔z��Ɉړ�
	std::vector<ComponentPtr> destroyList(
		std::move_iterator(iter),
		std::move_iterator(components.end()));

	// �z�񂩂�ړ��ς݃R���|�[�l���g���폜
	components.erase(iter, components.end());
	
	  // �R���C�_�[��colliders�ɂ��o�^����Ă���̂ŁA��������폜����K�v����
	const auto iter2 = std::remove_if(
		colliders.begin(), 
		colliders.end(),
		[](const auto& p) {
			return p->IsDestroyed(); 
		}
	);
	colliders.erase(iter2, colliders.end());


	// �j���\��̃R���|�[�l���g��OnDestory�����s
	for (auto& e : destroyList) {
		e->OnDestroy();

	}

	// �����Ŏ��ۂɃR���|�[�l���g���폜�����(destoyList�̎������I��邽��)
}

/**
* �X�^�[�g�C�x���g
*/
void GameObject::Start()
{
	for (auto& e : components)
	{
		if (!e->isStarted)
		{
			e->Start();
			e->isStarted = true;
		}
	}
}

/**
* �X�V�C�x���g
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void GameObject::Update(float deltaTime)
{
	for (auto& e : components)
	{
		e->Update(deltaTime);
	}
	 RemoveDestroyedComponent();
}

/**
* �Փ˃C�x���g
*
* @param self  �Փ˂����R���|�[�l���g(����)
* @param other �Փ˂����R���|�[�l���g(����)
*/
void GameObject::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	for (auto& e : components)
	{
		e->OnCollision(self, other);
	}
}

/**
* �폜�C�x���g
*/
void GameObject::OnDestroy()
{
	for (auto& e : components) {
		e->OnDestroy();

	}
}
//�����o�֐��𐶃|�C���^�ƃV�F�A�[�h�|�C���^�̗��������Ă���̂�
// �v���O�����������₷�����Ă��邽��

// �e�I�u�W�F�N�g��ݒ肷��
void GameObject::SetParent(GameObject* parent)
{
	// �����e���w�肳�ꂽ�ꍇ�͉������Ȃ�
	if (parent == this->parent) {
		return;

	}

	// �ʂ̐e������ꍇ�A���̐e�Ƃ̊֌W����������
	if (this->parent) {
		// �����̈ʒu������
		auto& c = this->parent->children;
		auto itr = std::find(c.begin(), c.end(), this);
		if (itr != c.end()) {
			c.erase(itr); // �z�񂩂玩�����폜

		}

	}

	// �V���Ȑe�q�֌W��ݒ�
	if (parent) {
		parent->children.push_back(this);

	}
	this->parent = parent;
}
//�V�F�A�[�h�|�C���^�ł̃����o�֐��̖ړI�́A
// �uget���Ăяo����Ԃ��Ȃ��ăv���O�����������₷�����邱�Ɓv
// �e�I�u�W�F�N�g��ݒ肷��
void GameObject::SetParent(const GameObjectPtr& parent)
{
	SetParent(parent.get());
}