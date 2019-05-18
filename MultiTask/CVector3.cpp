#include "stdafx.h"
#include "CVector3.h"

////////////////////////////////////////////////////////////
// �ÓI�����o�֐�
////////////////////////////////////////////////////////////
//����
double Vector3::dot(Vector3& v1, Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
//�����̂Q��
double Vector3::distanceSq(Vector3& v1, Vector3& v2) {
	return (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y) + (v1.z - v2.z)*(v1.z - v2.z);
}
//����
double Vector3::distance(Vector3& v1, Vector3& v2) {
	return sqrt(distanceSq(v1, v2));
}
//�����̂Q��
double Vector3::lengthSq(Vector3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
//����
double Vector3::length(Vector3& v) {
	return sqrt(lengthSq(v));
}
//�a
Vector3 Vector3::addVectors(Vector3& v1, Vector3& v2) {
	Vector3 v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	return v;
}
//��
Vector3 Vector3::subVectors(Vector3& v1, Vector3& v2) {
	Vector3 v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	return v;
}
//�O��
Vector3 Vector3::crossVectors(Vector3& v1, Vector3& v2) {
	Vector3 v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}
////////////////////////////////////////////////////////////
// �����o�֐�
////////////////////////////////////////////////////////////
//�x�N�g�������̐ݒ�
Vector3& Vector3::set(double _x, double _y, double _z) {
	x = _x;
	y = _y;
	z = _z;
	return *this;
}
//�R�s�[
Vector3& Vector3::copy(Vector3& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}
//�N���[��
Vector3 Vector3::clone() {
	return Vector3(x, y, z);
}
//�a
Vector3& Vector3::add(Vector3& vec) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}
//��
Vector3& Vector3::sub(Vector3& vec) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}
//�X�J���[�a
Vector3& Vector3::addScalor(double s) {
	x += s;
	y += s;
	z += s;
	return *this;
}
//�X�J���[��
Vector3& Vector3::subScalor(double s) {
	x -= s;
	y -= s;
	z -= s;
	return *this;
}
//�X�J���[��
Vector3& Vector3::multiply(double s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
//�������Ƃ̐�
Vector3& Vector3::multiply(Vector3& vec) {
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	return *this;
}
//�X�J���[��
Vector3& Vector3::multiplyScalor(double s) {
	multiply(s);
	return *this;
}
//�X�J���[��
Vector3& Vector3::divide(double s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}
//�������Ƃ̏�
Vector3& Vector3::divide(Vector3& vec) {
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	return *this;
}
//�X�J���[��
Vector3& Vector3::divideScalor(double s) {
	divide(s);
	return *this;
}

//�K�i��
Vector3& Vector3::normalize() {
	double l = (*this).length();
	(*this).divideScalor(l);
	return *this;
}
//�O��
Vector3& Vector3::cross(Vector3& vec) {
	x = y * vec.z - z * vec.y;
	y = z * vec.x - x * vec.z;
	z = x * vec.y - y * vec.x;
	return *this;
}
//����
double Vector3::dot(Vector3& vec) {
	return Vector3::dot(*this, vec);
}
//����
double Vector3::length() {
	return Vector3::length(*this);
}
//�����̂Q��
double Vector3::lengthSq() {
	return Vector3::lengthSq(*this);
}
//�Ȃ��p
double Vector3::angleTo(Vector3& vec) {
	double d = (*this).dot(vec) / ((*this).length() * vec.length());
	return acos(d);
}
//����
double Vector3::distanceTo(Vector3& vec) {
	return Vector3::distance(*this, vec);
}
//�����̂Q��
double Vector3::distanceToSquared(Vector3& vec) {
	return Vector3::distanceSq(*this, vec);
}
//���l����
bool Vector3::equals(Vector3& vec) {
	return (x == vec.x) && (y == vec.y) && (z == vec.z);
}


