#ifndef VERTEXARRAYOBJECT_H_INCLUDED
#define VERTEXARRAYOBJECT_H_INCLUDED
#include "glad/glad.h"
#include <memory>

// ��s�錾
class VertexArrayObject;
using VertexArrayObjectPtr = std::shared_ptr<VertexArrayObject>;

/**
* ���_�z��I�u�W�F�N�g(VAO)
*
* ���_�o�b�t�@�Ɋi�[����Ă��钸�_�f�[�^�ɂ��āA
* �e�v�f���ǂ̂悤�ɔz�u����Ă��邩���L�q����I�u�W�F�N�g�B
*/
class VertexArrayObject
{
public:
	/**
	* VAO���쐬����
	*
	* @return �쐬����VAO�ւ̃|�C���^
	*/
	static VertexArrayObjectPtr Create()
	{
		return std::make_shared<VertexArrayObject>();
	}

	// �R���X�g���N�^�A�f�X�g���N�^
	VertexArrayObject() { glCreateVertexArrays(1, &id); }
	~VertexArrayObject() { glDeleteVertexArrays(1, &id); }

	// �R�s�[�Ƒ�����֎~
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	// �Ǘ��ԍ����擾
	operator GLuint() const { return id; }

	/**
	* ���_�A�g���r���[�g��ݒ�
	*
	* @param index  �ݒ肷�钸�_�A�g���r���[�g�̔ԍ�(0�`15)
	* @param size   �v�f�̃f�[�^��(float=1, vec2=2, vec3=3, vec4=4)
	* @param stride ���̒��_�f�[�^�ɂ��铯��v�f�܂ł̊Ԋu(�o�C�g��)
	* @param offset ���_�f�[�^���ɂ�����v�f�̈ʒu(�o�C�g��)
	*/
	void SetAttribute(
		GLuint index, GLint size, size_t stride, size_t offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE,
			static_cast<GLsizei>(stride), reinterpret_cast<void*>(offset));
	};

private:
	GLuint id = 0; // �I�u�W�F�N�g�Ǘ��ԍ�
};

#endif // VERTEXARRAYOBJECT_H_INCLUDED