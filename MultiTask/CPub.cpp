#include "stdafx.h"
#include "CPub.h"
#include "solveSimultaneousEquations.h"


CPub::CPub(){
	WindowPositionX = 600;
	WindowPositionY = 100;
	WindowWidth = 800;
	WindowHeight = 600;
}
CPub::~CPub(){
}

int CPub::WindowPositionX;					//生成するウィンドウ位置のX座標
int CPub::WindowPositionY;					//生成するウィンドウ位置のY座標
int CPub::WindowWidth;						//生成するウィンドウの幅
int CPub::WindowHeight;						//生成するウィンドウの高さ

char CPub::WindowTitle[128] = "世界の始まり"; //ウィンドウのタイト
GLfloat CPub::floor_planar[4];
GLfloat CPub::floor_s = 50.0f;
GLfloat CPub::pM[16];
GLfloat CPub::lightpos[4] = { -30, -100, 50, 1 };

QUADS_VERTEX CPub::floor_v = { { floor_s,  floor_s, -1.0f },{ -floor_s,  floor_s, -1.0f },{ -floor_s, -floor_s, -1.0f },{ floor_s, -floor_s, -1.0f }, };

bool CPub::_Bitmap = false;

double CPub::PI = acos(-1.0);
int CPub::tn = 0;
double CPub::t = 0;
double CPub::dt = 0.005;
double CPub::omega = 2.0 * PI / 10.0;
gl_screenshot CPub::gs; //bmpファイルの出力

Vector3 CPub::ViewPoint(0.0, -100.0, 200.0);
Vector3 CPub::Pos_Sphere(0.0,0.0,0.0);
Vector3 CPub::Pos_Box(0.0, 0.0, 40.0);
double CPub::hanpatu = 0.9;

//直方体の定義
GLdouble CPub::vertex[][3] = {{ 0.0, 0.0, 0.0 },{ 2.0, 0.0, 0.0 },{ 2.0, 2.0, 0.0 },{ 0.0, 2.0, 0.0 },{ 0.0, 0.0, 30.0 },{ 2.0, 0.0, 30.0 },{ 2.0, 2.0, 30.0 },{ 0.0, 2.0, 30.0 }};
//面の定義
int CPub::face[][4] = {{ 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 5, 4, 7, 6 },{ 4, 0, 3, 7 },{ 4, 5, 1, 0 },{ 3, 2, 6, 7 }};
//面の法線ベクトル
GLdouble CPub::normal[][3] = {{ 0.0, 0.0,-1.0 },{ 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0 },{ -1.0, 0.0, 0.0 },{ 0.0,-1.0, 0.0 },{ 0.0, 1.0, 0.0 }};

int CPub::cx, CPub::cy;                // ドラッグ開始位置
double CPub::sx, CPub::sy;              // マウスの絶対位置→ウィンドウ内での相対位置の換算係数
double CPub::cq[4] = { 1.0, 0.0, 0.0, 0.0 };  // 回転の初期値 (クォータニオン)
double CPub::tq[4];              // ドラッグ中の回転 (クォータニオン)
double CPub::rt[16];              // 回転の変換行列

unsigned int CPub::listNumber;
float CPub::camera_z_pos = 50.0;


//----------------------------------------------------
// 色の定義
//----------------------------------------------------
//jade(翡翠)
MaterialStruct CPub::ms_jade = {{ 0.135,     0.2225,   0.1575,   1.0 },{ 0.54,      0.89,     0.63,     1.0 },{ 0.316228,  0.316228, 0.316228, 1.0 },12.8 };
//ruby(ルビー)
MaterialStruct CPub::ms_ruby = {{ 0.1745,   0.01175,  0.01175,   1.0 },{ 0.61424,  0.04136,  0.04136,   1.0 },{ 0.727811, 0.626959, 0.626959,  1.0 },76.8 };

GLfloat CPub::red[] = { 0.8, 0.2, 0.2, 1.0 }; //赤色
GLfloat CPub::green[] = { 0.2, 0.8, 0.2, 1.0 };//緑色
GLfloat CPub::blue[] = { 0.2, 0.2, 0.8, 1.0 };//青色
GLfloat CPub::yellow[] = { 0.8, 0.8, 0.2, 1.0 };//黄色
GLfloat CPub::white[] = { 1.0, 1.0, 1.0, 1.0 };//白色
GLfloat CPub::shininess = 30.0;//光沢の強さ
//-----------------------------------------

int CPub::list;


LRESULT CALLBACK  CPub::PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
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

void CPub::set_panel_tip_txt()
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
void CPub::ActOpenGL(void) {

	glutInit(&__argc, __argv);//環境の初期化
	glutInitWindowPosition(WindowPositionX, WindowPositionY);//ウィンドウの位置の指定
	glutInitWindowSize(WindowWidth, WindowHeight); //ウィンドウサイズの指定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//ディスプレイモードの指定
	glutCreateWindow(WindowTitle);		//ウィンドウの作成
	glutDisplayFunc(Display);			//描画時に呼び出される関数を指定する（関数名：Display）
	glutIdleFunc(Idle);					//プログラムアイドル状態時に呼び出される関数

	glutMouseWheelFunc(mouse_wheel);	//マウスホイール操作時に呼び出される関数
	Initialize();						//初期設定の関数を呼び出す

	glutMainLoop();

	return;
}

