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
	virtual Vector3 A(double t, Vector3& r, Vector3& v);
	//���x�x�N�g����^���郁�\�b�h
	virtual Vector3 V(double t, Vector3& r, Vector3& v);
	//���Ԕ��W���v�Z���郁�\�b�h
	void timeEvolution(double t);
};

#endif // __RK4_H__INCLUDED

#ifndef __RK4_Nbody_H__INCLUDED
#define __RK4_Nbody_H__INCLUDED


class RK4_Nbody {
private:
public:
	//�v���p�e�B
	int N;                              //���̐�
	double dt;                          //���ԍ��ݕ�
	Vector3 *rs, *vs;                   //�ʒu�x�N�g���Ƒ��x�x�N�g��
	Vector3 *drs, *dvs;                 //�ʒu�x�N�g���Ƒ��x�x�N�g���̕ω���
										//�ȉ��A�����Q�E�N�b�^�@�ŗ��p����z��
	Vector3 *v1s, *a1s, *_v1s, *_a1s;   //�P�i�ڗp
	Vector3 *v2s, *a2s, *_v2s, *_a2s;   //�Q�i�ڗp
	Vector3 *v3s, *a3s, *_v3s, *_a3s;   //�R�i�ڗp
	Vector3 *v4s, *a4s;                 //�S�i�ڗp
										//�R���X�g���N�^
	RK4_Nbody(int _N, double _dt) { //�������Ƃ��K�{
		N = _N;
		dt = _dt;
		rs = new Vector3[N]; vs = new Vector3[N]; drs = new Vector3[N]; dvs = new Vector3[N];
		v1s = new Vector3[N]; a1s = new Vector3[N]; _v1s = new Vector3[N]; _a1s = new Vector3[N];
		v2s = new Vector3[N]; a2s = new Vector3[N]; _v2s = new Vector3[N]; _a2s = new Vector3[N];
		v3s = new Vector3[N]; a3s = new Vector3[N]; _v3s = new Vector3[N]; _a3s = new Vector3[N];
		v4s = new Vector3[N];
		a4s = new Vector3[N];
	}
	//�f�B�X�g���N�^
	~RK4_Nbody() {
		//���I�m�ۂ����������̊J��
		delete[] rs;  delete[] vs;  delete[] drs;  delete[] dvs;
		delete[] v1s; delete[] a1s; delete[] _v1s; delete[] _a1s;
		delete[] v2s; delete[] a2s; delete[] _v2s; delete[] _a2s;
		delete[] v3s; delete[] a3s; delete[] _v3s; delete[] _a3s;
		delete[] v4s; delete[] a4s;
	}
	//�����x�x�N�g����^���郁�\�b�h
	virtual void A(double t, Vector3 *rs, Vector3 *vs, Vector3 *out_as);
	//���x�x�N�g����^���郁�\�b�h
	virtual void V(double t, Vector3 *rs, Vector3 *vs, Vector3 *out_vs);
	//���Ԕ��W���v�Z���郁�\�b�h
	void timeEvolution(double t);
};

#endif // __RK4_Nbody_H__INCLUDED


