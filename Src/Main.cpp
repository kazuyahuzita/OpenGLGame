/**
 * @file Main.cpp
 **/
#include "Engine/Engine.h"
#include "TitleScene.h"
#pragma warning(push)
#pragma warning(disable:4005)
#include<Windows.h>
#pragma warning(pop)
/**
/*�G���g���[�|�C���g
*/
//OpenGL���ƌ�����ۂ�unsigned������������
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR IpCmdLine,
	_In_ int nShowCmd
)
{

	Engine engine;
	engine.SetNextScene<TitleScene>();
	return engine.Run();
	
}


