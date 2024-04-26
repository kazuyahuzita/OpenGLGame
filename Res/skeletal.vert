/*
@file skeletal.vert
*/

#version 450
//�V�F�[�_�[�ւ̓���
layout(location =0)in vec3 inPosition;
layout(location=1) in vec2 inTexcoord; // �e�N�X�`�����W
layout(location=2) in vec3 inNormal;   // �@���x�N�g��
//�ϐ��̌^�̑O��u�������Ă���̂́u�����Ȃ������^�v���Ӗ�����
//�t��i�́u�������萮���^�v
layout(location=3) in uvec4 inJoints;  // ���W�ϊ��s��̔ԍ�
layout(location=4) in vec4 inWeights;  // ���W�ϊ��s��̉e���x

//�V�F�[�_�[����̏o��
//�����Ŋ֌W����荇���Ă���A�֐��Ƃ��̎g�����ł͂ł��Ȃ��i�o���邯�ǎg���Ȃ��j
layout(location=0) out vec3 outPosition; // ���[���h���W
layout(location=1) out vec2 outTexcoord; // �e�N�X�`�����W                                                                                                                                                            
layout(location=2) out vec3 outNormal;   // �@���x�N�g��
layout(location=3) out vec3 outShadowTexcoord; // �V���h�E�e�N�X�`�����W

// �v���O��������̓���
//�e�q�K�w�̕��G����h�����߂ɁA�v�Z�����œK��
//�u�O�̌v�Z���ʂɎ��̃x�N�g����������v������J��Ԃ����ƂŁA
//���W�ϊ��ɋ��߂��邱�Ƃ�������悤�ɂ���
//�s��ɂ��邱�ƂŁA���P�[�V�����ԍ��̐ߖ�A�x�N�g�������s��̕��������I
//�������A�K�v�ȃ������̈�̓x�N�g���z��Ɠ���
layout(location=0) uniform mat4 transformMatrix;
layout(location=1) uniform mat3 normalMatrix;

// �A�X�y�N�g��Ǝ���p�ɂ��g�嗦
layout(location=3) uniform vec2 aspectRatioAndScaleFov;

layout(location=4) uniform vec3 cameraPosition; // �J�����̈ʒu
layout(location=5) uniform vec2 cameraSinCosY;  // �J������Y����]
layout(location=6) uniform vec3 cameraPosition1;  // �J������X����]
layout(location=7) uniform vec2 cameraSinCosX;  // �J������X����]

layout(location=10) uniform mat4 shadowTextureMatrix; // �V���h�E�e�N�X�`���s��
layout(location=11) uniform float shadowNormalOffset; // ���W��@�������ɂ��炷��

// �֐߃f�[�^�p�̃V�F�[�_�X�g���[�W�u���b�N
//std430:�V�F�[�_�[�X�g���[�W�u���b�N�̃f�[�^�z�u���@�̎w��
//�V�F�[�_�X�g���[�W�u���b�N��p�ŁA�f�[�^��C����ɋ߂��`�Ń�������z�u��
//binding:CPU���ŃV�F�[�_�X�g���[�W�u���b�N���w�肷��Ƃ��Ɏg���ԍ���ݒ�
//�V�F�[�_�X�g���[�W�u���b�N�ł́AC����̔z��Ɠ��l�ɔz��̃T�C�Y���ȗ��ł���
//�T�C�Y���ȗ�������ACPU������T�C�Y�w�艻��
layout(std430, binding=0) buffer JointDataBlock
{
  mat4 jointMatrices[]; // �֐߂̍��W�ϊ��s��
};

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
    
    // �֐ߍs��ɉe���x���|���ĉ��Z����
    mat4 jointMatrix =
     jointMatrices[inJoints.x] * inWeights.x +
     jointMatrices[inJoints.y] * inWeights.y +
     jointMatrices[inJoints.z] * inWeights.z +
     jointMatrices[inJoints.w] * inWeights.w;
     
    // �֐ߍs��ƃQ�[���I�u�W�F�N�g�̍��W�ϊ��s�������
    mat4 modelMatrix = transformMatrix * jointMatrix;

    //���[�J�����W���烏�[���h���W�ɕϊ�
    gl_Position =  modelMatrix  * vec4(inPosition, 1);
    outPosition = gl_Position.xyz;

    // ���[���h�@�����v�Z
    mat3 jointNormalMatrix = transpose(inverse(mat3(modelMatrix)));
    outNormal =normalize(jointNormalMatrix * inNormal);

    // �V���h�E�e�N�X�`�����W���v�Z
    outShadowTexcoord = outPosition + outNormal * shadowNormalOffset;
    outShadowTexcoord = vec3(shadowTextureMatrix * vec4(outShadowTexcoord, 1));

    // ���[���h���W�n����r���[���W�n�ɕϊ�
    float cameraSinY = cameraSinCosY.x;
    float cameraCosY = cameraSinCosY.y;
    vec3 pos = gl_Position.xyz - cameraPosition;
    gl_Position.x = pos.x * cameraCosY + pos.z * cameraSinY;
    gl_Position.y = pos.y;
    gl_Position.z = pos.x * -cameraSinY + pos.z * cameraCosY;
    
    vec3 pos1 = gl_Position.xyz - cameraPosition1; 

    float cameraSinX = cameraSinCosX.x;
    float cameraCosX = cameraSinCosX.y;
    gl_Position.x = pos1.x;
    gl_Position.y = pos1.y * cameraCosX  + pos1.z * -cameraSinX;
    gl_Position.z = pos1.y * cameraSinX + pos1.z * cameraCosX;

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