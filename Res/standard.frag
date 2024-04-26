/*
@file standard.frag
*/
#version 450


//// �V�F�[�_�ւ̓���
layout(location=0) in vec3 inPosition; // ���[���h���W
layout(location=1) in vec2 inTexcoord; // �e�N�X�`�����W
layout(location=2) in vec3 inNormal;   // �@���x�N�g��
layout(location=3) in vec3 inShadowTexcoord; // �V���h�E�e�N�X�`�����W

// �e�N�X�`���T���v��
//sampler2D��
//�e�N�X�`������ǂݎ�����l�����̂܂ܕԂ�
layout(binding=0) uniform sampler2D texColor;
layout(binding=1) uniform sampler2D texEmission;

//sampler2DShadow��
//�e�N�X�`������ǂݎ�����l�ƈ����Ŏ󂯎����Z���W���r���āA���̌��ʂ�Ԃ�
layout(binding=2) uniform sampler2DShadow texShadow; //�e�e�N�X�`���p, �e�p�̐[�x�e�N�X�`��
//sampler2D�ɁuZ���W�Ɣ�r����v�Ƃ���������ǉ������T���v��
// �v���O��������̓���
layout(location=100) uniform vec4 color; // ���̂̐F
layout(location=101) uniform vec4 emission; // ���̂̔����F
layout(location=102) uniform float alphaCutoff; // �t���O�����g��j�����鋫�E�l
// ����
layout(location=107) uniform vec3 ambientLight;

// ���s����
struct DirectionalLight
{
  vec3 color;     // �F�Ɩ��邳
  vec3 direction; // ���̌���
};
layout(location=108) uniform DirectionalLight directionalLight;
 
// �_�����p�����[�^
struct Light
{
	vec4 colorAndFalloffAngle[16]; // �F�Ɩ��邳, �����J�n�p�x
	vec4 positionAndRadius[16]; // ���W�Ɣ��a
	vec4 directionAndConeAngle[16]; // ����, �ő�Ǝˊp�x 
};
layout(location=110) uniform int lightCount; // ���C�g�̌�
layout(location=111) uniform Light light;
// �o�͂���F�f�[�^
out vec4 outColor;

vec3 CalcLight(in vec4 colorAndFalloffAngle,
in vec4 positionAndRadius,
in vec4 directionAndConeAngle,
in vec3 normal)
{
		vec3 direction = positionAndRadius.xyz - inPosition;

		// �����܂ł̋���
		float sqrDistance = dot(direction, direction);
		float dist = sqrt(sqrDistance);

		// �����𐳋K�����Ē�����1�ɂ���
		direction = normalize(direction);

		// �����x���g�̗]�������g���Ė��邳���v�Z
		float theta = max(dot(direction, normal), 0);

		// �����o�[�g���˂ɂ�锽�ˌ��̃G�l���M�[�ʂ���ˌ��Ɠ��������邽�߂Ƀ΂Ŋ���
		float illuminance = theta / 3.14159265;

		//�u�X�|�b�g���C�g�̕����x�N�g���v�Ɓu�X�|�b�g���C�g����t���O�����g�֌������x�N�g���v
		//�̂Ȃ��p�����߁A�u�ő�Ǝˊp�x�v�Ɣ�r����
		//direction�́u�t���O�����g���烉�C�g�֌������x�N�g���v�ł��邽�߁A�������t�ɂ��Ă���

		// �Ǝˊp�x��0���傫����΃X�|�b�g���C�g�Ƃ݂Ȃ�
		const float coneAngle = directionAndConeAngle.w;
			if (coneAngle > 0)
			{
					 // �u���C�g����t���O�����g�֌������x�N�g���v�Ɓu�X�|�b�g���C�g�̃x�N�g���v�̂Ȃ��p���v�Z
					 // �p�x��coneAngle�ȏ�Ȃ�͈͊O
					 // acos�֐��̓R�T�C���̒l�����W�A���ɖ߂��֐�
					 float angle = acos(dot(-direction, directionAndConeAngle.xyz));
					 if (angle >= coneAngle) 
					 {
					   return vec3(0); // �Ǝ˔͈͊O
					 }

				// �ő�Ǝˊp�x�̂Ƃ�0, �����J�n�p�x�̂Ƃ�1�ɂȂ�悤�ɕ��
				const float falloffAngle = colorAndFalloffAngle.w;
				const float a = min((coneAngle - angle) / (coneAngle - falloffAngle), 1);
				illuminance *= a;

			} // if coneAngle

		 // ���C�g�̍ő勗���𐧌�
		 const float radius = positionAndRadius.w;
		 const float smoothFactor = clamp(1 - pow(dist / radius, 4), 0, 1);
		 illuminance *= smoothFactor * smoothFactor;

		 // �t2��̖@���ɂ���Ė��邳������������
		 // 0���Z���N���Ȃ��悤��1�𑫂��Ă���
		 illuminance /= sqrDistance + 1;
		 // �g�U���̖��邳���v�Z
		 return colorAndFalloffAngle.xyz * illuminance;
}

