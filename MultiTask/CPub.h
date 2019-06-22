#pragma once
#include "ThreadObj.h"
#include <time.h>
#include "CVector3.h"
#include "CRK4.h"

#define _BITMAP 0
#define SCALE (2.0 * 3.14159265358979323846)  // �}�E�X�̑��Έʒu����]�p�̊��Z�W��
#define G	9.80665


//----------------------------------------------------
// ���������̒�`
//----------------------------------------------------
struct MaterialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
};

typedef struct _QUADS_VERTEX {
	GLfloat v0[3];
	GLfloat v1[3];
	GLfloat v2[3];
	GLfloat v3[3];
}QUADS_VERTEX;

//----------------------------------------------------
// �I�u�W�F�N�g��`�N���X
//----------------------------------------------------
class MOB_Sphere : public RK4 //�݉�
{
public:
	MOB_Sphere() { r.x = 0.0, r.y = 0.0, r.z = 0.0, v.x = 0.0, v.y = 0.0, v.z = 0.0; dt = 0.01; };
	MOB_Sphere(double _dt, Vector3& _r, Vector3& _v) {dt = _dt;	r.copy(_r);	v.copy(_v);	};
	~MOB_Sphere() {}

	double L;
	Vector3 a;

	void init_mob(double _dt, Vector3 r0, Vector3 v0, Vector3 a0) { dt = _dt;  r.copy(r0); v.copy(v0); a.copy(a0); return; };
	Vector3 A(double t, Vector3& r, Vector3& v, Vector3& ref_a);
	double S(Vector3& r_box, Vector3& v_box, Vector3& a_box);
};

class MOB_Box : public RK4 //�ݓ_��

{
public:
	MOB_Box() { r.x = 0.0, r.y = 0.0, r.z = 0.0, v.x = 0.0, v.y = 0.0, v.z = 0.0; dt = 0.01;};
	MOB_Box(double _dt, Vector3& _r, Vector3& _v) {	dt =_dt;r.copy(_r);v.copy(_v);	}
	~MOB_Box() {};

	Vector3 a;

	void init_mob(double _dt, Vector3 r0, Vector3 v0, Vector3 a0) { dt = _dt;  r.copy(r0); v.copy(v0); a.copy(a0); return; };
	Vector3 A(double t, Vector3& r, Vector3& v);
	Vector3 A(Vector3& ref_a);
	void timeEvolution(double t);
};

//----------------------------------------------------
// ���^�X�N����̃R�}���h�����N���X
//----------------------------------------------------
class PUB_COMMAND_HANDLER {
public:
	PUB_COMMAND_HANDLER() {};
	~PUB_COMMAND_HANDLER() {};
	Vector3 rref_box;
	Vector3 vref_box;
	Vector3 aref_box;
};


class CPub :	public CThreadObj
{
public:
	CPub();
	~CPub();

	void routine_work(void *param);
	void init_task(void *pobj);

	PUB_COMMAND_HANDLER pub_com;

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

	static bool bGLactive;


	//----------------------------------------------------
	// ���_�̒�`
	//----------------------------------------------------
	static Vector3 ViewPoint;

	//----------------------------------------------------
	// �I�u�W�F�N�g��`
	//----------------------------------------------------
	
	static MOB_Sphere* pSphere0;//���I�u�W�F�N�g
	static MOB_Box*  pBox0;//�ݓ_�I�u�W�F�N�g

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
	static void mouse_wheel(int wheel_number, int direction, int x, int y);

	static void ActOpenGL(void);
	
//OPEN GL---------------------------------------------

};

