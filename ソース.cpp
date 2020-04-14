#include <windows.h>		//Windows��
#include <windowsx.h>		//Windows��
#include <stdio.h>			//���o�͗p
#include <process.h>		//�X���b�h�p
#include <stdlib.h>

#pragma comment(lib,"winmm.lib")//�����x�^�C�}

#include "resource.h"		//���\�[�X�t�@�C��
#include "�w�b�_�[.h"		//���\�[�X�t�@�C��

//�\����
typedef struct {
	HWND	hWnd;
	HWND	hwnd[4];
}SEND_POINTER_STRUCT;


//���C���v���V�[�W���i�_�C�A���O�j
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HANDLE hMutex;

	//���d�N������
	hMutex = CreateMutex(NULL, TRUE, DEF_MUTEX_NAME);		//�~���[�e�b�N�X�I�u�W�F�N�g�̐���
	if (GetLastError() == ERROR_ALREADY_EXISTS) {				//2�d�N���̗L�����m�F
		MessageBox(NULL, TEXT("���ɋN������Ă��܂��D"), NULL, MB_OK | MB_ICONERROR);
		return 0;											//�I��
	}

	//�_�C�A���O�N��
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, MainDlgProc);

	return FALSE;			//�I��
}

//���C���v���V�[�W���i�_�C�A���O�j
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont;				//�t�H���g
	static HANDLE hThread;
	static UINT thID;
	static SEND_POINTER_STRUCT Sps;
	//static HWND hEdit;

	static HWND hPict[4];
	//static HWND hWnd[4];

	switch (uMsg) {
	case WM_INITDIALOG:		//�_�C�A���O������
		Sps.hWnd = hDlg;
		//hEdit = GetDlgItem(hDlg, IDC_EDIT1);

		//�w�i�C����\��
		hPict[0] = GetDlgItem(hDlg, IDC_PICTBOX1);
		WinInitialize(NULL, hDlg, (HMENU)110, "TEST1", hPict[0], WndProc, &Sps.hwnd[0]);
		//������
		 //WinInitialize�֐��ɂ���Ďq�E�B���h�E�v���V�[�W����hPict1��hWnd1�Ƃ��ăZ�b�g����܂����D
		hPict[1] = GetDlgItem(hDlg, IDC_PICTBOX2);
		WinInitialize(NULL, hDlg, (HMENU)110, "TEST2", hPict[1], WndProc, &Sps.hwnd[1]);
		//������
		 //WinInitialize�֐��ɂ���Ďq�E�B���h�E�v���V�[�W����hPict2��hWnd2�Ƃ��ăZ�b�g����܂����D
		hPict[2] = GetDlgItem(hDlg, IDC_PICTBOX3);
		WinInitialize(NULL, hDlg, (HMENU)110, "TEST3", hPict[2], WndProc, &Sps.hwnd[2]);
		//������
		//WinInitialize�֐��ɂ���Ďq�E�B���h�E�v���V�[�W����hPict3��hWnd3�Ƃ��ăZ�b�g����܂����D
		hPict[3] = GetDlgItem(hDlg, IDC_PICTBOX4);
		WinInitialize(NULL, hDlg, (HMENU)110, "TEST4", hPict[3], WndProc, &Sps.hwnd[3]);
		//������
		 //WinInitialize�֐��ɂ���Ďq�E�B���h�E�v���V�[�W����hPict4��hWnd4�Ƃ��ăZ�b�g����܂����D
		return TRUE;

	case WM_COMMAND:		//�{�^���������ꂽ��
		switch (LOWORD(wParam)) {
		case IDOK:			//OK�{�^��



			//�f�[�^�ǂݍ��݃X���b�h�N��
			hThread = (HANDLE)_beginthreadex(NULL, 0, TFunc, (PVOID)&Sps, 0, &thID);   //_beginthreadex���X���b�h�𗧂��グ��֐�	
			EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);						//�J�n�{�^���������@�@�@�@//EnableWindow�œ��͂𖳌��܂��͗L���ɂ���B

			return TRUE;

		case IDCANCEL:	//��~�E�ĊJ�{�^��

						/*�@�T�X�y���h�J�E���^�@**************************
						�@�@�@���s��������܂ŃX���b�h�𓮂����Ȃ��B
						   �@�@ResumeThread�F�@�T�X�y���h�J�E���^��1���炷
							 �@SuspendThread�F�@�T�X�y���h�J�E���^��1���₷

							  0�̂Ƃ��͎��s�B����ȊO�͑ҋ@����B
							  **************************************************/


			if (ResumeThread(hThread) == 0) {					//��~�����𒲂ׂ�(�T�X�y���h�J�E���g���P���炷)
				SetDlgItemText(hDlg, IDCANCEL, TEXT("�ĊJ"));	//�ĊJ�ɕύX�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@//SetDlgItemText�Ń_�C�A���O���̃e�L�X�g�Ȃǂ�ύX���邱�Ƃ��ł���
				SuspendThread(hThread);						//�X���b�h�̎��s���~(�T�X�y���h�J�E���g���P���₷)
			}
			else
				SetDlgItemText(hDlg, IDCANCEL, TEXT("��~"));	//��~�ɕύX

			return TRUE;
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg, 0);			//�_�C�A���O�I��
		return TRUE;
	}

	return FALSE;
}

