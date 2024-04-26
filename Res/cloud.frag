/**
* @file cloud.frag
*/
#version 450

// �V�F�[�_�ւ̓���
layout(location=0) in vec3 inPosition; // ���[���h���W
layout(location=1) in vec2 inTexcoord; // �e�N�X�`�����W

// �e�N�X�`���T���v��
layout(binding=0) uniform sampler2D texDensity;

// �o�͂���F�f�[�^
out vec4 outColor;

// �v���O��������̓���
layout(location=100) uniform vec3 color; // ���̂̐F

void main()
{
	float density = texture(texDensity, inTexcoord).r;
	outColor = vec4(color, density);
}