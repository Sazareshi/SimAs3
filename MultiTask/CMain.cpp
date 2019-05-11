#include "stdafx.h"
#include "CMain.h"


CMain::CMain(){
	WindowPositionX = 700;
	WindowPositionY = 100;
	WindowWidth = 512;
	WindowHeight = 512;
	//dummy_argc = 1;
	//argv = WindowTitle;
	//dummy_argv = &argv;


}
CMain::~CMain(){
}

int CMain::WindowPositionX;					//生成するウィンドウ位置のX座標
int CMain::WindowPositionY;					//生成するウィンドウ位置のY座標
int CMain::WindowWidth;						//生成するウィンドウの幅
int CMain::WindowHeight;						//生成するウィンドウの高さ

char CMain::WindowTitle[128] = "世界の始まり"; //ウィンドウのタイト
GLfloat CMain::floor_planar[4];
GLfloat CMain::floor_s = 50.0f;
GLfloat CMain::pM[16];
GLfloat CMain::lightpos[4] = { -30, -100, 50, 1 };

QUADS_VERTEX CMain::floor_v = { { floor_s,  floor_s, -1.0f },{ -floor_s,  floor_s, -1.0f },{ -floor_s, -floor_s, -1.0f },{ floor_s, -floor_s, -1.0f }, };

bool CMain::_Bitmap = false;

double CMain::PI = acos(-1.0);
int CMain::tn = 0;
double CMain::t = 0;
double CMain::dt = 0.005;
double CMain::omega = 2.0 * PI / 10.0;
gl_screenshot CMain::gs; //bmpファイルの出力
st_p CMain::p[100];
int CMain::pn = 0;
double CMain::ax = 0.0, CMain::ay = 0.0, CMain::az = -2.0;
double CMain::vx = 5.0, CMain::vy = 5.0, CMain::vz = 20.0;
double CMain::hanpatu = 0.9;

//直方体の定義
GLdouble CMain::vertex[][3] = {{ 0.0, 0.0, 0.0 },{ 2.0, 0.0, 0.0 },{ 2.0, 2.0, 0.0 },{ 0.0, 2.0, 0.0 },{ 0.0, 0.0, 30.0 },{ 2.0, 0.0, 30.0 },{ 2.0, 2.0, 30.0 },{ 0.0, 2.0, 30.0 }};
//面の定義
int CMain::face[][4] = {{ 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 5, 4, 7, 6 },{ 4, 0, 3, 7 },{ 4, 5, 1, 0 },{ 3, 2, 6, 7 }};
//面の法線ベクトル
GLdouble CMain::normal[][3] = {{ 0.0, 0.0,-1.0 },{ 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0 },{ -1.0, 0.0, 0.0 },{ 0.0,-1.0, 0.0 },{ 0.0, 1.0, 0.0 }};

int CMain::cx, CMain::cy;                // ドラッグ開始位置
double CMain::sx, CMain::sy;              // マウスの絶対位置→ウィンドウ内での相対位置の換算係数
double CMain::cq[4] = { 1.0, 0.0, 0.0, 0.0 };  // 回転の初期値 (クォータニオン)
double CMain::tq[4];              // ドラッグ中の回転 (クォータニオン)
double CMain::rt[16];              // 回転の変換行列

unsigned int CMain::listNumber;
float CMain::camera_z_pos = 50.0;


//----------------------------------------------------
// 色の定義
//----------------------------------------------------
//jade(翡翠)
MaterialStruct CMain::ms_jade = {{ 0.135,     0.2225,   0.1575,   1.0 },{ 0.54,      0.89,     0.63,     1.0 },{ 0.316228,  0.316228, 0.316228, 1.0 },12.8 };
//ruby(ルビー)
MaterialStruct CMain::ms_ruby = {{ 0.1745,   0.01175,  0.01175,   1.0 },{ 0.61424,  0.04136,  0.04136,   1.0 },{ 0.727811, 0.626959, 0.626959,  1.0 },76.8 };

