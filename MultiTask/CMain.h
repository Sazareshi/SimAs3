#pragma once
#include "ThreadObj.h"
class CMain :	public CThreadObj
{
public:
	CMain();
	~CMain();

	static int WindowPositionX;					//��������E�B���h�E�ʒu��X���W
	static int WindowPositionY;					//��������E�B���h�E�ʒu��Y���W
	static int WindowWidth;						//��������E�B���h�E�̕�
	static int WindowHeight;						//��������E�B���h�E�̍���
	static char WindowTitle[128];// = "���E�̎n�܂�"; //�E�B���h�E�̃^�C�g
	 // Window Appli�ɂ��邽�߂̃_�~�[����
	static int dummy_argc;
	static char* argv;
	static char** dummy_argv;

	LRESULT CALLBACK PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void set_panel_tip_txt();//�^�u�p�l����Static�e�L�X�g��ݒ�

//OPEN GL---------------------------------------------
	static void Initialize(void);
	static void Display(void);

	static void ActOpenGL(void);
//OPEN GL---------------------------------------------

};

