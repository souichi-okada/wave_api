
//関数宣言
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);	//メインダイアログプロシージャ
BOOL WinInitialize(HINSTANCE hInst, HWND hPaWnd, HMENU chID, char* cWinName, HWND PaintArea, WNDPROC WndProc, HWND* hDC);//子ウィンドウを生成
HRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		//子ウィンドウプロシージャ
UINT WINAPI TFunc(LPVOID thParam);												//データ読み込み用スレッド
BOOL REPaint(HWND hWnd);
BOOL PLOT(HWND hWnd, int sec, int data, int bsec, int bdata);
static HWND hRadioR;		//ウィンドウハンドル（ラジオボタン）
static HWND hRadioG;		//ウィンドウハンドル（ラジオボタン）
static HWND hRadioB;		//ウィンドウハンドル（ラジオボタン）	

static COLORREF	color, colorPen;

#define DEF_APP_NAME	TEXT("Waveform Test")
#define DEF_MUTEX_NAME	DEF_APP_NAME			//ミューテックス名
#define DEF_DATAPERS 1000	//1秒間に何データ入出力す#pragma once
