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

int CPub::WindowPositionX;					//��������E�B���h�E�ʒu��X���W
int CPub::WindowPositionY;					//��������E�B���h�E�ʒu��Y���W
int CPub::WindowWidth;						//��������E�B���h�E�̕�
int CPub::WindowHeight;						//��������E�B���h�E�̍���

char CPub::WindowTitle[128] = "���E�̎n�܂�"; //�E�B���h�E�̃^�C�g
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
gl_screenshot CPub::gs; //bmp�t�@�C���̏o��

Vector3 CPub::ViewPoint(0.0, -100.0, 200.0);
Vector3 CPub::Pos_Sphere(0.0,0.0,0.0);
Vector3 CPub::Pos_Box(0.0, 0.0, 40.0);
double CPub::hanpatu = 0.9;

//�����̂̒�`
GLdouble CPub::vertex[][3] = {{ 0.0, 0.0, 0.0 },{ 2.0, 0.0, 0.0 },{ 2.0, 2.0, 0.0 },{ 0.0, 2.0, 0.0 },{ 0.0, 0.0, 30.0 },{ 2.0, 0.0, 30.0 },{ 2.0, 2.0, 30.0 },{ 0.0, 2.0, 30.0 }};
//�ʂ̒�`
int CPub::face[][4] = {{ 0, 1, 2, 3 },{ 1, 5, 6, 2 },{ 5, 4, 7, 6 },{ 4, 0, 3, 7 },{ 4, 5, 1, 0 },{ 3, 2, 6, 7 }};
//�ʂ̖@���x�N�g��
GLdouble CPub::normal[][3] = {{ 0.0, 0.0,-1.0 },{ 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0 },{ -1.0, 0.0, 0.0 },{ 0.0,-1.0, 0.0 },{ 0.0, 1.0, 0.0 }};

int CPub::cx, CPub::cy;                // �h���b�O�J�n�ʒu
double CPub::sx, CPub::sy;              // �}�E�X�̐�Έʒu���E�B���h�E���ł̑��Έʒu�̊��Z�W��
double CPub::cq[4] = { 1.0, 0.0, 0.0, 0.0 };  // ��]�̏����l (�N�H�[�^�j�I��)
double CPub::tq[4];              // �h���b�O���̉�] (�N�H�[�^�j�I��)
double CPub::rt[16];              // ��]�̕ϊ��s��

unsigned int CPub::listNumber;
float CPub::camera_z_pos = 50.0;


//----------------------------------------------------
// �F�̒�`
//----------------------------------------------------
//jade(�Ő�)
MaterialStruct CPub::ms_jade = {{ 0.135,     0.2225,   0.1575,   1.0 },{ 0.54,      0.89,     0.63,     1.0 },{ 0.316228,  0.316228, 0.316228, 1.0 },12.8 };
//ruby(���r�[)
MaterialStruct CPub::ms_ruby = {{ 0.1745,   0.01175,  0.01175,   1.0 },{ 0.61424,  0.04136,  0.04136,   1.0 },{ 0.727811, 0.626959, 0.626959,  1.0 },76.8 };

GLfloat CPub::red[] = { 0.8, 0.2, 0.2, 1.0 }; //�ԐF
GLfloat CPub::green[] = { 0.2, 0.8, 0.2, 1.0 };//�ΐF
GLfloat CPub::blue[] = { 0.2, 0.2, 0.8, 1.0 };//�F
GLfloat CPub::yellow[] = { 0.8, 0.8, 0.2, 1.0 };//���F
GLfloat CPub::white[] = { 1.0, 1.0, 1.0, 1.0 };//���F
GLfloat CPub::shininess = 30.0;//����̋���
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

	glutInit(&__argc, __argv);//���̏�����
	glutInitWindowPosition(WindowPositionX, WindowPositionY);//�E�B���h�E�̈ʒu�̎w��
	glutInitWindowSize(WindowWidth, WindowHeight); //�E�B���h�E�T�C�Y�̎w��
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//�f�B�X�v���C���[�h�̎w��
	glutCreateWindow(WindowTitle);		//�E�B���h�E�̍쐬
	glutDisplayFunc(Display);			//�`�掞�ɌĂяo�����֐����w�肷��i�֐����FDisplay�j
	glutIdleFunc(Idle);					//�v���O�����A�C�h����Ԏ��ɌĂяo�����֐�

	glutMouseWheelFunc(mouse_wheel);	//�}�E�X�z�C�[�����쎞�ɌĂяo�����֐�
	Initialize();						//�����ݒ�̊֐����Ăяo��

	glutMainLoop();

	return;
}

