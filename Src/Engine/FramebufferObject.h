/**
* @file FramebufferObject.h
*/
#ifndef FRAMEBUFFEROBJECT_H_INCLUDED
 #define FRAMEBUFFEROBJECT_H_INCLUDED
 #include "glad/glad.h"
 #include "Texture.h"

/**
* �t���[���o�b�t�@�E�I�u�W�F�N�g(FBO)
*/
class FramebufferObject
{
public:
	/**
	* �e�N�X�`������FBO���쐬����R���X�g���N�^
	*
	* @param color �J���[�e�N�X�`��
	* @param depth �[�x�e�N�X�`��
	*/
	FramebufferObject(const TexturePtr& color, const TexturePtr& depth);
	~FramebufferObject();

	// �R�s�[�Ƒ�����֎~
	FramebufferObject(const FramebufferObject&) = delete;
	FramebufferObject& operator=(const FramebufferObject&) = delete;

	// �Ǘ��ԍ����擾
	operator GLuint() const { return fbo; }

	// ���ƍ������擾
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	// �e�N�X�`�����擾
	const TexturePtr& GetColorTexture() const { return texColor; }
	const TexturePtr& GetDepthTexture() const { return texDepth; }

private:
	GLuint fbo = 0;      // FBO�̊Ǘ��ԍ�
	int width = 0;       // FBO�̕�
	int height = 0;      // FBO�̍���
	TexturePtr texColor; // �J���[�e�N�X�`��
	TexturePtr texDepth; // �[�x�e�N�X�`��
};
using FramebufferObjectPtr = std::shared_ptr<FramebufferObject>;

#endif // FRAMEBUFFEROBJECT_H_INCLUDED