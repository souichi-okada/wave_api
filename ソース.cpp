#include <windows.h>		//Windows環境
#include <windowsx.h>		//Windows環境
#include <stdio.h>			//入出力用
#include <process.h>		//スレッド用
#include <stdlib.h>

#pragma comment(lib,"winmm.lib")//高精度タイマ

#include "resource.h"		//リソースファイル
#include "ヘッダー.h"		//リソースファイル

//構造体
typedef struct {
	HWND	hWnd;
	HWND	hwnd[4];
}SEND_POINTER_STRUCT;


//メインプロシージャ（ダイアログ）
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HANDLE hMutex;

	//多重起動判定
	hMutex = CreateMutex(NULL, TRUE, DEF_MUTEX_NAME);		//ミューテックスオブジェクトの生成
	if (GetLastError() == ERROR_ALREADY_EXISTS) {				//2重起動の有無を確認
		MessageBox(NULL, TEXT("既に起動されています．"), NULL, MB_OK | MB_ICONERROR);
		return 0;											//終了
	}

	//ダイアログ起動
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, MainDlgProc);

	return FALSE;			//終了
}

//メインプロシージャ（ダイアログ）
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont;				//フォント
	static HANDLE hThread;
	static UINT thID;
	static SEND_POINTER_STRUCT Sps;
	//static HWND hEdit;

	static HWND hPict[4];
	//static HWND hWnd[4];

	switch (uMsg) {
	case WM_INITDIALOG:		//ダイアログ初期化
		Sps.hWnd = hDlg;
		//hEdit = GetDlgItem(hDlg, IDC_EDIT1);

		//背景，軸を表示
		hPict[0] = GetDlgItem(hDlg, IDC_PICTBOX1);
		WinInitialize(NULL, hDlg, (HMENU)110, "TEST1", hPict[0], WndProc, &Sps.hwnd[0]);
		//初期化
		 //WinInitialize関数によって子ウィンドウプロシージャにhPict1がhWnd1としてセットされました．
		hPict[1] = GetDlgItem(hDlg, IDC_PICTBOX2);
		WinInitialize(NULL, hDlg, (HMENU)110, "TEST2", hPict[1], WndProc, &Sps.hwnd[1]);
		//初期化
		 //WinInitialize関数によって子ウィンドウプロシージャにhPict2がhWnd2としてセットされました．
		hPict[2] = GetDlgItem(hDlg, IDC_PICTBOX3);
		WinInitialize(NULL, hDlg, (HMENU)110, "TEST3", hPict[2], WndProc, &Sps.hwnd[2]);
		//初期化
		//WinInitialize関数によって子ウィンドウプロシージャにhPict3がhWnd3としてセットされました．
		hPict[3] = GetDlgItem(hDlg, IDC_PICTBOX4);
		WinInitialize(NULL, hDlg, (HMENU)110, "TEST4", hPict[3], WndProc, &Sps.hwnd[3]);
		//初期化
		 //WinInitialize関数によって子ウィンドウプロシージャにhPict4がhWnd4としてセットされました．
		return TRUE;

	case WM_COMMAND:		//ボタンが押された時
		switch (LOWORD(wParam)) {
		case IDOK:			//OKボタン



			//データ読み込みスレッド起動
			hThread = (HANDLE)_beginthreadex(NULL, 0, TFunc, (PVOID)&Sps, 0, &thID);   //_beginthreadex→スレッドを立ち上げる関数	
			EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);						//開始ボタン無効化　　　　//EnableWindowで入力を無効または有効にする。

			return TRUE;

		case IDCANCEL:	//停止・再開ボタン

						/*　サスペンドカウンタ　**************************
						　　　実行を許可するまでスレッドを動かさない。
						   　　ResumeThread：　サスペンドカウンタを1減らす
							 　SuspendThread：　サスペンドカウンタを1増やす

							  0のときは実行。それ以外は待機する。
							  **************************************************/


			if (ResumeThread(hThread) == 0) {					//停止中かを調べる(サスペンドカウントを１減らす)
				SetDlgItemText(hDlg, IDCANCEL, TEXT("再開"));	//再開に変更　　　　　　　　　　　　　　　　　　　//SetDlgItemTextでダイアログ内のテキストなどを変更することができる
				SuspendThread(hThread);						//スレッドの実行を停止(サスペンドカウントを１増やす)
			}
			else
				SetDlgItemText(hDlg, IDCANCEL, TEXT("停止"));	//停止に変更

			return TRUE;
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg, 0);			//ダイアログ終了
		return TRUE;
	}

	return FALSE;
}