GLfloat CMain::red[] = { 0.8, 0.2, 0.2, 1.0 }; //赤色
GLfloat CMain::green[] = { 0.2, 0.8, 0.2, 1.0 };//緑色
GLfloat CMain::blue[] = { 0.2, 0.2, 0.8, 1.0 };//青色
GLfloat CMain::yellow[] = { 0.8, 0.8, 0.2, 1.0 };//黄色
GLfloat CMain::white[] = { 1.0, 1.0, 1.0, 1.0 };//白色
GLfloat CMain::shininess = 30.0;//光沢の強さ
//-----------------------------------------

double CMain::ViewPointX = 0.0;
double CMain::ViewPointY = -50.0;
double CMain::ViewPointZ = 20.0;

int CMain::list;


LRESULT CALLBACK  CMain::PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
	int n;

	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDC_TASK_FUNC_RADIO1: {
			ActOpenGL();
		}break;
		case IDC_TASK_FUNC_RADIO2:
		case IDC_TASK_FUNC_RADIO3:
		case IDC_TASK_FUNC_RADIO4:
		case IDC_TASK_FUNC_RADIO5:
		case IDC_TASK_FUNC_RADIO6:
			inf.panel_func_id = LOWORD(wp); set_panel_tip_txt(); set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0); break;

		case IDC_TASK_ITEM_RADIO1:
		case IDC_TASK_ITEM_RADIO2:
		case IDC_TASK_ITEM_RADIO3:
		case IDC_TASK_ITEM_RADIO4:
		case IDC_TASK_ITEM_RADIO5:
		case IDC_TASK_ITEM_RADIO6:
			inf.panel_type_id = LOWORD(wp); set_panel_tip_txt();  SetFocus(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT1));
			break;
		case IDSET: {
			wstring wstr, wstr_tmp;
			if ((inf.panel_func_id == IDC_TASK_FUNC_RADIO1) && (inf.panel_type_id == IDC_TASK_ITEM_RADIO1)) {
				GetDlgItemText(hDlg, IDC_TASK_EDIT1, (LPTSTR)wstr_tmp.c_str(), 128);
				n = stoi(wstr_tmp);
			}
			inf.panel_func_id = LOWORD(wp); set_panel_tip_txt(); set_PNLparam_value(1.0, 0.0, 0.0, 0.0, 0.0, 0.0); break;
			/*
			//サンプルとしていろいろな型で読み込んで表示している
			wstr += L"Param 1(d):";
			int n = GetDlgItemText(hDlg, IDC_TASK_EDIT1, (LPTSTR)wstr_tmp.c_str(), 128);
			if (n) wstr_tmp = to_wstring(stod(wstr_tmp));	wstr = wstr + wstr_tmp.c_str();

			wstr += L",  Param 2(i):";
			n = GetDlgItemText(hDlg, IDC_TASK_EDIT2, (LPTSTR)wstr_tmp.c_str(), 128);
			if (n) wstr_tmp = to_wstring(stoi(wstr_tmp));	wstr = wstr + wstr_tmp.c_str();

			wstr += L",  Param 3(f):";
			n = GetDlgItemText(hDlg, IDC_TASK_EDIT3, (LPTSTR)wstr_tmp.c_str(), 128);
			if (n) wstr_tmp = to_wstring(stof(wstr_tmp));	wstr = wstr + wstr_tmp.c_str();

			wstr += L",  Param 4(l):";
			n = GetDlgItemText(hDlg, IDC_TASK_EDIT4, (LPTSTR)wstr_tmp.c_str(), 128);
			if (n) wstr_tmp = to_wstring(stol(wstr_tmp));	wstr = wstr + wstr_tmp.c_str();

			wstr += L",  Param 5(c):";
			n = GetDlgItemText(hDlg, IDC_TASK_EDIT5, (LPTSTR)wstr_tmp.c_str(), 128);
			wstr += wstr_tmp.c_str();

			wstr += L",   Param 6(c):";
			n = GetDlgItemText(hDlg, IDC_TASK_EDIT6, (LPTSTR)wstr_tmp.c_str(), 128);
			wstr += wstr_tmp.c_str();

			txout2msg_listbox(wstr);
			*/

		}break;
		case IDRESET: {
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

		}break;

		case IDC_TASK_OPTION_CHECK1:
			SendMessage(GetDlgItem(hDlg, IDC_TASK_OPTION_CHECK2), BM_SETCHECK, BST_UNCHECKED, 0L);
			if (IsDlgButtonChecked(hDlg, IDC_TASK_OPTION_CHECK1) == BST_CHECKED) inf.work_select = THREAD_WORK_OPTION1;
			else inf.work_select = THREAD_WORK_ROUTINE;
			break;

		case IDC_TASK_OPTION_CHECK2:
			SendMessage(GetDlgItem(hDlg, IDC_TASK_OPTION_CHECK1), BM_SETCHECK, BST_UNCHECKED, 0L);
			if (IsDlgButtonChecked(hDlg, IDC_TASK_OPTION_CHECK2) == BST_CHECKED) inf.work_select = THREAD_WORK_OPTION2;
			else inf.work_select = THREAD_WORK_ROUTINE;
			break;
		}
	}
	return 0;
};

