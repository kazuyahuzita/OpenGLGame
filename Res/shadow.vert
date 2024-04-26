/**
* @file shadow.vert
*/
#version 450

// �V�F�[�_�ւ̓���
//�J���[�o�b�t�@(���Ȃ킿�@��)�͕s�v�I�I
layout(location=0) in vec3 inPosition; // ���_���W
layout(location=1) in vec2 inTexcoord; // �e�N�X�`�����W

// �V�F�[�_����̏o��
layout(location=1) out vec2 outTexcoord; // �e�N�X�`�����W

// �v���O��������̓���
//1�Ԃ�standard�V�F�[�_�[��normalMatrix������
layout(location=0) uniform mat4 transformMatrix; // ���W�ϊ��s��
layout(location=2) uniform mat4 viewProjectionMatrix; // �r���[�v���W�F�N�V�����s��

void main()
{
  // �e�N�X�`�����W��ݒ�
  outTexcoord = inTexcoord;

  // ���[�J�����W�n���烏�[���h���W�n�ɕϊ�
  gl_Position = transformMatrix * vec4(inPosition, 1);
  gl_Position = viewProjectionMatrix * gl_Position;
}