//データ読み込み用スレッド
UINT WINAPI TFunc(LPVOID thParam)
{
	static SEND_POINTER_STRUCT* FU = (SEND_POINTER_STRUCT*)thParam;        //構造体のポインタ取得

	FILE* fp;			//ファイルポインタ
	BOOL Flag = TRUE;		//ループフラグ
	HFONT hFont;		//フォント

	double data1, data2, data3, data4,data5;		//データ(ファイルから得る値)



	double width1, width2, hight1, hight2, width3, width4, hight3, hight4;				//PICTBOXの幅
	RECT rect1, rect2, rect3, rect4;
	GetClientRect(FU->hwnd[0], &rect1);	//PICTBOXの情報を入手(大きさ一緒だけど一応)
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

	double y1 = 0, y2 = 0, time1 = 0, time2 = 0,y3 = 0, y4 = 0, time3 = 0, time4 = 0;				//プロットするために変換する値

	double by1 = hight1 / 2, by2 = hight2 / 2, by3 = hight3 / 2, by4 = hight4 / 2;
	double btime1 = width1 * 0.1, btime2 = width2 * 0.1, btime3 = width3 * 0.1, btime4 = width4 * 0.1;			//前回の値


	////エディタのフォント変更(case WM_INITDIALOG:の中で設定しても良い)
	//hFont = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
	//	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, NULL);
	//SendMessage(FU->hedit[0], WM_SETFONT, (int)hFont, TRUE);

	beforeTime = timeGetTime();						//現在の時刻計算（初期時間）

	//ファイルオープン
	if ((fp = fopen("data1.txt", "r")) == NULL) {
		MessageBox(NULL, TEXT("Input File Open ERROR!"), NULL, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	//データ読み込み・表示
	while (Flag == TRUE) {
		DWORD nowTime, progress, idealTime;

		//時間の調整
		nowTime = timeGetTime();					//現在の時刻計算
		progress = nowTime - beforeTime;				//処理時間を計算
		idealTime = (DWORD)(DNum * (1000.0F / (double)DEF_DATAPERS));	//理想時間を計算
		if (idealTime > progress) Sleep(idealTime - progress);			//理想時間になるまで待機

		//データの読み込み
		if (fscanf(fp, "%lf %lf %lf %lf %lf", &data1, &data2,&data3, &data4, &data5) == EOF) {
			MessageBox(NULL, TEXT("終了"), TEXT("INFORMATION"), MB_OK | MB_ICONEXCLAMATION);
			EnableWindow(GetDlgItem(FU->hwnd[0], IDOK), TRUE);		//開始ボタン有効
			Flag = FALSE;												//ループ終了フラグ
			return FALSE;
		}
		
		//表示
		//表示
				//読み込んだ値を変換
				//データをPICTBOXに合わせて変形(*5してるのは適当です…波形見て判断してください)
		y1 = data2 * 5 * hight1 / 2 * 0.8;
		y2 = data3 *5* hight2 / 2 * 0.8;
		y3 = data4 *5* hight3 / 2 * 0.8;
		y4 = data5 *5* hight4 / 2 * 0.8;

		//データの表示(プロット)
		//一回に3秒(3000データ)分表示するとき(幅によって被って描画されるかも)
		PLOT(FU->hwnd[0], width1 * 0.1 + time * width1 / (3 * DEF_DATAPERS), hight1 / 2 - y1, btime1, by1);
		PLOT(FU->hwnd[1], width2 * 0.1 + time * width2 / (3 * DEF_DATAPERS), hight2 / 2 - y2, btime2, by2);
		PLOT(FU->hwnd[2], width3 * 0.1 + time * width3 / (3 * DEF_DATAPERS), hight3 / 2 - y3, btime3, by3);
		PLOT(FU->hwnd[3], width4 * 0.1 + time * width4 / (3 * DEF_DATAPERS), hight4 / 2 - y4, btime4, by4);

		//1msずつ横軸の値を+1する
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

		//1msごとに+1
		/*btime1 = width1*0.1 + time;
		btime2 = width2*0.1 + time;
		btime3 = width3*0.1 + time;
		btime4 = width4*0.1 + time;*/

		time++;
		//読み込んだ値を変換
		//データをPICTBOXに合わせて変形(*5してるのは適当です…波形見て判断してください)
		y1 = data2 *5* hight1 / 2 * 0.8;
		y2 = data3  *5* hight2 / 2 * 0.8;
		y3 = data4  *5* hight3 / 2 * 0.8;
		y4 = data5  *5* hight4 / 2 * 0.8;

		//データの表示(プロット)
		//一回に3秒(3000データ)分表示するとき(幅によって被って描画されるかも)
		PLOT(FU->hwnd[0], width1 * 0.1 + time * width1 / (3 * DEF_DATAPERS), hight1 / 2 - y1, btime1, by1);
		PLOT(FU->hwnd[1], width2 * 0.1 + time * width2 / (3 * DEF_DATAPERS), hight2 / 2 - y2, btime2, by2);
		PLOT(FU->hwnd[2], width3 * 0.1 + time * width3 / (3 * DEF_DATAPERS), hight3 / 2 - y3, btime3, by3);
		PLOT(FU->hwnd[3], width4 * 0.1 + time * width4 / (3 * DEF_DATAPERS), hight4 / 2 - y4, btime4, by4);

		//1msずつ横軸の値を+1する
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

		//1msごとに+1
		/*btime1 = width1*0.1 + time;
		btime2 = width2*0.1 + time;
		btime3 = width3*0.1 + time;
		btime4 = width4*0.1 + time;*/

		time++;
		DNum++;

		//一秒経過時
		if (progress >= 1000.0) {
			beforeTime = nowTime;
			DNum = 0;
		}

		if (time > 0.8 * DEF_DATAPERS * 3) {
			//if(time >0.8*width1){		//1msごとに+1
				//横軸を0に戻し、背景を黒くし軸を描く
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


//子ウィンドウプロシージャ
//軸を描く
HRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	HDC			hdc1;				//デバイスコンテキストのハンド
	PAINTSTRUCT ps;					//(構造体)クライアント領域描画するための情報	
	HBRUSH		hBrush, hOldBrush;	//ブラシ
	HPEN		hPen, hOldPen;		//ペン

	switch (uMsg) {
	case WM_CREATE:
		colorPen = RGB(0, 0, 0);	//色指定
		//colorPen = color;	//色指定
		break;

	case WM_PAINT:
		int width, hight;
		/********************************

		PictureControlに描画するためには，HDC型のハンドルを別に取得する
		必要があります．

		例：hdc = BeginPaint(hWnd, &ps);
		hdc:デバイスコンテキストのハンドル
		hWnd:PictureControlのハンドル
		ps：(構造体)クライアント領域描画するための情報

		********************************/
		RECT rect;
		GetClientRect(hWnd, &rect);
		width = rect.left + rect.right;
		hight = rect.top + rect.bottom;
		hdc1 = BeginPaint(hWnd, &ps);//デバイスコンテキストのハンドル取得

		/********************************

		PictureControlに描画するためには，線を引きたいときはペン，
		塗りつぶす際にはブラシが必要です．

		********************************/

		//ペン，ブラシ生成
		hBrush = CreateSolidBrush(color);				//ブラシ生成
		hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);	//ブラシ設定
		hPen = CreatePen(PS_SOLID, 2, colorPen);		//ペン生成
		hOldPen = (HPEN)SelectObject(hdc1, hPen);		//ペン設定
		//描画
		/********************************

		図形を描画するためには以下の関数を用います．
		長方形：Rectangle(HDC hdc ,int nLeftRect , int nTopRect ,int nRightRect , int nBottomRect);
		円：Ellipse(HDC hdc ,int nLeftRect , int nTopRect ,int nRightRect , int nBottomRect);

		 nLiftRect：長方形の左上X座標
		  nTopRect：左上Y座標
		  nRightRect：右下X座標
		  nBottomRect：右下のY座標

		線を引くには以下の関数を用います．

		線の始点設定：MoveToEx(HDC hdc , int X , int Y , NULL);
		  X,Y：線の始点の座標
		線；LineTo(HDC hdc , int nXEnd , int nYEnd);
		  nXEnd, nYEnd：線の終点の設定


		  以上を参考に図形を描画する関数を以下に記述しましょう
		********************************/
		//ここから


		Rectangle(hdc1, 0, 0, 1000, 1000);

		SelectObject(hdc1, hOldBrush);
		DeleteObject(hBrush);
		SelectObject(hdc1, hOldPen);
		DeleteObject(hPen);



		hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));			//ペン設定
		hOldPen = (HPEN)SelectObject(hdc1, hPen);
		hBrush = CreateSolidBrush(color);				//ブラシ生成	//ブラシ設定
		hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);
		MoveToEx(hdc1, 78, 0, NULL);
		LineTo(hdc1, 78, 180);
		SelectObject(hdc1, hOldPen);
		DeleteObject(hPen);
		SelectObject(hdc1, hOldBrush);
		DeleteObject(hBrush);

		hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));			//ペン設定
		hOldPen = (HPEN)SelectObject(hdc1, hPen);
		hBrush = CreateSolidBrush(color);				//ブラシ生成	//ブラシ設定
		hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);
		MoveToEx(hdc1, 78, 60, NULL);
		LineTo(hdc1, 1000, 60);
		//横軸のタイトル
		SetTextColor(hdc1, RGB(255, 255, 255));
		SetBkColor(hdc1, RGB(0, 0, 0));
		TextOut(hdc1, rect.right * 0.42, rect.bottom * 0.8, TEXT("Time(s)"), 7);
		//TextOut(hdc, 10, 10, TEXT("ch1"), 3);
		SelectObject(hdc1, hOldPen);
		DeleteObject(hPen);
		SelectObject(hdc1, hOldBrush);
		DeleteObject(hBrush);
		/********************************

		使い終わったペンとブラシは破棄する必要があります．

		********************************/


		//デバイスコンテキストのハンドル破棄
		EndPaint(hWnd, &ps);
		break;
	}


	return TRUE;
}