//----------------------------------------------------
// �����ݒ�̊֐�
//----------------------------------------------------
void CPub::Initialize(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); //�w�i�F

	glEnable(GL_DEPTH_TEST);//�f�v�X�o�b�t�@���g�p�FglutInitDisplayMode() �� GLUT_DEPTH ���w�肷��

	//�����̐ݒ�--------------------------------------
	GLfloat light_position0[] = { -50.0, -50.0, 20.0, 1.0 }; //����0�̍��W
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0); //����0���Z�b�g

}
//----------------------------------------------------
// �`��̊֐�
//----------------------------------------------------
void CPub::Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�o�b�t�@�̏���

	t = dt * tn;

	//�����ϊ��s��̐ݒ�------------------------------
	glMatrixMode(GL_PROJECTION);//�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();//�s��̏�����
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1000.0); //�������e�@�̎��̐�gluPerspactive(th, w/h, near, far);

	gluLookAt(
		ViewPoint.x, ViewPoint.y, ViewPoint.z, // ���_�̈ʒux,y,z;
		0.0, 0.0, 0.0,   // ���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
		0.0, 0.0, 1.0);  //���E�̏�����̃x�N�g��x,y,z


   //���f���r���[�ϊ��s��̐ݒ�--------------------------
	glMatrixMode(GL_MODELVIEW);//�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();//�s��̏�����
	glViewport(0, 0, WindowWidth, WindowHeight);
	//----------------------------------------------


	//�A�eON-----------------------------
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);//����0�𗘗p
	//-----------------------------------
	
	//�� �`��
	Pos_Sphere.x = 10.0 * cos(omega * t);
	Pos_Sphere.y = 10.0 * sin(omega * t);
	
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
	glTranslated(Pos_Sphere.x, Pos_Sphere.y, Pos_Sphere.z);//���s�ړ��l�̐ݒ�
	glutSolidSphere(4.0, 20, 20);//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
	glPopMatrix();

	//�����́@�`��

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glTranslated(Pos_Box.x, Pos_Box.y, Pos_Box.z);//���s�ړ��l�̐ݒ�
	glutSolidCube(3.0);//�����F(��ӂ̒���)
	glPopMatrix();


	//���[�v�`��

	glColor3d(0.0, 0.0, 1.0);
	glLineWidth(2.0);

	glBegin(GL_LINES);
	glVertex3d(Pos_Sphere.x, Pos_Sphere.y, Pos_Sphere.z);
	glVertex3d(Pos_Box.x, Pos_Box.y, Pos_Box.z);
	glEnd();

	//�A�eOFF-----------------------------
	glDisable(GL_LIGHTING);
	//-----------------------------------

	Ground();

	tn++;


	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)�Ń_�u���o�b�t�@�����O�𗘗p��
}

//----------------------------------------------------
// �A�C�h�����ɌĂяo�����֐�
//----------------------------------------------------
void CPub::Idle() {
	glutPostRedisplay(); //glutDisplayFunc()���P����s����
}


//----------------------------------------------------
// ��n�̕`��
//----------------------------------------------------
void CPub::Ground(void) {
	double ground_max_x = 300.0;
	double ground_max_y = 300.0;
	glColor3d(0.8, 0.8, 0.8);  // ��n�̐F

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
// �L�[�{�[�h���͎��ɌĂяo�����֐�
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
// ���̂̕`��
//----------------------------------------------------
void CPub::DrawStructure(bool flag) {
	;
}

//----------------------------------------------------
// �����ʂ̕������ƍs��̌v�Z
//----------------------------------------------------
void CPub::findPlane(
	GLfloat plane[4],  // �쐬���镽�ʕ������̌W��
	GLfloat v0[3],    // ���_�P
	GLfloat v1[3],    // ���_�Q
	GLfloat v2[3])    // ���_�R
{

//�O�ςŖ@���x�N�g�������߂ĕ��ʂ̕������̌W�������߂Ă���
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
	//�O�ς����߂Ă���
	plane[0] = vec0[1] * vec1[2] - vec0[2] * vec1[1];
	plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);
	plane[2] = vec0[0] * vec1[1] - vec0[1] * vec1[0];
	//�O�ςƒ��_1�Ƃ̓��ς����߂Ă���
	plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);
}

// ���ʎˉe�s��̎Z�o--------------------------
void CPub::shadowMatrix(
	GLfloat *m,      // �쐬����s��̃|�C���^ 4x4
	GLfloat plane[4],  // �ˉe����\�ʂ̕��ʕ������̌W��
	GLfloat light[4])  // �����̓������W�l
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
// ���̕`��Ɖe�̕`��
//----------------------------------------------------
void CPub::DrawFloor(bool bTexture) {
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
void CPub::DrawShadow(void) {
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
	//glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
	glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
	glDisable(GL_DEPTH_TEST);
	DrawFloor(false);//���̕`��
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
// �}�E�X�h���b�O��
//----------------------------------------------------
void CPub::mouse_motion(int x, int y) {
	double dx, dy, a;

	// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̕ψ�
	dx = (x - cx) * sx;
	dy = (y - cy) * sy;

	// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̋���
	a = sqrt(dx * dx + dy * dy);

	if (a != 0.0)
	{
		// �}�E�X�̃h���b�O�ɔ�����]�̃N�H�[�^�j�I�� dq �����߂�
		double ar = a * SCALE * 0.5;
		double as = sin(ar) / a;
		double dq[4] = { cos(ar), dy * as, dx * as, 0.0 };

		// ��]�̏����l cq �� dq ���|���ĉ�]������
		qmul(tq, dq, cq);

		// �N�H�[�^�j�I�������]�̕ϊ��s������߂�
		qrot(rt, tq);
	}
}
//----------------------------------------------------
// �}�E�X�N���b�N��
//----------------------------------------------------
void CPub::mouse_on(int button, int state, int x, int y)
{
	switch (button) {
	case 0:
		switch (state) {
		case 0:
			// �h���b�O�J�n�_���L�^
			cx = x;
			cy = y;
			break;
		case 1:
			// ��]�̕ۑ�
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
// �}�E�X�h���b�O�ɂ���]
//////////////////////////////////////////////////////////////////////////
// �N�H�[�^�j�I���̐� r <- p x q
void CPub::qmul(double r[], const double p[], const double q[])
{
	r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3];
	r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2];
	r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1];
	r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0];
}

// ��]�̕ϊ��s�� r <- �N�H�[�^�j�I�� q
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