//�f�[�^�ǂݍ��ݗp�X���b�h
UINT WINAPI TFunc(LPVOID thParam)
{
	static SEND_POINTER_STRUCT* FU = (SEND_POINTER_STRUCT*)thParam;        //�\���̂̃|�C���^�擾

	FILE* fp;			//�t�@�C���|�C���^
	BOOL Flag = TRUE;		//���[�v�t���O
	HFONT hFont;		//�t�H���g

	double data1, data2, data3, data4,data5;		//�f�[�^(�t�@�C�����瓾��l)



	double width1, width2, hight1, hight2, width3, width4, hight3, hight4;				//PICTBOX�̕�
	RECT rect1, rect2, rect3, rect4;
	GetClientRect(FU->hwnd[0], &rect1);	//PICTBOX�̏������(�傫���ꏏ�����ǈꉞ)
	GetClientRect(FU->hwnd[1], &rect2);
	GetClientRect(FU->hwnd[2], &rect3);
	GetClientRect(FU->hwnd[3], &rect4);

	width1 = rect1.left + rect1.right;
	width2 = rect2.left + rect2.right;
	hight1 = rect1.top + rect1.bottom;
	hight2 = rect2.top + rect2.bottom;
	width3 = rect3.left + rect3.right;
	width4 = rect4.left + rect4.right;
	hight3 = rect3.top + rect3.bottom;
	hight4 = rect4.top + rect4.bottom;

	DWORD DNum = 0, beforeTime;

	double time = 0;

	double y1 = 0, y2 = 0, time1 = 0, time2 = 0,y3 = 0, y4 = 0, time3 = 0, time4 = 0;				//�v���b�g���邽�߂ɕϊ�����l

	double by1 = hight1 / 2, by2 = hight2 / 2, by3 = hight3 / 2, by4 = hight4 / 2;
	double btime1 = width1 * 0.1, btime2 = width2 * 0.1, btime3 = width3 * 0.1, btime4 = width4 * 0.1;			//�O��̒l


	////�G�f�B�^�̃t�H���g�ύX(case WM_INITDIALOG:�̒��Őݒ肵�Ă��ǂ�)
	//hFont = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
	//	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, NULL);
	//SendMessage(FU->hedit[0], WM_SETFONT, (int)hFont, TRUE);

	beforeTime = timeGetTime();						//���݂̎����v�Z�i�������ԁj

	//�t�@�C���I�[�v��
	if ((fp = fopen("data1.txt", "r")) == NULL) {
		MessageBox(NULL, TEXT("Input File Open ERROR!"), NULL, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	//�f�[�^�ǂݍ��݁E�\��
	while (Flag == TRUE) {
		DWORD nowTime, progress, idealTime;

		//���Ԃ̒���
		nowTime = timeGetTime();					//���݂̎����v�Z
		progress = nowTime - beforeTime;				//�������Ԃ��v�Z
		idealTime = (DWORD)(DNum * (1000.0F / (double)DEF_DATAPERS));	//���z���Ԃ��v�Z
		if (idealTime > progress) Sleep(idealTime - progress);			//���z���ԂɂȂ�܂őҋ@

		//�f�[�^�̓ǂݍ���
		if (fscanf(fp, "%lf %lf %lf %lf %lf", &data1, &data2,&data3, &data4, &data5) == EOF) {
			MessageBox(NULL, TEXT("�I��"), TEXT("INFORMATION"), MB_OK | MB_ICONEXCLAMATION);
			EnableWindow(GetDlgItem(FU->hwnd[0], IDOK), TRUE);		//�J�n�{�^���L��
			Flag = FALSE;												//���[�v�I���t���O
			return FALSE;
		}
		
		//�\��
		//�\��
				//�ǂݍ��񂾒l��ϊ�
				//�f�[�^��PICTBOX�ɍ��킹�ĕό`(*5���Ă�͓̂K���ł��c�g�`���Ĕ��f���Ă�������)
		y1 = data2 * 5 * hight1 / 2 * 0.8;
		y2 = data3 *5* hight2 / 2 * 0.8;
		y3 = data4 *5* hight3 / 2 * 0.8;
		y4 = data5 *5* hight4 / 2 * 0.8;

		//�f�[�^�̕\��(�v���b�g)
		//����3�b(3000�f�[�^)���\������Ƃ�(���ɂ���Ĕ���ĕ`�悳��邩��)
		PLOT(FU->hwnd[0], width1 * 0.1 + time * width1 / (3 * DEF_DATAPERS), hight1 / 2 - y1, btime1, by1);
		PLOT(FU->hwnd[1], width2 * 0.1 + time * width2 / (3 * DEF_DATAPERS), hight2 / 2 - y2, btime2, by2);
		PLOT(FU->hwnd[2], width3 * 0.1 + time * width3 / (3 * DEF_DATAPERS), hight3 / 2 - y3, btime3, by3);
		PLOT(FU->hwnd[3], width4 * 0.1 + time * width4 / (3 * DEF_DATAPERS), hight4 / 2 - y4, btime4, by4);

		//1ms�������̒l��+1����
		/*PLOT(FU->hwnd[0], width1*0.1 + time, hight1 / 2 - y1, btime1, by1);
		PLOT(FU->hwnd[1], width1*0.1 + time, hight2 / 2 - y2, btime2, by2);
		PLOT(FU->hwnd[2], width1*0.1 + time, hight3 / 2 - y3, btime3, by3);
		PLOT(FU->hwnd[3], width1*0.1 + time, hight4 / 2 - y4, btime4, by4);*/


		by1 = hight1 / 2 - y1;
		by2 = hight2 / 2 - y2;
		by3 = hight3 / 2 - y3;
		by4 = hight4 / 2 - y4;

		btime1 = width1 * 0.1 + time * width1 / (3 * DEF_DATAPERS);
		btime2 = width2 * 0.1 + time * width2 / (3 * DEF_DATAPERS);
		btime3 = width3 * 0.1 + time * width3 / (3 * DEF_DATAPERS);
		btime4 = width4 * 0.1 + time * width4 / (3 * DEF_DATAPERS);

		//1ms���Ƃ�+1
		/*btime1 = width1*0.1 + time;
		btime2 = width2*0.1 + time;
		btime3 = width3*0.1 + time;
		btime4 = width4*0.1 + time;*/

		time++;
		//�ǂݍ��񂾒l��ϊ�
		//�f�[�^��PICTBOX�ɍ��킹�ĕό`(*5���Ă�͓̂K���ł��c�g�`���Ĕ��f���Ă�������)
		y1 = data2 *5* hight1 / 2 * 0.8;
		y2 = data3  *5* hight2 / 2 * 0.8;
		y3 = data4  *5* hight3 / 2 * 0.8;
		y4 = data5  *5* hight4 / 2 * 0.8;

		//�f�[�^�̕\��(�v���b�g)
		//����3�b(3000�f�[�^)���\������Ƃ�(���ɂ���Ĕ���ĕ`�悳��邩��)
		PLOT(FU->hwnd[0], width1 * 0.1 + time * width1 / (3 * DEF_DATAPERS), hight1 / 2 - y1, btime1, by1);
		PLOT(FU->hwnd[1], width2 * 0.1 + time * width2 / (3 * DEF_DATAPERS), hight2 / 2 - y2, btime2, by2);
		PLOT(FU->hwnd[2], width3 * 0.1 + time * width3 / (3 * DEF_DATAPERS), hight3 / 2 - y3, btime3, by3);
		PLOT(FU->hwnd[3], width4 * 0.1 + time * width4 / (3 * DEF_DATAPERS), hight4 / 2 - y4, btime4, by4);

		//1ms�������̒l��+1����
		/*PLOT(FU->hwnd[0], width1*0.1 + time, hight1 / 2 - y1, btime1, by1);
		PLOT(FU->hwnd[1], width1*0.1 + time, hight2 / 2 - y2, btime2, by2);
		PLOT(FU->hwnd[2], width1*0.1 + time, hight3 / 2 - y3, btime3, by3);
		PLOT(FU->hwnd[3], width1*0.1 + time, hight4 / 2 - y4, btime4, by4);*/


		by1 = hight1 / 2 - y1;
		by2 = hight2 / 2 - y2;
		by3 = hight3 / 2 - y3;
		by4 = hight4 / 2 - y4;

		btime1 = width1 * 0.1 + time * width1 / (3 * DEF_DATAPERS);
		btime2 = width2 * 0.1 + time * width2 / (3 * DEF_DATAPERS);
		btime3 = width3 * 0.1 + time * width3 / (3 * DEF_DATAPERS);
		btime4 = width4 * 0.1 + time * width4 / (3 * DEF_DATAPERS);

		//1ms���Ƃ�+1
		/*btime1 = width1*0.1 + time;
		btime2 = width2*0.1 + time;
		btime3 = width3*0.1 + time;
		btime4 = width4*0.1 + time;*/

		time++;
		DNum++;

		//��b�o�ߎ�
		if (progress >= 1000.0) {
			beforeTime = nowTime;
			DNum = 0;
		}

		if (time > 0.8 * DEF_DATAPERS * 3) {
			//if(time >0.8*width1){		//1ms���Ƃ�+1
				//������0�ɖ߂��A�w�i������������`��
			time = 0;
			btime1 = width1 * 0.1;
			btime2 = width2 * 0.1;
			btime3 = width3 * 0.1;
			btime4 = width4 * 0.1;
			REPaint(FU->hwnd[0]);
			REPaint(FU->hwnd[1]);
			REPaint(FU->hwnd[2]);
			REPaint(FU->hwnd[3]);
		}


	}

	return 0;
}


//�q�E�B���h�E�v���V�[�W��
//����`��
HRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	HDC			hdc1;				//�f�o�C�X�R���e�L�X�g�̃n���h
	PAINTSTRUCT ps;					//(�\����)�N���C�A���g�̈�`�悷�邽�߂̏��	
	HBRUSH		hBrush, hOldBrush;	//�u���V
	HPEN		hPen, hOldPen;		//�y��

	switch (uMsg) {
	case WM_CREATE:
		colorPen = RGB(0, 0, 0);	//�F�w��
		//colorPen = color;	//�F�w��
		break;

	case WM_PAINT:
		int width, hight;
		/********************************

		PictureControl�ɕ`�悷�邽�߂ɂ́CHDC�^�̃n���h����ʂɎ擾����
		�K�v������܂��D

		��Fhdc = BeginPaint(hWnd, &ps);
		hdc:�f�o�C�X�R���e�L�X�g�̃n���h��
		hWnd:PictureControl�̃n���h��
		ps�F(�\����)�N���C�A���g�̈�`�悷�邽�߂̏��

		********************************/
		RECT rect;
		GetClientRect(hWnd, &rect);
		width = rect.left + rect.right;
		hight = rect.top + rect.bottom;
		hdc1 = BeginPaint(hWnd, &ps);//�f�o�C�X�R���e�L�X�g�̃n���h���擾

		/********************************

		PictureControl�ɕ`�悷�邽�߂ɂ́C�������������Ƃ��̓y���C
		�h��Ԃ��ۂɂ̓u���V���K�v�ł��D

		********************************/

		//�y���C�u���V����
		hBrush = CreateSolidBrush(color);				//�u���V����
		hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);	//�u���V�ݒ�
		hPen = CreatePen(PS_SOLID, 2, colorPen);		//�y������
		hOldPen = (HPEN)SelectObject(hdc1, hPen);		//�y���ݒ�
		//�`��
		/********************************

		�}�`��`�悷�邽�߂ɂ͈ȉ��̊֐���p���܂��D
		�����`�FRectangle(HDC hdc ,int nLeftRect , int nTopRect ,int nRightRect , int nBottomRect);
		�~�FEllipse(HDC hdc ,int nLeftRect , int nTopRect ,int nRightRect , int nBottomRect);

		 nLiftRect�F�����`�̍���X���W
		  nTopRect�F����Y���W
		  nRightRect�F�E��X���W
		  nBottomRect�F�E����Y���W

		���������ɂ͈ȉ��̊֐���p���܂��D

		���̎n�_�ݒ�FMoveToEx(HDC hdc , int X , int Y , NULL);
		  X,Y�F���̎n�_�̍��W
		���GLineTo(HDC hdc , int nXEnd , int nYEnd);
		  nXEnd, nYEnd�F���̏I�_�̐ݒ�


		  �ȏ���Q�l�ɐ}�`��`�悷��֐����ȉ��ɋL�q���܂��傤
		********************************/
		//��������


		Rectangle(hdc1, 0, 0, 1000, 1000);

		SelectObject(hdc1, hOldBrush);
		DeleteObject(hBrush);
		SelectObject(hdc1, hOldPen);
		DeleteObject(hPen);



		hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));			//�y���ݒ�
		hOldPen = (HPEN)SelectObject(hdc1, hPen);
		hBrush = CreateSolidBrush(color);				//�u���V����	//�u���V�ݒ�
		hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);
		MoveToEx(hdc1, 78, 0, NULL);
		LineTo(hdc1, 78, 180);
		SelectObject(hdc1, hOldPen);
		DeleteObject(hPen);
		SelectObject(hdc1, hOldBrush);
		DeleteObject(hBrush);

		hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));			//�y���ݒ�
		hOldPen = (HPEN)SelectObject(hdc1, hPen);
		hBrush = CreateSolidBrush(color);				//�u���V����	//�u���V�ݒ�
		hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);
		MoveToEx(hdc1, 78, 60, NULL);
		LineTo(hdc1, 1000, 60);
		//�����̃^�C�g��
		SetTextColor(hdc1, RGB(255, 255, 255));
		SetBkColor(hdc1, RGB(0, 0, 0));
		TextOut(hdc1, rect.right * 0.42, rect.bottom * 0.8, TEXT("Time(s)"), 7);
		//TextOut(hdc, 10, 10, TEXT("ch1"), 3);
		SelectObject(hdc1, hOldPen);
		DeleteObject(hPen);
		SelectObject(hdc1, hOldBrush);
		DeleteObject(hBrush);
		/********************************

		�g���I������y���ƃu���V�͔j������K�v������܂��D

		********************************/


		//�f�o�C�X�R���e�L�X�g�̃n���h���j��
		EndPaint(hWnd, &ps);
		break;
	}


	return TRUE;
}

