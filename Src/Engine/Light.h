/**
* @file Light.h
*/
#ifndef LIGHT_H_INCLUDED
 #define LIGHT_H_INCLUDED
 #include "Component.h"
 #include "VecMath.h"

/**
* ���C�g�E�R���|�[�l���g
* 
* Awake��Engine���烉�C�g�C���f�b�N�X���擾���A
* Update�Ŏ��g�ɐݒ肳�ꂽ�l��
* �C���f�b�N�X�ɑΉ����郉�C�g�ɔ��f����܂�
* �擾�������C�g�C���f�b�N�X��Engine����f�X�g���N�^�ŌĂ΂ꂽ��Ԃ�
*/
class Light : public Component
{
public:
	Light() = default;
	virtual ~Light();
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	
	//�V�F�[�_�[���Ō����������������A�G��₷���悤�Ɍʂ̕ϐ��ɂ���
	//�f�[�^�������s��Ȃ��N���X�Ȃ̂Łu���삵�₷���v���d��
	  // ���C�g�̎��
	enum class Type
	{
		pointLight, // �_����
		spotLight,  // �X�|�b�g����
	};
	Type type = Type::pointLight; // ���C�g�̎��

	vec3 color = { 1, 1, 1 }; // �F
	float intensity = 1; // ���邳
	float radius = 1;    // ���C�g���͂��ő唼�a

	
	// �X�|�b�g���C�g�p�̃p�����[�^
	// ������GameObject::rotation����v�Z
	float coneAngle = radians(30);    // �X�|�b�g���C�g���Ƃ炷�p�x
	float falloffAngle = radians(20); // �X�|�b�g���C�g�̌����J�n�p�x

private:
	int lightIndex = -1; // ���C�g�C���f�b�N�X
};

#endif // LIGHT_H_INCLUDED