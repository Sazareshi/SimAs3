#pragma once
#include "ThreadObj.h"
class CMain :	public CThreadObj
{
public:
	CMain();
	~CMain();

	LRESULT CALLBACK PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
	void set_panel_tip_txt();//�^�u�p�l����Static�e�L�X�g��ݒ�

};

