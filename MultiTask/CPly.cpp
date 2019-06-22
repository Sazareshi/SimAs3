#include "stdafx.h"
#include "CPly.h"
#include "CPub.h"

extern CPub* pPubObj;

CPly::CPly(){
	countf = 0.0;
}

CPly::~CPly(){}

void CPly::init_task(void *pobj) {

	dt = double(inf.cycle_ms) / 1000.0;
	set_panel_tip_txt();

	return;
};

void CPly::routine_work(void *param) {

	Vector3 tmp_a(0.0, 0.0, 0.0);
	
	t = t + dt;
	double omega = 2*3.14*0.1;
	tmp_a.x =  5.0 * omega*omega * cos( t * omega);
	
	if((int(t)+0x070)&0x080) tmp_a.y = 0.1 * omega*omega;
	else tmp_a.y = -0.1 * omega*omega;
	//	tmp_a.x = 0.0 * omega*omega * cos(t * omega);

	pPubObj->pub_com.aref_box.copy(tmp_a);

	
	ws << L"I am working!" << *(inf.psys_counter);
	tweet2owner(ws.str()); ws.str(L""); ws.clear();

};
