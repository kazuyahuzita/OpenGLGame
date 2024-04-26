/**
* @file Texture.cpp
*/
#include "Texture.h"
#include "Debug.h"
 #include <vector>
 #include <fstream>
 #include <filesystem>

/**
* �R���X�g���N�^
*
* @param filename �e�N�X�`���t�@�C����
*/
Texture::Texture(const char* filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		LOG_ERROR("%s���J���܂���", filename);
		return;
	}

	// �t�@�C����ǂݍ���
	const size_t filesize = std::filesystem::file_size(filename);
	std::vector<uint8_t> buffer(filesize);
	file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
	file.close();

	// �w�b�_��������擾
	const size_t tgaHeaderSize = 18; // �w�b�_���̃o�C�g��
	const int imageOffset = tgaHeaderSize + buffer[0]; // �摜�f�[�^�̈ʒu
	const uint8_t imageType = buffer[2];               // �摜�`��
	const int pixelBytes = (buffer[16] + 7) / 8;       // 1�s�N�Z���̃o�C�g��

	width = buffer[12] + buffer[13] * 256;
	height = buffer[14] + buffer[15] * 256;

	
	  // ���k�`���̏ꍇ�͓W�J����
	  //  0(0b0000): �摜�Ȃ�
	  //  1(0b0001): �C���f�b�N�X(�����k)
	  //  2(0b0010): �g�D���[�J���[(�����k)
	  //  3(0b0011): ����(�����k)
	  //  9(0b1001): �C���f�b�N�X(RLE)
	  // 10(0b1010): �g�D���[�J���[(RLE)
	  // 11(0b1011): ����(RLE)
	if (imageType & 0b1000) {

		// �W�J�p�̃o�b�t�@��p��
		const int imageBytes = width * height * pixelBytes; // �W�J��̃o�C�g��
		std::vector<uint8_t> tmp(imageOffset + imageBytes);

		// swap�ɔ����āA�摜�f�[�^���O�̕������R�s�[
		std::copy_n(buffer.begin(), imageOffset, tmp.begin());

		
		    // �摜�f�[�^��W�J
		const uint8_t * src = buffer.data() + imageOffset; // ���k�f�[�^�̃A�h���X
		uint8_t * dest = tmp.data() + imageOffset;         // �f�[�^�W�J��A�h���X
		const uint8_t* const destEnd = dest + imageBytes; // �W�J�I���A�h���X
		
		while (dest != destEnd) {
			// �p�P�b�g�w�b�_����ID�ƃf�[�^�����擾
			const int isRLE = *src & 0x80;
			const int count = (*src & 0x7f) + 1;
			++src; // �A�h���X���p�P�b�g�f�[�^�̈ʒu�ɐi�߂�

			if (isRLE) {
				// ���k�f�[�^�̏ꍇ�A�p�P�b�g�f�[�^���w��񐔃R�s�[
				for (int a = 0; a < count; ++a) {
					memcpy(dest, src, pixelBytes);
					dest += pixelBytes;

				}
				src += pixelBytes;

			}
			else {
				// �����k�f�[�^�̏ꍇ�A�p�P�b�g�f�[�^�S�̂��R�s�[
				const int dataBytes = pixelBytes * count;
				memcpy(dest, src, dataBytes);
				dest += dataBytes;
				src += dataBytes;

			}

		} // while
			// buffer��tmp�̓��e������
			buffer.swap(tmp);
	}

	// �i�[�������u�ォ�牺�v�̏ꍇ�A�f�[�^���㉺���]
	const bool topToBottom = buffer[17] & 0b0010'0000;
	if (topToBottom) {
		const int pixelDepth = buffer[16];               // 1�s�N�Z���̃r�b�g��
		const int lineByteSize = width * pixelDepth / 8; // 1�s�̃o�C�g��
		uint8_t* top = buffer.data() + tgaHeaderSize;        // ��̍s�̈ʒu
		uint8_t* bottom = top + lineByteSize * (height - 1); // ���̍s�̈ʒu
		std::vector<uint8_t> tmp(lineByteSize);              // �㉺����ւ��p�̃o�b�t�@

		// �s�P�ʂŏ㉺���]
		// �㉺�̍s�̈ʒu���t�]����܂ŌJ��Ԃ�
		while (top < bottom) {
			std::copy_n(top, lineByteSize, tmp.data());    // �u��̍s�̃R�s�[�v�����
			std::copy_n(bottom, lineByteSize, top);        // ���̍s���A��̍s�ɏ㏑��
			std::copy_n(tmp.data(), lineByteSize, bottom); // �u��̍s�̃R�s�[�v�����̍s�ɏ㏑��
			top += lineByteSize;                           // ��̍s�̈ʒu��1�s���Ɉړ�
			bottom -= lineByteSize;                        // ���̍s�̈ʒu��1�s��Ɉړ�

		}

	} // if topToBottom

	
	  // 1�s�N�Z���̃r�b�g���ɑΉ�����`���̃Z�b�g������
	struct Format
	{
		int pixelDepth;     // TGA��1�s�N�Z���̃r�b�g��
		GLenum imageFormat; // TGA�̃f�[�^�`��
		GLenum imageType;   // TGA�̃f�[�^�^
		GLenum gpuFormat;   // GPU���̃f�[�^�`��
	};
	//32,24.16.15.8�r�b�g�ɑΉ�����`���ɂ���
	constexpr Format formatList[] = {
	{ 32, GL_BGRA, GL_UNSIGNED_BYTE,              GL_RGBA8 },
	{ 24, GL_BGR,  GL_UNSIGNED_BYTE,              GL_RGB8 },
	{ 16, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGB5_A1 },
	{ 15, GL_BGR,  GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGB5 },
	{  8, GL_RED,  GL_UNSIGNED_BYTE,              GL_R8 },
	};





	
	const Format * format = 
		std::find_if(formatList, 
			std::end(formatList) - 1,
		[pixelDepth = buffer[16]](const Format& e){ return e.pixelDepth == pixelDepth; });

	// ���݂̃A���C�������g���L�^
	GLint alignment;
	//���݂̃A���C�������g���擾
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	
	// �摜�̃A���C�������g������
	// 1�s�̃o�C�g����4�Ŋ���؂��Ƃ���4�A2�Ŋ���؂��Ƃ���2�A����ȊO��1�ɐݒ�
	constexpr int alignmentList[] = { 4, 1, 2, 1 };
	const int lineByteSize = width * pixelBytes; // 1�s�̃o�C�g��
	const int imageAlignment = alignmentList[lineByteSize % 4];
	
		  // �A���C�������g��ύX
	if (alignment != imageAlignment) 
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, imageAlignment);

	}

	// �e�N�X�`�����쐬
	GLuint object = 0; // �e�N�X�`���̊Ǘ��ԍ�
	glCreateTextures(GL_TEXTURE_2D, 1, &object);
	//format�ϐ��̒l�������ɓn���悤�ɂ���
	glTextureStorage2D(object, 1, format->gpuFormat, width, height);
	glTextureSubImage2D(object, 0, 0, 0, width, height,
		format->imageFormat, format->imageType, buffer.data() + tgaHeaderSize);
	
	 // �A���C�������g�����ɖ߂�
	if (alignment != imageAlignment) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		
	}
	id = object;
	name = filename;

	//�����摜��\�����邽�߂ɁA�Ԃ̗v�f��Ɨ΂ɃR�s�[���āA
	// �\���ł���悤�ɂ���
	// glTextureParameteri���g���āA�f�[�^�̏��������ւ���
	  // �O���[�X�P�[���e�N�X�`���̏ꍇ�A�Ԑ�����΂ƐɃR�s�[����悤�ɐݒ肷��
	if (format->imageFormat == GL_RED)
	{
		glTextureParameteri(object, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTextureParameteri(object, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTextureParameteri(object, GL_TEXTURE_SWIZZLE_B, GL_RED);
	}
	
}

/**
* ��̃e�N�X�`�����쐬����R���X�g���N�^
*/
Texture::Texture(const char* name, int width, int height,
	GLenum gpuFormat, GLenum wrapMode) :
	name(name), width(width), height(height)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, gpuFormat, width, height);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, wrapMode);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, wrapMode);

	// �[�x�e�N�X�`���̏ꍇ�A�V���h�E�}�b�s���O�̂��߂ɔ�r���[�h��ݒ肷��
	switch (gpuFormat) {
	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
		//��q����sampler2DShadow�T���v���̐�������r�����邽�߂�
		//�f�v�X�V���h�E�}�b�s���O���s���ꍇ�́AGL_COMPARE_REF_TO_TEXTURE���w�肵�܂�.

		glTextureParameteri(id, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		break;
	default:
		break;

	}
}
/**
* �f�X�g���N�^
*/
Texture::~Texture()
{
	glDeleteTextures(1, &id);
}