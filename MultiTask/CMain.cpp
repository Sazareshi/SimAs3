#include "stdafx.h"
#include "CMain.h"


CMain::CMain(){
	WindowPositionX = 700;
	WindowPositionY = 100;
	WindowWidth = 512;
	WindowHeight = 512;
	dummy_argc = 1;
	argv = WindowTitle;
	dummy_argv = &argv;


}
CMain::~CMain(){
}

char CMain::WindowTitle[128] = "���E�̎n�܂�"; //�E�B���h�E�̃^�C�g
int CMain::WindowPositionX;					//��������E�B���h�E�ʒu��X���W
int CMain::WindowPositionY;					//��������E�B���h�E�ʒu��Y���W
int CMain::WindowWidth;						//��������E�B���h�E�̕�
int CMain::WindowHeight;						//��������E�B���h�E�̍���
int CMain::dummy_argc;
char* CMain::argv;
char** CMain::dummy_argv;
bool CMain::_Bitmap = false;

double CMain::PI = acos(-1.0);
int CMain::tn = 0;
double CMain::t = 0;
double CMain::dt = 0.005;
double CMain::omega = 2.0 * PI / 10.0;
gl_screenshot CMain::gs; //bmp�t�@�C���̏o��
st_p CMain::p[100];
int CMain::pn = 0;
double CMain::ax = 0.0, CMain::ay = 0.0, CMain::az = -2.0;
double CMain::vx = 5.0, CMain::vy = 10.0, CMain::vz = 20.0;
double CMain::hanpatu = 0.9;
GLfloat CMain::floor_planar[4];
GLfloat CMain::floor_s = 50.0f;
GLfloat CMain::pM[16];
GLfloat CMain::lightpos[4] = { -30, -100, 50, 1 };
QUADS_VERTEX CMain::floor_v = {{ floor_s,  floor_s, -1.0f },{ -floor_s,  floor_s, -1.0f },{ -floor_s, -floor_s, -1.0f },{ floor_s, -floor_s, -1.0f },};

//�����̂̒�`
GLdouble CMain::vertex[][3] = {{ 0.0, 0.0, 0.0 },{ 2.0, 0.0, 0.0 },{ 2.0, 2.0, 0.0 },{ 0.0, 2.0, 0.0 },{ 0.0, 0.0, 30.0 },{ 2.0, 0.0, 30.0 },{ 2.0, 2.0, 30.0 },{ 0.0, 2.0, 30.0 }};
//�ʂ̒�`
int CMain::face[][4] = {{ 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 5, 4, 7, 6 },{ 4, 0, 3, 7 },{ 4, 5, 1, 0 },{ 3, 2, 6, 7 }};
//�ʂ̖@���x�N�g��
GLdouble CMain::normal[][3] = {{ 0.0, 0.0,-1.0 },{ 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0 },{ -1.0, 0.0, 0.0 },{ 0.0,-1.0, 0.0 },{ 0.0, 1.0, 0.0 }};

//----------------------------------------------------
// �F�̒�`
//----------------------------------------------------
//jade(�Ő�)
MaterialStruct CMain::ms_jade = {{ 0.135,     0.2225,   0.1575,   1.0 },{ 0.54,      0.89,     0.63,     1.0 },{ 0.316228,  0.316228, 0.316228, 1.0 },12.8 };
//ruby(���r�[)
MaterialStruct CMain::ms_ruby = {{ 0.1745,   0.01175,  0.01175,   1.0 },{ 0.61424,  0.04136,  0.04136,   1.0 },{ 0.727811, 0.626959, 0.626959,  1.0 },76.8 };

GLfloat CMain::red[] = { 0.8, 0.2, 0.2, 1.0 }; //�ԐF
GLfloat CMain::green[] = { 0.2, 0.8, 0.2, 1.0 };//�ΐF
GLfloat CMain::blue[] = { 0.2, 0.2, 0.8, 1.0 };//�F
GLfloat CMain::yellow[] = { 0.8, 0.8, 0.2, 1.0 };//���F
GLfloat CMain::white[] = { 1.0, 1.0, 1.0, 1.0 };//���F
GLfloat CMain::shininess = 30.0;//����̋���
//-----------------------------------------

double CMain::ViewPointX = 0.0;
double CMain::ViewPointY = -50.0;
double CMain::ViewPointZ = 20.0;


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
			//�T���v���Ƃ��Ă��낢��Ȍ^�œǂݍ���ŕ\�����Ă���
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

	if (_Bitmap) _mkdir("bitmap"); //bmp�t�@�C���ۑ��p�̃t�H���_�̍쐬

	glutInit(&dummy_argc, dummy_argv);//���̏�����
	glutInitWindowPosition(WindowPositionX, WindowPositionY);//�E�B���h�E�̈ʒu�̎w��
	glutInitWindowSize(WindowWidth, WindowHeight); //�E�B���h�E�T�C�Y�̎w��
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//�f�B�X�v���C���[�h�̎w��
	glutCreateWindow(WindowTitle);  //�E�B���h�E�̍쐬
	glutDisplayFunc(Display); //�`�掞�ɌĂяo�����֐����w�肷��i�֐����FDisplay�j
	glutKeyboardFunc(Keyboard);//�L�[�{�[�h���͎��ɌĂяo�����֐����w�肷��i�֐����FKeyboard�j

	glutIdleFunc(Idle);       //�v���O�����A�C�h����Ԏ��ɌĂяo�����֐�
	Initialize(); //�����ݒ�̊֐����Ăяo��
	glutMainLoop();

	return;
}