void CMain::set_panel_tip_txt()
{
	wstring wstr_type; wstring wstr;
	switch (inf.panel_func_id) {
	case IDC_TASK_FUNC_RADIO1: {
		wstr = L"Type for Func1 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO2: {
		wstr = L"Type for Func2 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO3: {
		wstr = L"Type for Func3 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO4: {
		wstr = L"Type for Func4 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO5: {
		wstr = L"Type for Func5 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO6: {
		wstr = L"Type for Func6 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			break;
		default:break;
		}
	}break;
	default: {
		wstr = L"Type for Func? \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		wstr_type += L"(Param of type?) \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
	}break;
	}

	SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_STATIC_TASKSET3), wstr.c_str());
	SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_STATIC_TASKSET4), wstr_type.c_str());
}


//#################### OPEN GL ################################################
void CMain::ActOpenGL(void) {

	srand((unsigned)time(NULL));

	if (_Bitmap) _mkdir("bitmap"); //bmpファイル保存用のフォルダの作成

//	glutInit(&dummy_argc, dummy_argv);//環境の初期化
	glutInit(&__argc, __argv);//環境の初期化
	glutInitWindowPosition(WindowPositionX, WindowPositionY);//ウィンドウの位置の指定
	glutInitWindowSize(WindowWidth, WindowHeight); //ウィンドウサイズの指定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//ディスプレイモードの指定
	glutCreateWindow(WindowTitle);  //ウィンドウの作成
	glutDisplayFunc(Display); //描画時に呼び出される関数を指定する（関数名：Display）
	glutKeyboardFunc(Keyboard);//キーボード入力時に呼び出される関数を指定する（関数名：Keyboard）

	glutMouseFunc(mouse_on);      //マウスクリック時に呼び出される関数
	glutMotionFunc(mouse_motion);      //マウスドラッグ解除時に呼び出される関数

	glutIdleFunc(Idle);       //プログラムアイドル状態時に呼び出される関数
	Initialize(); //初期設定の関数を呼び出す
	glutMainLoop();

	return;
}

//----------------------------------------------------
// 初期設定の関数
//----------------------------------------------------
void CMain::Initialize(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); //背景色
	glEnable(GL_DEPTH_TEST);//デプスバッファを使用：glutInitDisplayMode() で GLUT_DEPTH を指定する

	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);

	findPlane(floor_planar,	floor_v.v0,	floor_v.v1,	floor_v.v2);

	// ディスプレイリストを作成
	listNumber = glGenLists(1);
	glNewList(listNumber, GL_COMPILE);

	glEndList();
	//--------------------------------------
	// マウスポインタ位置のウィンドウ内の相対的位置への換算用
	sx = 1.0 / (double)512;
	sy = 1.0 / (double)512;

	// 回転行列の初期化
	qrot(rt, cq);
	//--------------------------------------


	//透視変換行列の設定------------------------------
	glMatrixMode(GL_PROJECTION);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();//行列の初期化
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1000.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);

	//光源の設定--------------------------------------
	GLfloat light_position0[] = { -50.0, -50.0, 20.0, 1.0 }; //光源0の座標
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0); //光源0を設定

}
//----------------------------------------------------
// 描画の関数
//----------------------------------------------------
void CMain::Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去

	t = dt * tn;

	//ViewPointX = -100.0 * cos(omega * t);
	//ViewPointY = -100.0 * sin(omega * t);
	//ViewPointY += (-50.0 - ViewPointY) * 0.001;


	//透視変換行列の設定------------------------------
