
//�֐��錾
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);	//���C���_�C�A���O�v���V�[�W��
BOOL WinInitialize(HINSTANCE hInst, HWND hPaWnd, HMENU chID, char* cWinName, HWND PaintArea, WNDPROC WndProc, HWND* hDC);//�q�E�B���h�E�𐶐�
HRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		//�q�E�B���h�E�v���V�[�W��
UINT WINAPI TFunc(LPVOID thParam);												//�f�[�^�ǂݍ��ݗp�X���b�h
BOOL REPaint(HWND hWnd);
BOOL PLOT(HWND hWnd, int sec, int data, int bsec, int bdata);
static HWND hRadioR;		//�E�B���h�E�n���h���i���W�I�{�^���j
static HWND hRadioG;		//�E�B���h�E�n���h���i���W�I�{�^���j
static HWND hRadioB;		//�E�B���h�E�n���h���i���W�I�{�^���j	

static COLORREF	color, colorPen;

#define DEF_APP_NAME	TEXT("Waveform Test")
#define DEF_MUTEX_NAME	DEF_APP_NAME			//�~���[�e�b�N�X��
#define DEF_DATAPERS 1000	//1�b�Ԃɉ��f�[�^���o�͂�#pragma once