//�g�`�`��
BOOL PLOT(HWND hWnd, int sec, int data, int bsec, int bdata) {
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = GetDC(hWnd);
	//hdc = BeginPaint(hWnd,&ps);
	HBRUSH		hBrush, hOldBrush;	//�u���V
	HPEN		hPen, hOldPen;		//�y��
	colorPen = RGB(255, 241, 0);
	hBrush = CreateSolidBrush(color);				//�u���V����
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);	//�u���V�ݒ�
	hPen = CreatePen(PS_SOLID, 2, colorPen);		//�y������
	hOldPen = (HPEN)SelectObject(hdc, hPen);		//�y���ݒ�

	LPPOINT IpPoint = (0, 0);
	MoveToEx(hdc, bsec, bdata, IpPoint);
	LineTo(hdc, sec, data);


	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	ReleaseDC(hWnd, hdc);

	return TRUE;
}
//�w�i�A���̍ĕ`��
BOOL REPaint(HWND hWnd) {


		HDC			hdc1;				//�f�o�C�X�R���e�L�X�g�̃n���h��
		PAINTSTRUCT ps;					//(�\����)�N���C�A���g�̈�`�悷�邽�߂̏��	
		HBRUSH		hBrush, hOldBrush;	//�u���V
		HPEN		hPen, hOldPen;		//�y��
		hdc1 = GetDC(hWnd);
			int width, hight;
			color = RGB(0, 0, 0);		//��
			colorPen = RGB(255, 255, 255);
			hBrush = CreateSolidBrush(color);				//�u���V����
			hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);	//�u���V�ݒ�
			hPen = CreatePen(PS_SOLID, 2, colorPen);		//�y������
			hOldPen = (HPEN)SelectObject(hdc1, hPen);		//�y���ݒ�

			RECT rect;
			GetClientRect(hWnd, &rect);
			width = rect.left + rect.right;
			hight = rect.top + rect.bottom;

		
			//�y���C�u���V����
			hBrush = CreateSolidBrush(color);				//�u���V����
			hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);	//�u���V�ݒ�
			hPen = CreatePen(PS_SOLID, 2, colorPen);		//�y������
			hOldPen = (HPEN)SelectObject(hdc1, hPen);		//�y���ݒ�


			Rectangle(hdc1, 0, 0, 1000, 1000);

			SelectObject(hdc1, hOldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc1, hOldPen);
			DeleteObject(hPen);


			hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));			//�y���ݒ�
			hOldPen = (HPEN)SelectObject(hdc1, hPen);
			hBrush = CreateSolidBrush(color);				//�u���V����	//�u���V�ݒ�
			hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);
			MoveToEx(hdc1, 78, 0, NULL);
			LineTo(hdc1, 78, 180);
			SelectObject(hdc1, hOldPen);
			DeleteObject(hPen);
			SelectObject(hdc1, hOldBrush);
			DeleteObject(hBrush);

			hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));			//�y���ݒ�
			hOldPen = (HPEN)SelectObject(hdc1, hPen);
			hBrush = CreateSolidBrush(color);				//�u���V����	//�u���V�ݒ�
			hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);
			MoveToEx(hdc1, 78, 60, NULL);
			LineTo(hdc1, 1000, 60);
			//�����̃^�C�g��
			SetTextColor(hdc1, RGB(255, 255, 255));
			SetBkColor(hdc1, RGB(0, 0, 0));
			TextOut(hdc1, rect.right * 0.42, rect.bottom * 0.8, TEXT("Time(s)"), 7);
			//TextOut(hdc, 10, 10, TEXT("ch1"), 3);
			SelectObject(hdc1, hOldPen);
			DeleteObject(hPen);
			SelectObject(hdc1, hOldBrush);
			DeleteObject(hBrush);
			/********************************

			�g���I������y���ƃu���V�͔j������K�v������܂��D

			********************************/


			//�f�o�C�X�R���e�L�X�g�̃n���h���j��
			EndPaint(hWnd, &ps);



		return TRUE;
}




