#include "stdafx.h"
#include "CPly.h"
#include "CPub.h"


CPly::CPly(){
	countf = 0.0;
}

CPly::~CPly(){}

void CPly::routine_work(void *param) {

	Vector3 tmp_pos(0.0, 0.0, 0.0);
	tmp_pos.z =  50.0 * cos( *(inf.psys_counter) * 3.14 / 1000);

	CPub::Box0.move(tmp_pos);

	countf += 1.0;
	if (countf > 50) countf = 0.0;
};
