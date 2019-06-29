#pragma once
#include "ThreadObj.h"
#include "CVector3.h"
#include "CPub.h"

class CPly :
	public CThreadObj
{
public:
	CPly();
	~CPly();

	double t=0.0;
	double dt=0.01;

	void routine_work(void *param);
	void init_task(void *pobj); 
	LRESULT CALLBACK PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void set_panel_tip_txt();//�^�u�p�l����Static�e�L�X�g��ݒ�
	void set_panel_pb_txt(); //Function PB�̃e�L�X�g�Z�b�g

private:
	double countf;
	Vector3 v_box_ref;
	void set_v_box_ref(WORD wp);
};