#if 0
	glMatrixMode(GL_PROJECTION);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();//行列の初期化
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1000.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);
#endif
	//----------------------------------------------


	//光源の設定--------------------------------------
	GLfloat light_position0[] = { 0.0, 50.0, 50.0, 1.0 }; //光源0の座標
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0); //光源0を
																				   
	//視点の設定------------------------------
	gluLookAt(
/*
		ViewPointX, ViewPointY, ViewPointZ, // 視点の位置x,y,z;
		0.0, 0.0, ViewPointZ,   // 視界の中心位置の参照点座標x,y,z
*/
		0.0, -100.0, 50.0, // 視点の位置x,y,z;
		0.0, 100.0, 0.0,   // 視界の中心位置の参照点座標x,y,z
		0.0, 0.0, 1.0);  //視界の上方向のベクトルx,y,z

	//モデルビュー変換行列の設定--------------------------
	glMatrixMode(GL_MODELVIEW);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();//行列の初期化
	glViewport(0, 0, WindowWidth, WindowHeight);
	//----------------------------------------------

	glPushMatrix();

	//ステンシルバッファクリア値の設定--------------------------
	glClearStencil(0);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	//----------------------------------------

	// 平面射影行列の算出--------------------------
	shadowMatrix(pM, floor_planar, lightpos);
	//--------------------------

	//陰影ON-----------------------------
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);//光源0を利用
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	//-----------------------------------

	////回転///////////////////////////////////////////////
	glMultMatrixd(rt);
	///////////////////////////////////////////////////////


#if 1
	//球
	glPushMatrix();
    //glColor3d(1.0, 0.0, 0.0); //色の設定
	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
	glTranslated(0.0, 10.0, 20.0);//平行移動値の設定
	glutSolidSphere(4.0, 20, 20);//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
	glPopMatrix();

	//立方体
	glPushMatrix();
	//glColor3d(0.0, 1.0, 0.0);//色の設定
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glTranslated(-20.0, 0.0, 20.0);//平行移動値の設定
	glutSolidCube(10.0);//引数：(一辺の長さ)
	glPopMatrix();

	//円錐
	glPushMatrix();
	//glColor3d(0.0, 0.0, 1.0);//色の設定
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glTranslated(20.0, 100.0, 0.0);//平行移動値の設定
	glutSolidCone(5.0, 10.0, 20, 20);//引数：(半径, 高さ, Z軸まわりの分割数, Z軸に沿った分割数)
	glPopMatrix();

	//直方体
	glPushMatrix();
	//glColor3d(0.0, 1.0, 1.0);//色の設定
	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_jade.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_jade.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_jade.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_jade.shininess);
	glTranslated(30.0, 50.0, 0.0);//平行移動値の設定
	glBegin(GL_QUADS);
	for (int j = 0; j < 6; ++j) {
		glNormal3dv(normal[j]); //法線ベクトルの指定

		for (int i = 0; i < 4; ++i) {
			glVertex3dv(vertex[face[j][i]]);
		}
	}

	glEnd();
	glPopMatrix();
#endif
#if 0
	for (int i = 1; i <= pn; i++) {
		p[i].vx += ax * dt;
		p[i].vy += ay * dt;
		p[i].vz += az * dt;
		p[i].x += p[i].vx * dt;
		p[i].y += p[i].vy * dt;
		p[i].z += p[i].vz * dt;
		if (p[i].z < 4) {
			p[i].z = 4.0;
			p[i].vz = -hanpatu * p[i].vz;
		}
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
		glTranslated(p[i].x, p[i].y, p[i].z);//平行移動値の設定
		glutSolidSphere(4.0, 20, 20);//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
		glPopMatrix();
	}