//----------------------------------------------------
// 初期設定の関数
//----------------------------------------------------
void CPub::Initialize(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); //背景色

	glEnable(GL_DEPTH_TEST);//デプスバッファを使用：glutInitDisplayMode() で GLUT_DEPTH を指定する

	//光源の設定--------------------------------------
	GLfloat light_position0[] = { -50.0, -50.0, 20.0, 1.0 }; //光源0の座標
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0); //光源0をセット

}
//----------------------------------------------------
// 描画の関数
//----------------------------------------------------
void CPub::Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去

	t = dt * tn;

	//透視変換行列の設定------------------------------
	glMatrixMode(GL_PROJECTION);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();//行列の初期化
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1000.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);

	gluLookAt(
		ViewPoint.x, ViewPoint.y, ViewPoint.z, // 視点の位置x,y,z;
		0.0, 0.0, 0.0,   // 視界の中心位置の参照点座標x,y,z
		0.0, 0.0, 1.0);  //視界の上方向のベクトルx,y,z


   //モデルビュー変換行列の設定--------------------------
	glMatrixMode(GL_MODELVIEW);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();//行列の初期化
	glViewport(0, 0, WindowWidth, WindowHeight);
	//----------------------------------------------


	//陰影ON-----------------------------
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);//光源0を利用
	//-----------------------------------
	
	//球 描画
	Pos_Sphere.x = 10.0 * cos(omega * t);
	Pos_Sphere.y = 10.0 * sin(omega * t);
	
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
	glTranslated(Pos_Sphere.x, Pos_Sphere.y, Pos_Sphere.z);//平行移動値の設定
	glutSolidSphere(4.0, 20, 20);//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
	glPopMatrix();

	//立方体　描画

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glTranslated(Pos_Box.x, Pos_Box.y, Pos_Box.z);//平行移動値の設定
	glutSolidCube(3.0);//引数：(一辺の長さ)
	glPopMatrix();


	//ロープ描画

	glColor3d(0.0, 0.0, 1.0);
	glLineWidth(2.0);

	glBegin(GL_LINES);
	glVertex3d(Pos_Sphere.x, Pos_Sphere.y, Pos_Sphere.z);
	glVertex3d(Pos_Box.x, Pos_Box.y, Pos_Box.z);
	glEnd();

	//陰影OFF-----------------------------
	glDisable(GL_LIGHTING);
	//-----------------------------------

	Ground();

	tn++;


	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}

//----------------------------------------------------
// アイドル時に呼び出される関数
//----------------------------------------------------
void CPub::Idle() {
	glutPostRedisplay(); //glutDisplayFunc()を１回実行する
}


//----------------------------------------------------
// 大地の描画
//----------------------------------------------------
void CPub::Ground(void) {
	double ground_max_x = 300.0;
	double ground_max_y = 300.0;
	glColor3d(0.8, 0.8, 0.8);  // 大地の色

	glBegin(GL_LINES);
	for (double ly = -ground_max_y; ly <= ground_max_y; ly += 10.0) {
		glVertex3d(-ground_max_x, ly, 0);
		glVertex3d(ground_max_x, ly, 0);
	}
	for (double lx = -ground_max_x; lx <= ground_max_x; lx += 10.0) {
		glVertex3d(lx, ground_max_y, 0);
		glVertex3d(lx, -ground_max_y, 0);
	}
	glEnd();
}

//----------------------------------------------------
// キーボード入力時に呼び出される関数
//----------------------------------------------------
void CPub::Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':

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
void CPub::DrawStructure(bool flag) {
	;
}

//----------------------------------------------------
// 床平面の方程式と行列の計算
//----------------------------------------------------
void CPub::findPlane(
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
void CPub::shadowMatrix(
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
void CPub::DrawFloor(bool bTexture) {
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
void CPub::DrawShadow(void) {
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
void CPub::DRAW_STRING(int x, int y, char *string, void *font = GLUT_BITMAP_TIMES_ROMAN_24) {
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}
void CPub::DISPLAY_TEXT(int x, int y, char *string) {
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
void CPub::mouse_motion(int x, int y) {
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
void CPub::mouse_on(int button, int state, int x, int y)
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
void CPub::qmul(double r[], const double p[], const double q[])
{
	r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3];
	r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2];
	r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1];
	r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0];
}

// 回転の変換行列 r <- クォータニオン q
void CPub::qrot(double r[], double q[]) {
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


void CPub::mouse_wheel(int wheel_number, int direction, int x, int y) {
	
	if (direction == 1) { 
		ViewPoint.z += 5.0;
	}
	else {
		ViewPoint.z -= 5.0;
	}

	if (ViewPoint.z < 0.0f)
	{
		ViewPoint.z = 0.0f;
	}

}