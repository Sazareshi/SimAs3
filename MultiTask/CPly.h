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

private:
	double countf;
};



