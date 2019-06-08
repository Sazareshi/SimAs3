#pragma once
#include "ThreadObj.h"
#include <time.h>
#include "CVector3.h"

#define _BITMAP 0
#define SCALE (2.0 * 3.14159265358979323846)  // マウスの相対位置→回転角の換算係数

//----------------------------------------------------
// 物質質感の定義
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


class CPub :	public CThreadObj
{
public:
	CPub();
	~CPub();

	static int WindowPositionX;					//生成するウィンドウ位置のX座標
	static int WindowPositionY;					//生成するウィンドウ位置のY座標
	static int WindowWidth;						//生成するウィンドウの幅
	static int WindowHeight;					//生成するウィンドウの高さ
	static char WindowTitle[128];// = "世界の始まり"; //ウィンドウのタイトル

	static bool _Bitmap;
	static int tn;
	static double t;
	static double dt;
	static double omega;
	static gl_screenshot gs; //bmpファイルの出力
	static double PI;

	//----------------------------------------------------
	// 視点の定義
	//----------------------------------------------------
	static Vector3 ViewPoint;

	//----------------------------------------------------
	// オブジェクトの位置定義
	//----------------------------------------------------
	static Vector3 Pos_Sphere;
	static Vector3 Pos_Box;
		//----------------------------------------------------
	// 直方体の定義
	//----------------------------------------------------
	static GLdouble vertex[][3];//頂点座標
	static int face[][4];//面定義
	static GLdouble normal[][3];

	//----------------------------------------------------
	// 色の定義
	//----------------------------------------------------
	static MaterialStruct ms_jade;//jade(翡翠)
	static MaterialStruct ms_ruby;//ruby(ルビー)
	static GLfloat red[]; //赤色
	static GLfloat green[];//緑色
	static GLfloat blue[];//青色
	static GLfloat yellow[];//黄色
	static GLfloat white[];//白色
	static GLfloat shininess;//光沢の強さ

	static double hanpatu;

	static GLfloat floor_planar[4];
	static GLfloat floor_s;
	static GLfloat pM[16];
	static GLfloat lightpos[4];
	static QUADS_VERTEX floor_v;

	static int list;

	static int cx, cy;    // ドラッグ開始位置
	static double sx, sy; // マウスの絶対位置→ウィンドウ内での相対位置の換算係数
	static double cq[4];  // 回転の初期値 (クォータニオン)
	static double tq[4];  // ドラッグ中の回転 (クォータニオン)
	static double rt[16]; // 回転の変換行列

	static unsigned int listNumber;
	static float camera_z_pos;

	//-----------------------------------------

	LRESULT CALLBACK PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void set_panel_tip_txt();//タブパネルのStaticテキストを設定

//OPEN GL---------------------------------------------
	static void Initialize(void);
	static void Display(void);
	static void Ground(void); //大地の描画Keyboard
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
	static void mouse_wheel(int wheel_number, int direction, int x, int y);

	static void ActOpenGL(void);
	
//OPEN GL---------------------------------------------
	static void move_sphere(Vector3 pos) { Pos_Sphere.copy(pos); };
	static void move_box(Vector3 pos) { Pos_Box.copy(pos); };

};

