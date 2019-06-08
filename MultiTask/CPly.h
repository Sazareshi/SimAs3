#pragma once
#include "ThreadObj.h"
#include "CVector3.h"


class CPly :
	public CThreadObj
{
public:
	CPly();
	~CPly();
	void routine_work(void *param);

private:
	double countf;
};

