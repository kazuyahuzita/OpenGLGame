/**
* @file shadow.frag
*/
#version 450

// �V�F�[�_�ւ̓���
layout(location=1) in vec2 inTexcoord; // �e�N�X�`�����W

// �e�N�X�`���T���v��
layout(binding=0) uniform sampler2D texColor;

// �v���O��������̓���
layout(location=102) uniform float alphaCutoff; // �t���O�����g��j�����鋫�E�l

void main()
{
  vec4 c = texture(texColor, inTexcoord);

  // �J�b�g�I�t�l���w�肳��Ă���ꍇ�A�A���t�@�����̒l�����̏ꍇ�͕`����L�����Z��
  if (alphaCutoff > 0) {
    if (c.a < alphaCutoff) {
      discard; // �t���O�����g��j��(�L�����Z��)
    }
  }
}