//----------------------------------------------------
// �����ݒ�̊֐�
//----------------------------------------------------
void CMain::Initialize(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); //�w�i�F
	glEnable(GL_DEPTH_TEST);//�f�v�X�o�b�t�@���g�p�FglutInitDisplayMode() �� GLUT_DEPTH ���w�肷��

	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);

	findPlane(floor_planar,	floor_v.v0,	floor_v.v1,	floor_v.v2);

	//�����ϊ��s��̐ݒ�------------------------------
	glMatrixMode(GL_PROJECTION);//�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();//�s��̏�����
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1000.0); //�������e�@�̎��̐�gluPerspactive(th, w/h, near, far);

	//�����̐ݒ�--------------------------------------
	GLfloat light_position0[] = { -50.0, -50.0, 20.0, 1.0 }; //����0�̍��W
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0); //����0��ݒ�

}
//----------------------------------------------------
// �`��̊֐�
//----------------------------------------------------
void CMain::Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�o�b�t�@�̏���


	t = dt * tn;

	//ViewPointX = -100.0 * cos(omega * t);
	//ViewPointY = -100.0 * sin(omega * t);
	//ViewPointY += (-50.0 - ViewPointY) * 0.001;


	//�����ϊ��s��̐ݒ�------------------------------
	glMatrixMode(GL_PROJECTION);//�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();//�s��̏�����
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1000.0); //�������e�@�̎��̐�gluPerspactive(th, w/h, near, far);

	//���_�̐ݒ�------------------------------
	gluLookAt(
/*
		ViewPointX, ViewPointY, ViewPointZ, // ���_�̈ʒux,y,z;
		0.0, 0.0, ViewPointZ,   // ���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
*/
		0.0, -160.0, 40, // ���_�̈ʒux,y,z;
		0.0, 0.0, 0.0,   // ���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
		0.0, 0.0, 1.0);  //���E�̏�����̃x�N�g��x,y,z

	//���f���r���[�ϊ��s��̐ݒ�--------------------------
	glMatrixMode(GL_MODELVIEW);//�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();//�s��̏�����
	glViewport(0, 0, WindowWidth, WindowHeight);
	//----------------------------------------------

	//�X�e���V���o�b�t�@�N���A�l�̐ݒ�--------------------------
	glClearStencil(0);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	//----------------------------------------

	// ���ʎˉe�s��̎Z�o--------------------------
	shadowMatrix(pM, floor_planar, lightpos);
	//--------------------------

	//�A�eON-----------------------------
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);//����0�𗘗p
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	//-----------------------------------

#if 0
	//��
	glPushMatrix();
    //glColor3d(1.0, 0.0, 0.0); //�F�̐ݒ�
	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
	glTranslated(0.0, 10.0, 20.0);//���s�ړ��l�̐ݒ�
	glutSolidSphere(4.0, 20, 20);//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
	glPopMatrix();

	//������
	glPushMatrix();
	//glColor3d(0.0, 1.0, 0.0);//�F�̐ݒ�
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glTranslated(-20.0, 0.0, 20.0);//���s�ړ��l�̐ݒ�
	glutSolidCube(10.0);//�����F(��ӂ̒���)
	glPopMatrix();

	//�~��
	glPushMatrix();
	//glColor3d(0.0, 0.0, 1.0);//�F�̐ݒ�
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glTranslated(20.0, 100.0, 0.0);//���s�ړ��l�̐ݒ�
	glutSolidCone(5.0, 10.0, 20, 20);//�����F(���a, ����, Z���܂��̕�����, Z���ɉ�����������)
	glPopMatrix();

	//������
	glPushMatrix();
	//glColor3d(0.0, 1.0, 1.0);//�F�̐ݒ�
	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_jade.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_jade.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_jade.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_jade.shininess);
	glTranslated(30.0, 50.0, 0.0);//���s�ړ��l�̐ݒ�
	glBegin(GL_QUADS);
	for (int j = 0; j < 6; ++j) {
		glNormal3dv(normal[j]); //�@���x�N�g���̎w��

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
		glTranslated(p[i].x, p[i].y, p[i].z);//���s�ړ��l�̐ݒ�
		glutSolidSphere(4.0, 20, 20);//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
		glPopMatrix();
	}
