/*
@file unlit.vert
*/

#version 450
//unlit �́u�����肪�Ȃ��v�Ƃ����Ӗ�
//���C�e�B���O���g��Ȃ��V�F�[�_

//�V�F�[�_�[�ւ̓���
layout(location =0)in vec3 inPosition;
layout(location=1) in vec2 inTexcoord; // �e�N�X�`�����W

//�V�F�[�_�[����̏o��
//�����Ŋ֌W����荇���Ă���A�֐��Ƃ��̎g�����ł͂ł��Ȃ��i�o���邯�ǎg���Ȃ��j
layout(location=0) out vec3 outPosition; // ���[���h���W
layout(location=1) out vec2 outTexcoord; // �e�N�X�`�����W                                                                                                                                                            

// �v���O��������̓���
layout(location=0) uniform mat4 transformMatrix;
layout(location=1) uniform mat3 normalMatrix;


// �A�X�y�N�g��Ǝ���p�ɂ��g�嗦
layout(location=3) uniform vec2 aspectRatioAndScaleFov;

layout(location=4) uniform vec3 cameraPosition; // �J�����̈ʒu
layout(location=5) uniform vec2 cameraSinCosY;  // �J������Y����]


/**
* �x�N�g������]������
*
* @param v       ��]������x�N�g��
* @param sinCosX X����]�Ɏg�p����sin��cos�̒l
* @param sinCosY Y����]�Ɏg�p����sin��cos�̒l
*
* @return �x�N�g��v���AX��->Y���̏��Ԃŉ�]�������x�N�g��
*/
vec3 RotateXY(vec3 v, vec2 sinCosX, vec2 sinCosY)
{
  // X����]
  v.zy = vec2(
    v.z * sinCosX.y + v.y * sinCosX.x,
    -v.z * sinCosX.x + v.y * sinCosX.y);

  // Y����]
  v.xz = vec2(
    v.x * sinCosY.y + v.z * sinCosY.x,
    -v.x * sinCosY.x + v.z * sinCosY.y);

  return v;
}
void main()
{
    outTexcoord = inTexcoord;
// ���[�J�����W�n���烏�[���h���W�n�ɕϊ�      

       gl_Position = transformMatrix * vec4(inPosition, 1);
       outPosition = gl_Position.xyz;

      // ���[���h���W�n����r���[���W�n�ɕϊ�
       float cameraSinY = cameraSinCosY.x;
       float cameraCosY = cameraSinCosY.y;
       vec3 pos = gl_Position.xyz - cameraPosition;
       gl_Position.x = pos.x * cameraCosY + pos.z * cameraSinY;
       gl_Position.y = pos.y;
       gl_Position.z = pos.x * -cameraSinY + pos.z * cameraCosY;

        gl_Position.xy *= aspectRatioAndScaleFov;

        // �[�x�l�̌v�Z���ʂ�-1�`+1�ɂȂ�悤�ȃp�����[�^A, B���v�Z
        const float near = 0.35;
        const float far = 1000;
        const float A = -2 * far * near / (far - near);
        const float B = (far + near) / (far - near);
       // ���ߖ@��L���ɂ���
        gl_Position.w = -gl_Position.z;
        gl_Position.z = -gl_Position.z * B + A; // �[�x�l��␳
 }