/**
* @file ImageSettings.h
*/
#ifndef IMAGESETTINGS_H_INCLUDED
#define IMAGESETTINGS_H_INCLUDED

//�t�@�C���̖��O���ԈႦ�Ȃ��悤�ɁA���ꂼ��������Ɏ�������
//inline constexpr�́u��`���ЂƂɕۂv�Ƃ����Ӗ�
//�C���N���[�h��ŃR�s�[������āA���s�t�@�C���̃T�C�Y����剻����̂�h��

/**
* �摜�ƃ��S��ʂ̉摜�̐ݒ�
*/
namespace Image_Title
{
	inline constexpr char title_bg[]		= "Res/Image/Title/Title_StartBg.tga";		// �^�C�g���̔w�i�摜
	inline constexpr char title_logo[]		= "Res/Image/Title/Title_StartLogo.tga";	// �^�C�g����ʂ̃��S�摜
	inline constexpr char start_button[]	= "Res/Image/Title/Title_StartButton.tga";	// �X�^�[�g�{�^���̉摜
	inline constexpr char quit_button[]		= "Res/Image/Title/Title_Quitbutton.tga";	// �Q�[���I���{�^���̉摜

} // namespace Image_Title

/*
* �z���[�Q�[���p�̉摜�̐ݒ�
*
*/
namespace Image_Horror
{
	//�^�C�g���p�̉摜
	inline constexpr char title_bg[] = "Res/Image/Horror/Horror_TitleBg.tga";				// �Q�[���N���A�̉摜
	//�Q�[���N���A�p�̉摜
	inline constexpr char clear_bg[] = "Res/Image/Horror/Horror_GameClearbg.tga";				// �Q�[���N���A�̉摜
	inline constexpr char clear_logo[] = "Res/Image/Horror/Horror_GameClearlogo.tga";		// �Q�[���N���A��ʂ̃��S�摜
	//�Q�[���I�[�o�[�p�̉摜
	inline constexpr char over_bg[] = "Res/Image/Horror/Horror_GameOverbg.tga";			// �Q�[���I�[�o�[�̃��S�摜
	inline constexpr char over_logo[] = "Res/Image/Horror/Horror_GameOverlogo.tga";			// �Q�[���I�[�o�[�̃��S�摜
	//�Q�[���v���C���̉摜
	inline constexpr char target[] 
		= "Res/Image/Horror/Horror_TextTarget.tga";			// �Q�[���V�[���̉摜(�ڕW�Ə����Ă���)
	inline constexpr char search[] 
		= "Res/Image/Horror/Horror_TextSeach.tga";			// �Q�[���V�[���̉摜(�����e�������Ă���)
	inline constexpr char search_last[] 
		= "Res/Image/Horror/Horror_TextSearchLast.tga";		// �Q�[���V�[���̉摜(�����e�������Ă���)
	inline constexpr char goal[] 
		= "Res/Image/Horror/Horror_TextGoal.tga";			// �Q�[���V�[���̉摜(�S�[����ڎw���悤�����Ă���)

	//�{�^���p�̉摜
	inline constexpr char back_button[]  = "Res/Image/Horror/Horror_Quitbutton.tga";			// �߂�Ƃ��̉摜
	inline constexpr char retry_button[] = "Res/Image/Athletic/Athletic_RetryButton.tga";			// �߂�Ƃ��̉摜
	inline constexpr char start_button[] = "Res/Image/Horror/Horror_StartButton.tga";			// �߂�Ƃ��̉摜

}//namespace Image_Horror

/*
* �A�X���`�b�N�Q�[���p�̉摜�̐ݒ�
*
*/
namespace Image_Athletic
{
	//�^�C�g���p�̉摜
	inline constexpr char title_bg[]	   = "Res/Image/Athletic/Athletic_Titlebg.tga";				// �^�C�g���̉摜
	inline constexpr char title_logo[] = "Res/Image/Athletic/Athletic_Titlelogo.tga";		// �^�C�g����ʂ̃��S�摜
	//�Q�[���N���A�p�̉摜
	inline constexpr char clear_bg[]   = "Res/Image/Athletic/Athletic_GameClearbg.tga";		// �Q�[���N���A�̉摜
	inline constexpr char clear_logo[] = "Res/Image/Athletic/Athletic_GameClearlogo.tga";	// �Q�[���N���A��ʂ̃��S�摜
	//�Q�[���I�[�o�[�p�̉摜
	inline constexpr char over_logo[]  = "Res/Image/Athletic/Athletic_GameOverlogo.tga";		// �Q�[���I�[�o�[�̃��S�摜
	//�Q�[���v���C���̉摜
	inline constexpr char backscene[]  = "Res/Image/Athletic/BackScene.tga";					// �X�e�[�W�I���V�[���ɖ߂�e�L�X�g�摜
	inline constexpr char nextscene[]  = "Res/Image/Athletic/NextScene.tga";					// ���̃X�e�[�W�ɍs���e�L�X�g�摜
	inline constexpr char clock[]	   = "Res/Image/Athletic/Clock.tga";							// �������ԗp�̎��v�̉摜
	inline constexpr char time_bg[]	   = "Res/Image/Athletic/Time_bg.tga";						// �������Ԃ̌��摜
	inline constexpr char opengate[]   = "Res/Image/Athletic/GateOpen.tga";					// �傪�J�������̃e�L�X�g�摜
	//�{�^���p�̉摜
	inline constexpr char back_button[]  = "Res/Image/Athletic/Athletic_BackButton.tga";			// �߂�Ƃ��̉摜
	inline constexpr char retry_button[] = "Res/Image/Athletic/Athletic_RetryButton.tga";			// �߂�Ƃ��̉摜
	inline constexpr char start_button[] = "Res/Image/Athletic/Athletic_StartButton.tga";			// �߂�Ƃ��̉摜

	//�������Ԃ̎��Ɏg���������̉摜
	inline constexpr char number0[] = "Res/Image/Athletic/0.tga";			
	inline constexpr char number1[] = "Res/Image/Athletic/1.tga";			
	inline constexpr char number2[] = "Res/Image/Athletic/2.tga";			
	inline constexpr char number3[] = "Res/Image/Athletic/3.tga";			
	inline constexpr char number4[] = "Res/Image/Athletic/4.tga";			
	inline constexpr char number5[] = "Res/Image/Athletic/5.tga";			
	inline constexpr char number6[] = "Res/Image/Athletic/6.tga";			
	inline constexpr char number7[] = "Res/Image/Athletic/7.tga";			
	inline constexpr char number8[] = "Res/Image/Athletic/8.tga";			
	inline constexpr char number9[] = "Res/Image/Athletic/9.tga";			

}//namespace Image_Athletic

namespace Image_Fade
{
	//Fade�p�̉摜
	inline constexpr char white[]  = "Res/Image/fade/white.tga"; // �t�F�[�h�p�̔��摜
	inline constexpr char black[]  = "Res/Image/fade/black.tga"; // �t�F�[�h�p�̍��摜
	inline constexpr char nowLoading[]  = "Res/Image/fade/NowLoadig.tga"; // �t�F�[�h�p�̍��摜

}//namespace Image_Fade

namespace Image_Sphere
{
	//Fade�p�̉摜
	inline constexpr char skysphere[]  = "Res/MeshData/sky_sphere/sky.tga"; // �t�F�[�h�p�̔��摜
	inline constexpr char graysphere[]  = "Res/Image/fade/black.tga"; // �t�F�[�h�p�̍��摜

}//namespace Image_Fade

#endif // IMAGESETTINGS_H_INCLUDED