#pragma once
#ifndef __RK4_H__INCLUDED
#define __RK4_H__INCLUDED

#include "CVector3.h"
class RK4 {
private:
public:
	//プロパティ
	double dt = 0.01; //時間間隔
	Vector3 r;   //位置ベクトル
	Vector3 v;   //速度ベクトル
	Vector3 dr;  //位置ベクトルの変化分
	Vector3 dv;  //速度ベクトルの変化分
				 //コンストラクタ
	RK4() {}
	RK4(double _dt) {
		dt = _dt;
	}
	RK4(double _dt, Vector3& _r, Vector3& _v) {
		dt = _dt;
		r.copy(_r);
		v.copy(_v);
	}
	//ディストラクタ
	~RK4() {}
	//加速度ベクトルを与えるメソッド　　継承先で再定義する
	virtual Vector3 A(double, Vector3&, Vector3&);
	//速度ベクトルを与えるメソッド
	Vector3 V(double, Vector3&, Vector3&);
	//時間発展を計算するメソッド
	void timeEvolution(double t);
};

#endif // __RK4_H__INCLUDED
