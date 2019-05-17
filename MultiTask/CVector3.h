#pragma once

#ifndef __Vector3_H__INCLUDED
#define __Vector3_H__INCLUDED

#include <math.h>

class Vector3{

private:
public:
	//�v���p�e�B
	double x, y, z;
	//�R���X�g���N�^
	Vector3() {
		x = y = z = 0;
	};
	Vector3(double _x, double _y, double _z) {
		x = _x;	y = _y;	z = _z;
	}
	//�f�B�X�g���N�^
	~Vector3() {};

	//�I�[�o�[���[�h�u*=�v
	friend void operator *= (Vector3& v1, const Vector3& v2) {
		v1.x *= v2.x;	v1.y *= v2.y;	v1.z *= v2.z;
	}
	//�I�[�o�[���[�h�u+=�v
	friend void operator += (Vector3& v1, const Vector3& v2) {
		v1.x += v2.x;	v1.y += v2.y;	v1.z += v2.z;
	}
	//�I�[�o�[���[�h�u*�v
	friend Vector3 operator * (const Vector3& v1, const Vector3& v2) {
		Vector3 v;
		v.x = v1.x * v2.x;	v.y = v1.y * v2.y;	v.z = v1.z * v2.z;
		return v;
	}
	//�I�[�o�[���[�h�u*�v
	friend Vector3 operator * (const Vector3& vec, double s) {
		Vector3 v;
		v.x = vec.x * s;	v.y = vec.y * s;	v.z = vec.z * s;
		return v;
	}
	//�I�[�o�[���[�h�u*�v
	friend Vector3 operator * (double s, const Vector3 vec) {
		Vector3 v;
		v.x = vec.x * s;	v.y = vec.y * s;	v.z = vec.z * s;
		return v;
	}
	//�I�[�o�[���[�h�u+�v
	friend Vector3 operator + (const Vector3& v1, const Vector3& v2) {
		Vector3 v;
		v.x = v1.x + v2.x;	v.y = v1.y + v2.y;	v.z = v1.z + v2.z;
		return v;
	}
	//�I�[�o�[���[�h�u+�v
	friend Vector3 operator + (const Vector3& v1, double s) {
		Vector3 v;
		v.x = v1.x + s;	v.y = v1.y + s;	v.z = v1.z + s;
		return v;
	}
	//�I�[�o�[���[�h�u+�v
	friend Vector3 operator + (double s, const Vector3& v1) {
		Vector3 v;
		v.x = v1.x + s;	v.y = v1.y + s;	v.z = v1.z + s;
		return v;
	}
	//�I�[�o�[���[�h�u-�v
	friend Vector3 operator - (const Vector3& v1, const Vector3& v2) {
		Vector3 v;
		v.x = v1.x - v2.x;	v.y = v1.y - v2.y;	v.z = v1.z - v2.z;
		return v;
	}
	//�I�[�o�[���[�h�u-�v
	friend Vector3 operator - (const Vector3& v1, double s) {
		Vector3 v;
		v.x = v1.x - s;	v.y = v1.y - s;	v.z = v1.z - s;
		return v;
	}
	//�I�[�o�[���[�h�u-�v
	friend Vector3 operator - (double s, const Vector3& v1) {
		Vector3 v;
		v.x = s - v1.x;	v.y = s - v1.y;	v.z = s - v1.z;
		return v;
	}
	//�I�[�o�[���[�h�u/�v
	friend Vector3 operator / (const Vector3& v1, const Vector3& v2) {
		Vector3 v;
		v.x = v1.x / v2.x;
		v.y = v1.y / v2.y;
		v.z = v1.z / v2.z;
		return v;
	}
	//�I�[�o�[���[�h�u/�v
	friend Vector3 operator / (const Vector3& vec, double s) {
		Vector3 v;
		v.x = vec.x / s;	v.y = vec.y / s;	v.z = vec.z / s;
		return v;
	}
	//�I�[�o�[���[�h�u/�v
	friend Vector3 operator / (double s, const Vector3 vec) {
		Vector3 v;
		v.x = s / vec.x;	v.y = s / vec.y;	v.z = s / vec.z;
		return v;
	}

	//�ÓI�����o�֐�
	static double dot(Vector3&, Vector3&);           //����
	static double distanceSq(Vector3&, Vector3&);    //�����̂Q��
	static double distance(Vector3&, Vector3&);      //����
	static double lengthSq(Vector3&);                //�����̂Q��
	static double length(Vector3&);                  //����
	static Vector3 addVectors(Vector3&, Vector3&);   //�a
	static Vector3 subVectors(Vector3&, Vector3&);   //��
	static Vector3 crossVectors(Vector3&, Vector3&); //�O��
	//�����o�֐�
	Vector3 clone();                                 //�N���[��
	Vector3& set(double, double, double);            //�x�N�g�������̐ݒ�
	Vector3& copy(Vector3&);                         //�R�s�[
	Vector3& add(Vector3&);                          //�a
	Vector3& sub(Vector3&);                          //��
	Vector3& addScalor(double);                      //�X�J���[�a
	Vector3& subScalor(double);                      //�X�J���[��
	Vector3& multiply(double);                       //�X�J���[��
	Vector3& multiply(Vector3&);                     //�������Ƃ̐�
	Vector3& multiplyScalor(double);                 //�X�J���[��
	Vector3& divide(double);                         //�X�J���[��
	Vector3& divide(Vector3&);                       //�������Ƃ̏�
	Vector3& divideScalor(double);                   //�X�J���[��
	Vector3& normalize();                            //�K�i��
	Vector3& cross(Vector3&);                        //�O��
	double dot(Vector3&);                            //����
	double length();                                 //����
	double lengthSq();                               //�����̂Q��
	double angleTo(Vector3&);                        //�Ȃ��p
	double distanceTo(Vector3&);                     //����
	double distanceToSquared(Vector3&);              //�����̂Q��
	bool equals(Vector3&);                           //���l����

};

#endif // __Vector3_H__INCLUDED
