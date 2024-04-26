#ifndef AUDIOSETTINGS_H_INCLUDED
 #define AUDIOSETTINGS_H_INCLUDED

//�t�@�C���̖��O���ԈႦ�Ȃ��悤�ɁA���ꂼ��������Ɏ�������
/**
* �����Đ��v���C���[�ԍ�
*/
namespace AudioPlayer
{
	inline constexpr int bgm = 0; // BGM�̍Đ��Ɏg���v���C���[�ԍ�
	inline constexpr int se =  1; // SE�̍Đ��Ɏg���v���C���[�ԍ�
	inline constexpr int se1 =  2; //SE�̍Đ��Ɏg���v���C���[�ԍ�
	inline constexpr int playerse0 =  3; // Player��SE�̍Đ��Ɏg���v���C���[�ԍ�
	
} // namespace Player

/**
* BGM�ݒ�
*/
namespace BGM_Horror
{
	inline constexpr char title[]		= "Res/Audio/Horror/TitleScene.mp3"; // �^�C�g����ʂ�BGM
	inline constexpr char playScene[]	= "Res/Audio/Horror/PlayScene.mp3"; // �Q�[���V�[����BGM
	inline constexpr char riderScene[]	= "Res/Audio/Horror/RiderScene.wav"; // ���C�_�[�V�[����BGM
	inline constexpr char clear[]		= "Res/Audio/Horror/GameClear.wav"; // �Q�[���N���A��BGM
	inline constexpr char over[]		= "Res/Audio/Horror/GameOver.wav"; // �Q�[���I�[�o�[��BGM
}
namespace BGM_Athletic
{
	inline constexpr char title[] = "Res/Audio/Athletic/Athletic_TitleScene.mp3"; // �^�C�g����ʂ�BGM
	inline constexpr char playScene[]  = "Res/Audio/Athletic/Athletic_PlayScene.mp3"; // �Q�[���V�[����BGM
	inline constexpr char clear[] = "Res/Audio/Athletic/GameClear.wav"; // �Q�[���N���A��BGM
	inline constexpr char over[]  = "Res/Audio/Athletic/GameOver.wav"; // �Q�[���I�[�o�[��BGM

}
namespace BGM_Exploration
{
	inline constexpr char title[]	 = "Res/Audio/Exploration/Exploration_PlayScene.mp3"; // �^�C�g����ʂ�BGM
	inline constexpr char playScene[] = "Res/Audio/Exploration/Exploration_TitleScene.mp3"; // �Q�[���V�[����BGM
}

/**
* ���ʉ��ݒ�
*/
namespace SE
{
	inline constexpr char buttonClick[]		= "Res/Audio/SE/ClickButton.mp3";		// �{�^���N���b�N��
	inline constexpr char missClick[]		= "Res/Audio/SE/missClick.mp3";		// �{�^���̃~�X�N���b�N��
	inline constexpr char goalEvent[]		= "Res/Audio/SE/GameClear.m4a";		// �S�[���ɓ��B����
	inline constexpr char gameOverEvent[]	= "Res/Audio/SE/GameOver.m4a";			// �G�ɂ��܂���
	inline constexpr char GetGem[]			= "Res/Audio/SE/GetGem.mp3";			// �W�F�����Q�b�g����
	inline constexpr char Jump[]			= "Res/Audio/SE/Jump.mp3";				// �W�����v����
	inline constexpr char Walk[]			= "Res/Audio/SE/WalkSound.mp3";		// �����Ă��鎞�̉�
	inline constexpr char Stoke[]			= "Res/Audio/SE/Stoke.wav";			// �G�̒ǐ�BGM

} // namespace SE

#endif // AUDIOSETTINGS_H_INCLUDED