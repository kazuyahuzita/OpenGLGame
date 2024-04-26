/**
* @file FramebufferObject.cpp
*/
#include "FramebufferObject.h"
 #include "Debug.h"

/**
* �����̃e�N�X�`������FBO���쐬����R���X�g���N�^
* 
* @param �쐬����FBO�̐�
* @param �쐬����FBO��ID���i�[����ϐ��A�h���X
*/
FramebufferObject::FramebufferObject(
	const TexturePtr& color, const TexturePtr& depth)
	: texColor(color), texDepth(depth)
{
	glCreateFramebuffers(1, &fbo);

	// �J���[�o�b�t�@��ݒ�
	if (color) {
		//�K�v�ɉ����ăJ���[�e�N�X�`����[�x�e�N�X�`�������蓖��
		glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, *color, 0);
		width = texColor->GetWidth();
		height = texColor->GetHeight();

	}
	else {

		//GL_NONE(�W�[�G���E�m��): �J���[��`�悵�Ȃ�v
		//GL_COLOR_ATTACHIMENT0�`GL_COLOR_ATTACHIMENT31: �J���[�o�b�t�@
		//�J���[�͕`�悵�Ȃ�
		glNamedFramebufferDrawBuffer(fbo, GL_NONE);

	}

	// �[�x�o�b�t�@��ݒ�
	if (depth) {

		glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, *depth, 0);
		width = texDepth->GetWidth();
		height = texDepth->GetHeight();

	}

	// FBO�̃G���[�`�F�b�N
	if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glDeleteFramebuffers(1, &fbo);
		fbo = 0;
		texColor.reset();
		texDepth.reset();
		LOG_ERROR("FBO�̍쐬�Ɏ��s");
		return;

	}

	// �쐬����
	LOG("FBO(width=%d, height=%d)���쐬", width, height);

}

/**
* �f�X�g���N�^
*/
FramebufferObject::~FramebufferObject()
{
	glDeleteFramebuffers(1, &fbo);
}