#endif
	glPushMatrix();
		DrawStructure(false);
		DrawShadow();
	glPopMatrix();

	glDisable(GL_AUTO_NORMAL);
	glDisable(GL_NORMALIZE);

	//�A�eOFF-----------------------------
	glDisable(GL_LIGHTING);
	//-----------------------------------

	Ground();

#if _BITMAP
	ostringstream fname;
	int tt = tn + 10000;
	fname << "bitmap/" << tt << ".bmp";//�o�̓t�@�C����
	string name = fname.str();
	gs.screenshot(name.c_str(), 24);
	tn++;

#else
	
	if (_Bitmap) {
		ostringstream fname;
		int tt = tn + 10000;
		fname << "bitmap/" << tt << ".bmp";//�o�̓t�@�C����
		string name = fname.str();
		gs.screenshot(name.c_str(), 24);
	}
	tn++;
#endif
	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)�Ń_�u���o�b�t�@�����O�𗘗p��
}

//----------------------------------------------------
// �A�C�h�����ɌĂяo�����֐�
//----------------------------------------------------
void CMain::Idle() {
	glutPostRedisplay(); //glutDisplayFunc()���P����s����
}


//----------------------------------------------------
// ��n�̕`��
//----------------------------------------------------
void CMain::Ground(void) {
	double ground_max_x = 300.0;
	double ground_max_y = 300.0;
	glColor3d(0.8, 0.8, 0.8);  // ��n�̐F
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
// �L�[�{�[�h���͎��ɌĂяo�����֐�
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
		p[pn].vy = vy * ((double)rand() / (double)RAND_MAX);
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
// ���̂̕`��
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
			glTranslated(p[i].x, p[i].y, p[i].z);//���s�ړ��l�̐ݒ�
			glutSolidSphere(4.0, 20, 20);//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
			glPopMatrix();
		}
	}
}


//----------------------------------------------------
// �����ʂ̕������ƍs��̌v�Z
//----------------------------------------------------
void CMain::findPlane(
	GLfloat plane[4],  // �쐬���镽�ʕ������̌W��
	GLfloat v0[3],    // ���_�P
	GLfloat v1[3],    // ���_�Q
	GLfloat v2[3])    // ���_�R
{
	GLfloat vec0[3], vec1[3];

	// Need 2 vectors to find cross product.
	vec0[0] = v1[0] - v0[0];
	vec0[1] = v1[1] - v0[1];
	vec0[2] = v1[2] - v0[2];

	vec1[0] = v2[0] - v0[0];
	vec1[1] = v2[1] - v0[1];
	vec1[2] = v2[2] - v0[2];

	// find cross product to get A, B, and C of plane equation
	plane[0] = vec0[1] * vec1[2] - vec0[2] * vec1[1];
	plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);
	plane[2] = vec0[0] * vec1[1] - vec0[1] * vec1[0];

	plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);
}
void CMain::shadowMatrix(
	GLfloat *m,      // �쐬����s��̃|�C���^
	GLfloat plane[4],  // �ˉe����\�ʂ̕��ʕ������̌W��
	GLfloat light[4])  // �����̓������W�l
{
	GLfloat dot;

	// Find dot product between light position vector and ground plane normal.
	dot = plane[0] * light[0] +
		plane[1] * light[1] +
		plane[2] * light[2] +
		plane[3] * light[3];

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
// ���̕`��Ɖe�̕`��
//----------------------------------------------------
void CMain::DrawFloor(bool bTexture) {
	if (bTexture) {
		// ���Ƀe�N�X�`�����g�����̓R�R�Őݒ肷��
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
	//���̃X�e���V����t����
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, ~0);
	//���ꂩ��`�悷����̂̃X�e���V���l�ɂ��ׂĂP�^�O������
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glColor4f(0.7f, 0.4f, 0.0f, 1.0f);
	DrawFloor(true);//���̕`��

					/////////////////////////////////////////////
					//�J���[�E�f�v�X�o�b�t�@�}�X�N���Z�b�g����
					//����ňȉ��̓��e�̃s�N�Z���̐F�̒l�́A�������܂�Ȃ��B
	glColorMask(0, 0, 0, 0);
	glDepthMask(0);
	/////////////////////////////////////////////
	//���ɃI�u�W�F�N�g�̉e�̃X�e���V����t����
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, ~0);
	//���ꂩ��`�悷����̂̃X�e���V���l�ɂ��ׂĂP�^�O������
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glMultMatrixf(pM);
	DrawStructure(true);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	/////////////////////////////////////////////
	//�r�b�g�}�X�N������
	glColorMask(1, 1, 1, 1);
	glDepthMask(1);

	/////////////////////////////////////////////
	//�e������
	glStencilFunc(GL_EQUAL, 2, ~0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
	glDisable(GL_DEPTH_TEST);
	DrawFloor(false);//���̕`��
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
}

