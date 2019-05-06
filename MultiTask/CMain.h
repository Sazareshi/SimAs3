#pragma once
#include "ThreadObj.h"

#define _BITMAP 0

//----------------------------------------------------
// 物質質感の定義
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

	static int WindowPositionX;					//生成するウィンドウ位置のX座標
	static int WindowPositionY;					//生成するウィンドウ位置のY座標
	static int WindowWidth;						//生成するウィンドウの幅
	static int WindowHeight;					//生成するウィンドウの高さ
	static char WindowTitle[128];// = "世界の始まり"; //ウィンドウのタイトル
	 // Window Appliにするためのダミー引数
	static int dummy_argc;
	static char* argv;
	static char** dummy_argv;

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
	static double ViewPointX;
	static double ViewPointY;
	static double ViewPointZ;
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

	static st_p p[100];
	static int pn;
	static double ax, ay, az;
	static double vx, vy, vz;
	static double hanpatu;

	#include <time.h>

	static GLfloat floor_planar[4];
	static GLfloat floor_s;
	static GLfloat pM[16];
	static GLfloat lightpos[4];
	static QUADS_VERTEX floor_v;
	
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

	static void ActOpenGL(void);



//OPEN GL---------------------------------------------

};