#endif
	glPushMatrix();
		DrawStructure(false);
		DrawShadow();
	glPopMatrix();

	glDisable(GL_AUTO_NORMAL);
	glDisable(GL_NORMALIZE);

	//陰影OFF-----------------------------
	glDisable(GL_LIGHTING);
	//-----------------------------------

	Ground();

	//文字の描画
	char t_char[20];
	char t_char2[20];

	strcpy_s(t_char2, "t = ");
	sprintf_s(t_char, "%d", tn);
	strcat_s(t_char2, t_char);
	DISPLAY_TEXT(3, 93, t_char2);

	strcpy_s(t_char2, "p = ");
	sprintf_s(t_char, "%d", pn);
	strcat_s(t_char2, t_char);
	DISPLAY_TEXT(3, 89, t_char2);


#if _BITMAP
	ostringstream fname;
	int tt = tn + 10000;
	fname << "bitmap/" << tt << ".bmp";//出力ファイル名
	string name = fname.str();
	gs.screenshot(name.c_str(), 24);
	tn++;

#else
	
	if (_Bitmap) {
		ostringstream fname;
		int tt = tn + 10000;
		fname << "bitmap/" << tt << ".bmp";//出力ファイル名
		string name = fname.str();
		gs.screenshot(name.c_str(), 24);
	}
	tn++;
#endif
	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}

//----------------------------------------------------
// アイドル時に呼び出される関数
//----------------------------------------------------
void CMain::Idle() {
	glutPostRedisplay(); //glutDisplayFunc()を１回実行する
}


//----------------------------------------------------
// 大地の描画
//----------------------------------------------------
void CMain::Ground(void) {
	double ground_max_x = 300.0;
	double ground_max_y = 300.0;
	glColor3d(0.8, 0.8, 0.8);  // 大地の色
	glBegin(GL_LINES);
	for (double ly = -ground_max_y; ly <= ground_max_y; ly += 10.0) {
		glVertex3d(-ground_max_x, ly, -1.1);
		glVertex3d(ground_max_x, ly, -1.1);
	}
	for (double lx = -ground_max_x; lx <= ground_max_x; lx += 10.0) {
		glVertex3d(lx, ground_max_y, -1.1);
		glVertex3d(lx, -ground_max_y, -1.1);
	}
	glEnd();
}

//----------------------------------------------------
// キーボード入力時に呼び出される関数
//----------------------------------------------------
void CMain::Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':
		pn++;
		p[pn].x = -0.0;
		p[pn].y = -10.0;
		p[pn].z = 5.0;
		p[pn].vx = vx * ((double)rand() / (double)RAND_MAX - (double)rand() / (double)RAND_MAX);
		p[pn].vy = vy * ((double)rand() / (double)RAND_MAX - (double)rand() / (double)RAND_MAX);
		p[pn].vz = vz * ((double)rand() / (double)RAND_MAX);

		break;
	case 'q':
		exit(0);
		break;

	default:
		break;
	}
}

//----------------------------------------------------
// 物体の描画
//----------------------------------------------------
void CMain::DrawStructure(bool flag) {
	for (int i = 1; i <= pn; i++) {
		p[i].vx += ax * dt;
		p[i].vy += ay * dt;
		p[i].vz += az * dt;
		p[i].x += p[i].vx * dt;
		p[i].y += p[i].vy * dt;
		p[i].z += p[i].vz * dt;
		if (p[i].z < 4 && abs(p[i].x) < floor_s  && abs(p[i].y) < floor_s) {
			p[i].z = 4.0;
			p[i].vz = -hanpatu * p[i].vz;
		}
		if (!flag || p[i].z >0) {
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
			glTranslated(p[i].x, p[i].y, p[i].z);//平行移動値の設定
			glutSolidSphere(4.0, 20, 20);//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
			glPopMatrix();
		}
	}
}

