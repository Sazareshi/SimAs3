#pragma once
#include "ThreadObj.h"
#include <time.h>

#define _BITMAP 0
#define SCALE (2.0 * 3.14159265358979323846)  // �}�E�X�̑��Έʒu����]�p�̊��Z�W��

//----------------------------------------------------
// ���������̒�`
//----------------------------------------------------
struct MaterialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
};

struct st_p {
	double x,y,z;
	double vx, vy, vz;
};

typedef struct _QUADS_VERTEX {
	GLfloat v0[3];
	GLfloat v1[3];
	GLfloat v2[3];
	GLfloat v3[3];
}QUADS_VERTEX;


class CMain :	public CThreadObj
{
public:
	CMain();
	~CMain();

	static int WindowPositionX;					//��������E�B���h�E�ʒu��X���W
	static int WindowPositionY;					//��������E�B���h�E�ʒu��Y���W
	static int WindowWidth;						//��������E�B���h�E�̕�
	static int WindowHeight;					//��������E�B���h�E�̍���
	static char WindowTitle[128];// = "���E�̎n�܂�"; //�E�B���h�E�̃^�C�g��

	static bool _Bitmap;
	static int tn;
	static double t;
	static double dt;
	static double omega;
	static gl_screenshot gs; //bmp�t�@�C���̏o��
	static double PI;

	//----------------------------------------------------
	// ���_�̒�`
	//----------------------------------------------------
	static double ViewPointX;
	static double ViewPointY;
	static double ViewPointZ;
	//----------------------------------------------------
	// �����̂̒�`
	//----------------------------------------------------
	static GLdouble vertex[][3];//���_���W
	static int face[][4];//�ʒ�`
	static GLdouble normal[][3];

	//----------------------------------------------------
	// �F�̒�`
	//----------------------------------------------------
	static MaterialStruct ms_jade;//jade(�Ő�)
	static MaterialStruct ms_ruby;//ruby(���r�[)
	static GLfloat red[]; //�ԐF
	static GLfloat green[];//�ΐF
	static GLfloat blue[];//�F
	static GLfloat yellow[];//���F
	static GLfloat white[];//���F
	static GLfloat shininess;//����̋���

	static st_p p[100];
	static int pn;
	static double ax, ay, az;
	static double vx, vy, vz;
	static double hanpatu;


	static GLfloat floor_planar[4];
	static GLfloat floor_s;
	static GLfloat pM[16];
	static GLfloat lightpos[4];
	static QUADS_VERTEX floor_v;

	static int list;

	static int cx, cy;    // �h���b�O�J�n�ʒu
	static double sx, sy; // �}�E�X�̐�Έʒu���E�B���h�E���ł̑��Έʒu�̊��Z�W��
	static double cq[4];  // ��]�̏����l (�N�H�[�^�j�I��)
	static double tq[4];  // �h���b�O���̉�] (�N�H�[�^�j�I��)
	static double rt[16]; // ��]�̕ϊ��s��

	static unsigned int listNumber;
	static float camera_z_pos;

	//-----------------------------------------

	LRESULT CALLBACK PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void set_panel_tip_txt();//�^�u�p�l����Static�e�L�X�g��ݒ�

//OPEN GL---------------------------------------------
	static void Initialize(void);
	static void Display(void);
	static void Ground(void); //��n�̕`��Keyboard
	static void Idle();
	static void Keyboard(unsigned char key, int x, int y);

	static void findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]);
	static void shadowMatrix(GLfloat *m, GLfloat plane[4], GLfloat light[4]);
	static void DrawFloor(bool bTexture);
	static void DrawShadow(void);
	static void DrawStructure(bool);
	static void DRAW_STRING(int x, int y, char *string, void *font);
	static void DISPLAY_TEXT(int x, int y, char *string);

	static void qmul(double r[], const double p[], const double q[]);
	static void qrot(double r[], double q[]);
	static void mouse_motion(int x, int y);
	static void mouse_on(int button, int state, int x, int y);
	static void mouse_wheel(float z);

	static void ActOpenGL(void);
	
//OPEN GL---------------------------------------------

};