BOOL WinInitialize(HINSTANCE hInst, HWND hPaWnd, HMENU chID, char* cWinName, HWND PaintArea, WNDPROC WndProc, HWND* hWnd)
{
	WNDCLASS wc;			//�E�B���h�E�N���X
	WINDOWPLACEMENT	wplace;	//�q�E�B���h�E�����̈�v�Z�p�i��ʏ�̃E�B���h�E�̔z�u�����i�[����\���́j
	RECT WinRect;			//�q�E�B���h�E�����̈�
	ATOM atom;				//�A�g��

							//�E�B���h�E�N���X������
	wc.style = CS_HREDRAW ^ WS_MAXIMIZEBOX | CS_VREDRAW;	//�E�C���h�E�X�^�C��
	wc.lpfnWndProc = WndProc;									//�E�C���h�E�̃��b�Z�[�W����������R�[���o�b�N�֐��ւ̃|�C���^
	wc.cbClsExtra = 0;											//
	wc.cbWndExtra = 0;
	wc.hCursor = NULL;										//�v���O�����̃n���h��
	wc.hIcon = NULL;										//�A�C�R���̃n���h��
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//�E�C���h�E�w�i�F
	wc.hInstance = hInst;										//�E�C���h�E�v���V�[�W��������C���X�^���X�n���h��
	wc.lpszMenuName = NULL;										//���j���[��
	wc.lpszClassName = (LPCTSTR)cWinName;									//�E�C���h�E�N���X��

	if (!(atom = RegisterClass(&wc))) {
		MessageBox(hPaWnd, TEXT("�E�B���h�E�N���X�̐����Ɏ��s���܂����D"), NULL, MB_OK | MB_ICONERROR);
		return false;
	}

	GetWindowPlacement(PaintArea, &wplace);	//�`��̈�n���h���̏����擾(�E�B���h�E�̕\����Ԃ��擾)
	WinRect = wplace.rcNormalPosition;		//�`��̈�̐ݒ�

											//�E�B���h�E����
	*hWnd = CreateWindow(
		(LPCTSTR)atom,
		(LPCTSTR)cWinName,
		WS_CHILD | WS_VISIBLE,//| WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME |WS_VISIBLE, 
		WinRect.left, WinRect.top,
		WinRect.right - WinRect.left, WinRect.bottom - WinRect.top,
		hPaWnd, chID, hInst, NULL
	);

	if (*hWnd == NULL) {
		MessageBox(hPaWnd, TEXT("�E�B���h�E�̐����Ɏ��s���܂����D"), NULL, MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

