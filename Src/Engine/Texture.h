/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
 #define TEXTURE_H_INCLUDED
 #include "glad/glad.h"
 #include <string>
 #include <memory>

// ��s�錾
class Texture;
using TexturePtr = std::shared_ptr<Texture>;

/**
* �e�N�X�`���Ǘ��N���X
*/
class Texture
{
	//�w�肵���N���X���v���C�x�[�g�����o�ŌĂяo��
	friend class Engine;
public:
	// �R�s�[�Ƒ�����֎~

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// �Ǘ��ԍ����擾

	operator GLuint() const { return id; }

	// ���ƍ������擾
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }


	// �摜�̃A�X�y�N�g����擾
	float GetAspectRatio() const {
		return static_cast<float>(width) / static_cast<float>(height);


	}

private:
	//�v���C�x�[�g�ɒu�����ƂŁA
	//Texture�N���X���g��Engine�N���X�ȊO��
	//Texture�I�u�W�F�N�g�����Ȃ��Ȃ�
	explicit Texture(const char* filename);

	/**
	* ��̃e�N�X�`�����쐬����R���X�g���N�^
	*
	* @param name      �e�N�X�`�����ʗp�̖��O
	* @param width     �e�N�X�`���̕�(�s�N�Z����)
	* @param height    �e�N�X�`���̍���(�s�N�Z����)
	* @param gpuFormat �f�[�^�`��
	* @param wrapMode  ���b�v���[�h
	*/
	Texture(const char* name, int width, int height,
		GLenum gpuFormat, GLenum wrapMode = GL_CLAMP_TO_EDGE);

	~Texture();

	std::string name; // �e�N�X�`����(��Ƀf�o�b�O�p)
	GLuint id = 0;    // �I�u�W�F�N�g�Ǘ��ԍ�
	int width = 0;    // �e�N�X�`���̕�
	int height = 0;   // �e�N�X�`���̍���
};

#endif // TEXTURE_H_INCLUDED