//----------------------------------------------------
// 床平面の方程式と行列の計算
//----------------------------------------------------
void CMain::findPlane(
	GLfloat plane[4],  // 作成する平面方程式の係数
	GLfloat v0[3],    // 頂点１
	GLfloat v1[3],    // 頂点２
	GLfloat v2[3])    // 頂点３
{

//外積で法線ベクトルを求めて平面の方程式の係数を求めている
// Ax+By+Cz+D=0 ABCD->plane[0]-[3]
	GLfloat vec0[3], vec1[3];

	// Need 2 vectors to find cross product.
	vec0[0] = v1[0] - v0[0];
	vec0[1] = v1[1] - v0[1];
	vec0[2] = v1[2] - v0[2];

	vec1[0] = v2[0] - v0[0];
	vec1[1] = v2[1] - v0[1];
	vec1[2] = v2[2] - v0[2];

	// find cross product to get A, B, and C of plane equation
	//外積を求めている
	plane[0] = vec0[1] * vec1[2] - vec0[2] * vec1[1];
	plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);
	plane[2] = vec0[0] * vec1[1] - vec0[1] * vec1[0];
	//外積と頂点1との内積を求めている
	plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);
}

// 平面射影行列の算出--------------------------
void CMain::shadowMatrix(
	GLfloat *m,      // 作成する行列のポインタ 4x4
	GLfloat plane[4],  // 射影する表面の平面方程式の係数
	GLfloat light[4])  // 光源の同時座標値
{
	GLfloat dot;

	// Find dot product between light position vector and ground plane normal.
	dot = plane[0] * light[0] +	plane[1] * light[1] +	plane[2] * light[2] +	plane[3] * light[3];

	m[0] = dot - light[0] * plane[0];
	m[4] = 0.f - light[0] * plane[1];
	m[8] = 0.f - light[0] * plane[2];
	m[12] = 0.f - light[0] * plane[3];

	m[1] = 0.f - light[1] * plane[0];
	m[5] = dot - light[1] * plane[1];
	m[9] = 0.f - light[1] * plane[2];
	m[13] = 0.f - light[1] * plane[3];

	m[2] = 0.f - light[2] * plane[0];
	m[6] = 0.f - light[2] * plane[1];
	m[10] = dot - light[2] * plane[2];
	m[14] = 0.f - light[2] * plane[3];

	m[3] = 0.f - light[3] * plane[0];
	m[7] = 0.f - light[3] * plane[1];
	m[11] = 0.f - light[3] * plane[2];
	m[15] = dot - light[3] * plane[3];
}

//----------------------------------------------------
// 床の描画と影の描画
//----------------------------------------------------
void CMain::DrawFloor(bool bTexture) {
	if (bTexture) {
		// 床にテクスチャを使う時はココで設定する
		//  glBindTexture( GL_TEXTURE_2D, );

		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
		//    glTexCoord2f( , );
		glVertex3fv(floor_v.v0);
		//    glTexCoord2f( , );
		glVertex3fv(floor_v.v1);
		//    glTexCoord2f( , );
		glVertex3fv(floor_v.v2);
		//    glTexCoord2f( , );
		glVertex3fv(floor_v.v3);
		glEnd();
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
		glVertex3fv(floor_v.v0);
		glVertex3fv(floor_v.v1);
		glVertex3fv(floor_v.v2);
		glVertex3fv(floor_v.v3);
		glEnd();
		glEnable(GL_LIGHTING);
	}
}
void CMain::DrawShadow(void) {
	/////////////////////////////////////////////
	//床のステンシルを付ける
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, ~0);
	//これから描画するもののステンシル値にすべて１タグをつける
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glColor4f(0.7f, 0.4f, 0.0f, 1.0f);
	DrawFloor(true);//床の描画

	/////////////////////////////////////////////
	//カラー・デプスバッファマスクをセットする
	//これで以下の内容のピクセルの色の値は、書き込まれない。
	glColorMask(0, 0, 0, 0);
	glDepthMask(0);
	/////////////////////////////////////////////
	//床にオブジェクトの影のステンシルを付ける
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, ~0);
	//これから描画するもののステンシル値にすべて１タグをつける
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glMultMatrixf(pM);
	DrawStructure(true);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	/////////////////////////////////////////////
	//ビットマスクを解除
	glColorMask(1, 1, 1, 1);
	glDepthMask(1);

	/////////////////////////////////////////////
	//影をつける
	glStencilFunc(GL_EQUAL, 2, ~0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
	glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
	glDisable(GL_DEPTH_TEST);
	DrawFloor(false);//床の描画
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
}