//波形描画
BOOL PLOT(HWND hWnd, int sec, int data, int bsec, int bdata) {
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = GetDC(hWnd);
	//hdc = BeginPaint(hWnd,&ps);
	HBRUSH		hBrush, hOldBrush;	//ブラシ
	HPEN		hPen, hOldPen;		//ペン
	colorPen = RGB(255, 241, 0);
	hBrush = CreateSolidBrush(color);				//ブラシ生成
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);	//ブラシ設定
	hPen = CreatePen(PS_SOLID, 2, colorPen);		//ペン生成
	hOldPen = (HPEN)SelectObject(hdc, hPen);		//ペン設定

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
//背景、軸の再描画
BOOL REPaint(HWND hWnd) {


		HDC			hdc1;				//デバイスコンテキストのハンドル
		PAINTSTRUCT ps;					//(構造体)クライアント領域描画するための情報	
		HBRUSH		hBrush, hOldBrush;	//ブラシ
		HPEN		hPen, hOldPen;		//ペン
		hdc1 = GetDC(hWnd);
			int width, hight;
			color = RGB(0, 0, 0);		//黒
			colorPen = RGB(255, 255, 255);
			hBrush = CreateSolidBrush(color);				//ブラシ生成
			hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);	//ブラシ設定
			hPen = CreatePen(PS_SOLID, 2, colorPen);		//ペン生成
			hOldPen = (HPEN)SelectObject(hdc1, hPen);		//ペン設定

			RECT rect;
			GetClientRect(hWnd, &rect);
			width = rect.left + rect.right;
			hight = rect.top + rect.bottom;

		
			//ペン，ブラシ生成
			hBrush = CreateSolidBrush(color);				//ブラシ生成
			hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);	//ブラシ設定
			hPen = CreatePen(PS_SOLID, 2, colorPen);		//ペン生成
			hOldPen = (HPEN)SelectObject(hdc1, hPen);		//ペン設定


			Rectangle(hdc1, 0, 0, 1000, 1000);

			SelectObject(hdc1, hOldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc1, hOldPen);
			DeleteObject(hPen);


			hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));			//ペン設定
			hOldPen = (HPEN)SelectObject(hdc1, hPen);
			hBrush = CreateSolidBrush(color);				//ブラシ生成	//ブラシ設定
			hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);
			MoveToEx(hdc1, 78, 0, NULL);
			LineTo(hdc1, 78, 180);
			SelectObject(hdc1, hOldPen);
			DeleteObject(hPen);
			SelectObject(hdc1, hOldBrush);
			DeleteObject(hBrush);

			hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));			//ペン設定
			hOldPen = (HPEN)SelectObject(hdc1, hPen);
			hBrush = CreateSolidBrush(color);				//ブラシ生成	//ブラシ設定
			hOldBrush = (HBRUSH)SelectObject(hdc1, hBrush);
			MoveToEx(hdc1, 78, 60, NULL);
			LineTo(hdc1, 1000, 60);
			//横軸のタイトル
			SetTextColor(hdc1, RGB(255, 255, 255));
			SetBkColor(hdc1, RGB(0, 0, 0));
			TextOut(hdc1, rect.right * 0.42, rect.bottom * 0.8, TEXT("Time(s)"), 7);
			//TextOut(hdc, 10, 10, TEXT("ch1"), 3);
			SelectObject(hdc1, hOldPen);
			DeleteObject(hPen);
			SelectObject(hdc1, hOldBrush);
			DeleteObject(hBrush);
			/********************************

			使い終わったペンとブラシは破棄する必要があります．

			********************************/


			//デバイスコンテキストのハンドル破棄
			EndPaint(hWnd, &ps);



		return TRUE;
}




