#pragma once
#ifndef __RK4_H__INCLUDED
#define __RK4_H__INCLUDED

#include "CVector3.h"
class RK4 {
private:
public:
	//�v���p�e�B
	double dt = 0.01; //���ԊԊu
	Vector3 r;   //�ʒu�x�N�g��
	Vector3 v;   //���x�x�N�g��
	Vector3 dr;  //�ʒu�x�N�g���̕ω���
	Vector3 dv;  //���x�x�N�g���̕ω���
				 //�R���X�g���N�^
	RK4() {}
	RK4(double _dt) {
		dt = _dt;
	}
	RK4(double _dt, Vector3& _r, Vector3& _v) {
		dt = _dt;
		r.copy(_r);
		v.copy(_v);
	}
	//�f�B�X�g���N�^
	~RK4() {}
	//�����x�x�N�g����^���郁�\�b�h�@�@�p����ōĒ�`����
	virtual Vector3 A(double, Vector3&, Vector3&);
	//���x�x�N�g����^���郁�\�b�h
	Vector3 V(double, Vector3&, Vector3&);
	//���Ԕ��W���v�Z���郁�\�b�h
	void timeEvolution(double t);
};

#endif // __RK4_H__INCLUDED
