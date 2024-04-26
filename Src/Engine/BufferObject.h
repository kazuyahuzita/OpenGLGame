#ifndef BUFFEROBJECT_H_INCLUDED
 #define BUFFEROBJECT_H_INCLUDED
 #include "glad/glad.h"
 #include <memory>

// ��s�錾
class BufferObject;
using BufferObjectPtr = std::shared_ptr<BufferObject>;

/**
* �o�b�t�@�I�u�W�F�N�g(BO)
*/
class BufferObject
{
public:
	/**
	* �o�b�t�@�I�u�W�F�N�g���쐬����
	*
	* @param size  �o�b�t�@�I�u�W�F�N�g�̃o�C�g�T�C�Y
	* @param data  �o�b�t�@�ɃR�s�[����f�[�^�̃A�h���X
	*              nullptr���w�肷��Ƌ�̃o�b�t�@���쐬�����
	* @param flags �p�r�������r�b�g�t���O�̘_���a(glBufferStorage���Q��)
	*
	* @return �쐬�����o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^
	*/
	static BufferObjectPtr Create(GLsizeiptr size,
		const void* data = nullptr, GLbitfield flags = 0)
	{
		return std::make_shared<BufferObject>(size, data, flags);
	}

	// �R���X�g���N�^
	BufferObject(GLsizeiptr size, const void* data, GLbitfield flags)
	{
		bufferSize = size;
		glCreateBuffers(1, &id);

		glNamedBufferStorage(id, bufferSize, data, flags);
	}

	// �f�X�g���N�^
	~BufferObject() { glDeleteBuffers(1, &id); }

	// �R�s�[�Ƒ�����֎~
	BufferObject(const BufferObject&) = delete;
	BufferObject& operator=(const BufferObject&) = delete;

	// �Ǘ��ԍ����擾
	operator GLuint() const { return id; }

	// �o�b�t�@�T�C�Y���擾
	GLsizeiptr GetSize() const { return bufferSize; }

	/**
	* GPU�������Ƀf�[�^���R�s�[����
	*
	* @param offset �R�s�[��(GPU������)�̈ʒu
	* @param data   �R�s�[����f�[�^�̃A�h���X
	* @param size   �R�s�[����o�C�g��
	*/
	void CopyData(size_t offset, const void* data, size_t size)
	{
		//�ꎞ�I��GPU�̈���쐬���Ă���A�����CPU��������̃f�[�^���R�s�[���A
		//��������GPU�̈�ɍēx�R�s�[
		//
		GLuint tmp;
		glCreateBuffers(1, &tmp);
		glNamedBufferStorage(tmp, size, data, 0);
		glCopyNamedBufferSubData(tmp, id, 0, offset, size);
		glDeleteBuffers(1, &tmp);

	}

private:
	GLuint id = 0;             // �I�u�W�F�N�g�Ǘ��ԍ�
	GLsizeiptr bufferSize = 0; // �o�b�t�@�T�C�Y(�o�C�g��)
};

#endif // BUFFEROBJECT_H_INCLUDED