void CMain::DRAW_STRING(int x, int y, char *string, void *font = GLUT_BITMAP_TIMES_ROMAN_24) {
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}
void CMain::DISPLAY_TEXT(int x, int y, char *string) {
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPushAttrib(GL_ENABLE_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);
	glCallList(list);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);

	DRAW_STRING(x, y, string, GLUT_BITMAP_TIMES_ROMAN_24);
	glEndList();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

//----------------------------------------------------
// マウスドラッグ時
//----------------------------------------------------
void CMain::mouse_motion(int x, int y) {
	double dx, dy, a;

	// マウスポインタの位置のドラッグ開始位置からの変位
	dx = (x - cx) * sx;
	dy = (y - cy) * sy;

	// マウスポインタの位置のドラッグ開始位置からの距離
	a = sqrt(dx * dx + dy * dy);

	if (a != 0.0)
	{
		// マウスのドラッグに伴う回転のクォータニオン dq を求める
		double ar = a * SCALE * 0.5;
		double as = sin(ar) / a;
		double dq[4] = { cos(ar), dy * as, dx * as, 0.0 };

		// 回転の初期値 cq に dq を掛けて回転を合成
		qmul(tq, dq, cq);

		// クォータニオンから回転の変換行列を求める
		qrot(rt, tq);
	}
}
//----------------------------------------------------
// マウスクリック時
//----------------------------------------------------
void CMain::mouse_on(int button, int state, int x, int y)
{
	switch (button) {
	case 0:
		switch (state) {
		case 0:
			// ドラッグ開始点を記録
			cx = x;
			cy = y;
			break;
		case 1:
			// 回転の保存
			cq[0] = tq[0];
			cq[1] = tq[1];
			cq[2] = tq[2];
			cq[3] = tq[3];
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	cout << x << " " << y << endl;
}

//////////////////////////////////////////////////////////////////////////
// マウスドラッグによる回転
//////////////////////////////////////////////////////////////////////////
// クォータニオンの積 r <- p x q
void CMain::qmul(double r[], const double p[], const double q[])
{
	r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3];
	r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2];
	r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1];
	r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0];
}

// 回転の変換行列 r <- クォータニオン q
void CMain::qrot(double r[], double q[]) {
	double x2 = q[1] * q[1] * 2.0;
	double y2 = q[2] * q[2] * 2.0;
	double z2 = q[3] * q[3] * 2.0;
	double xy = q[1] * q[2] * 2.0;
	double yz = q[2] * q[3] * 2.0;
	double zx = q[3] * q[1] * 2.0;
	double xw = q[1] * q[0] * 2.0;
	double yw = q[2] * q[0] * 2.0;
	double zw = q[3] * q[0] * 2.0;

	r[0] = 1.0 - y2 - z2;
	r[1] = xy + zw;
	r[2] = zx - yw;
	r[4] = xy - zw;
	r[5] = 1.0 - z2 - x2;
	r[6] = yz + xw;
	r[8] = zx + yw;
	r[9] = yz - xw;
	r[10] = 1.0 - x2 - y2;
	r[3] = r[7] = r[11] = r[12] = r[13] = r[14] = 0.0;
	r[15] = 1.0;
}

void CMain::mouse_wheel(float z) {
	camera_z_pos += z;

	if (camera_z_pos < 0.0f)
	{
		camera_z_pos = 0.0f;
	}
}