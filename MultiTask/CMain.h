#pragma once
#include "ThreadObj.h"
class CMain :	public CThreadObj
{
public:
	CMain();
	~CMain();

	static int WindowPositionX;					//生成するウィンドウ位置のX座標
	static int WindowPositionY;					//生成するウィンドウ位置のY座標
	static int WindowWidth;						//生成するウィンドウの幅
	static int WindowHeight;						//生成するウィンドウの高さ
	static char WindowTitle[128];// = "世界の始まり"; //ウィンドウのタイト
	 // Window Appliにするためのダミー引数
	static int dummy_argc;
	static char* argv;
	static char** dummy_argv;

	LRESULT CALLBACK PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void set_panel_tip_txt();//タブパネルのStaticテキストを設定

//OPEN GL---------------------------------------------
	static void Initialize(void);
	static void Display(void);

	static void ActOpenGL(void);
//OPEN GL---------------------------------------------

};