BOOL WinInitialize(HINSTANCE hInst, HWND hPaWnd, HMENU chID, char* cWinName, HWND PaintArea, WNDPROC WndProc, HWND* hWnd)
{
	WNDCLASS wc;			//ウィンドウクラス
	WINDOWPLACEMENT	wplace;	//子ウィンドウ生成領域計算用（画面上のウィンドウの配置情報を格納する構造体）
	RECT WinRect;			//子ウィンドウ生成領域
	ATOM atom;				//アトム

							//ウィンドウクラス初期化
	wc.style = CS_HREDRAW ^ WS_MAXIMIZEBOX | CS_VREDRAW;	//ウインドウスタイル
	wc.lpfnWndProc = WndProc;									//ウインドウのメッセージを処理するコールバック関数へのポインタ
	wc.cbClsExtra = 0;											//
	wc.cbWndExtra = 0;
	wc.hCursor = NULL;										//プログラムのハンドル
	wc.hIcon = NULL;										//アイコンのハンドル
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//ウインドウ背景色
	wc.hInstance = hInst;										//ウインドウプロシージャがあるインスタンスハンドル
	wc.lpszMenuName = NULL;										//メニュー名
	wc.lpszClassName = (LPCTSTR)cWinName;									//ウインドウクラス名

	if (!(atom = RegisterClass(&wc))) {
		MessageBox(hPaWnd, TEXT("ウィンドウクラスの生成に失敗しました．"), NULL, MB_OK | MB_ICONERROR);
		return false;
	}

	GetWindowPlacement(PaintArea, &wplace);	//描画領域ハンドルの情報を取得(ウィンドウの表示状態を取得)
	WinRect = wplace.rcNormalPosition;		//描画領域の設定

											//ウィンドウ生成
	*hWnd = CreateWindow(
		(LPCTSTR)atom,
		(LPCTSTR)cWinName,
		WS_CHILD | WS_VISIBLE,//| WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME |WS_VISIBLE, 
		WinRect.left, WinRect.top,
		WinRect.right - WinRect.left, WinRect.bottom - WinRect.top,
		hPaWnd, chID, hInst, NULL
	);

	if (*hWnd == NULL) {
		MessageBox(hPaWnd, TEXT("ウィンドウの生成に失敗しました．"), NULL, MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