void main()
{
	 vec4 c = texture(texColor, inTexcoord);
	 if (alphaCutoff > 0)
	 {
		// �J�b�g�I�t�l���w�肳��Ă���ꍇ�A�A���t�@�����̒l�����̏ꍇ�͕`����L�����Z��
		if (c.a < alphaCutoff) {
		  discard; // �t���O�����g��j��(�L�����Z��)
		}
		   c.a = 1; // �j������Ȃ�������s�����ɂ���
	 }

	 // �e�N�X�`���̃K���}�␳������
	 const float crtGamma = 2.2; // CRT���j�^�[�̃K���}�l
	 c.rgb = pow(c.rgb, vec3(2.2));
	 
	 outColor = c * color;
	 
	 // ���`��Ԃɂ���Ē�����1�ł͂Ȃ��Ȃ��Ă���̂ŁA���K�����Ē�����1�ɕ�������
	vec3 normal = normalize(inNormal);


     vec3 diffuse = vec3(0); // �g�U���̖��邳�̍��v
	 //���s�����ɂ��Ή��ł���悤�ɂ��邽�߂ɖ@���̐��K�����Ȃ���
	
    for (int i = 0; i < 12; i++)
	{
		if (i >= lightCount) {
			break;
		}

	//for���̓Y�������g���ƁA�ʂ̐����������Ă��܂�
	//���g��������Ă��܂��̂ŁAswitch���ɂ��ׂ������Ȏw����J��Ԃ����Ƃ�
	//�������s�����Ƃ�

	//�}�N����`�͈ꕶ�ɍς܂���l����������̂ŏo���邾���ꕶ��
	//�����A�ʂ̍s�ɂ������Ƃ��́����g��
	//���F���s���Ă��ꕶ��������

#define CALC_LIGHT(i) \
	diffuse += CalcLight( \
		light.colorAndFalloffAngle[i], \
		light.positionAndRadius[i], \
		light.directionAndConeAngle[i], \
		normal);

	switch (i) {
		case 0: CALC_LIGHT(0); break;
		case 1: CALC_LIGHT(1); break;
		case 2: CALC_LIGHT(2); break;
		case 3: CALC_LIGHT(3); break;
		case 4: CALC_LIGHT(4); break;
		case 5: CALC_LIGHT(5); break;
		case 6: CALC_LIGHT(6); break;
		case 7: CALC_LIGHT(7); break;
		case 8: CALC_LIGHT(8); break;
		case 9: CALC_LIGHT(9); break;
		case 10: CALC_LIGHT(10); break;
		case 11: CALC_LIGHT(11); break;
		}
	} // for lightCount
	
	
	// �e���v�Z
	//�t���O�����g���������ɕ��̂����݂���ꍇ��0
	//���݂��Ȃ��ꍇ�͂P���Ԃ����
	float shadow = texture(texShadow, inShadowTexcoord).r;

	// ���s�����̖��邳���v�Z
	//�����̌��������Ă���direction�ϐ���
	//�t���O�����g���猩�����̌���������K�v������̂ŁA�������t��
	float theta = max(dot(-directionalLight.direction, normal), 0);
	float illuminance = theta / 3.14159265;
	diffuse += directionalLight.color * illuminance * shadow;

	// �A���r�G���g���C�g�̖��邳���v�Z
	diffuse += ambientLight;


	// �g�U���̉e���𔽉f
	// ���݂��̓�������𑫂����Ƃł�胊�A���̌��`�ʂ�����Ă���
	outColor.rgb *= diffuse ;
 
	// �����F�𔽉f
	//emission��w�́u�G�~�b�V�����e�N�X�`���̗L���v�𔻒�
	//

	if (emission.w > 0) 
	{
	  outColor.rgb += texture(texEmission, inTexcoord).rgb * emission.rgb;
	 }else{
	  outColor.rgb += emission.rgb;
	}
	// �K���}�␳���s��
	outColor.rgb = pow(outColor.rgb, vec3(1 / 2.2));
}