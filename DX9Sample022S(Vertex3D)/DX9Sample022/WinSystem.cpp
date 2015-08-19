// -------------------- IncludeFile -----------------
#include <crtdbg.h>
#include "Application.h"
// ------------------------------------------------------------------------
//�@�֐���	:WinMain				�A�v���P�[�V�����G���g���[�|�C���g	
//	����	:hInstance				�C���X�^���X�n���h��(Windows���Ŏ��s����Ă���v���O��������ӂɎ��ʂ���)	
//			:hPrevInstance			Windows95�ȍ~��NULL
//			:pCmdLine				�v���O�����ɓn���ꂽ�R�}���h���C���������i�[���ꂽ������̃|�C���^
//			:CmdShow				�E�B���h�E�Y�̕`����@
//	�߂�l	:						Windows�ւ̏I���R�[�h		
// ------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR pCmdLine,int CmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CApplication* pApp = CApplication::getInstance();

	// �A�v���P�[�V�����̏�����
	LPCSTR WindowTitle = "DX9Sample022";		// �A�v���P�[�V������
	SIZE WindowSize = {800,600};				// �X�N���[���T�C�Y
	BOOL bWindowed = TRUE;						// ���[�h�I��(TRUE:�E�B���h�E�AFALSE:�t���X�N���[��)
	HWND hWnd = pApp->Initialize(hInstance,WindowTitle,WindowSize,bWindowed);
	if(hWnd == NULL)
		return 0;

	// �A�v���P�[�V�������s
	return pApp->Run(hWnd);
}