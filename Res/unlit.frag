/**
* @file unlit.frag
*/
#version 450

// �V�F�[�_�ւ̓���
layout(location=0) in vec3 inPosition; // ���[���h���W
layout(location=1) in vec2 inTexcoord; // �e�N�X�`�����W

// �e�N�X�`���T���v��
layout(binding=0) uniform sampler2D texColor;
layout(binding=1) uniform sampler2D texEmission;

// �o�͂���F�f�[�^
out vec4 outColor;

// �v���O��������̓���
layout(location=100) uniform vec4 color; // ���̂̐F
layout(location=101) uniform vec4 emission; // ���̂̕��ːF

void main()
{
  vec4 c = texture(texColor, inTexcoord);

  // �e�N�X�`���̃K���}�␳������
  const float crtGamma = 2.2;
  c.rgb = pow(c.rgb, vec3(2.2));

  outColor = c * color;
  if (emission.w > 0) 
    {
      outColor.rgb += texture(texEmission, inTexcoord).rgb * emission.rgb;
    } else {
      outColor.rgb += emission.rgb;
    }

  // �K���}�␳���s��
  outColor.rgb = pow(outColor.rgb, vec3(1 / crtGamma));
}