#include "stdafx.h"
#include "CPub.h"

/* Sphere*/
Vector3 MOB_Sphere::A(double t, Vector3& r, Vector3& v, Vector3& ref_a) {
	return a;
}
double  MOB_Sphere::S(Vector3& r_box, Vector3& v_box, Vector3& a_box) {
	Vector3 v_ = v.clone().sub(v_box);
	double v_abs2 = v_.lengthSq();
	Vector3 vectmp;
	Vector3 vecL = vectmp.subVectors(r,r_box);
	return -(v_abs2 - a_box.dot(vecL) - G * vecL.z) / (L*L);
}

/* Box*/
Vector3 MOB_Box::A(double t, Vector3& r, Vector3& v) {
	return a;
}
Vector3 MOB_Box::A(Vector3& ref_a) {
	a.copy(ref_a);
	return a;
}

//ルンゲ・クッタ法による時間発展
void MOB_Box::timeEvolution(double t) {

	Vector3 v1 = V(t, r, v);
	Vector3 a1 = a;

	Vector3 _v1 = Vector3(r.x + v1.x*dt / 2.0, r.y + v1.y*dt / 2.0, r.z + v1.z*dt / 2.0);
	Vector3 _a1 = Vector3(v.x + a1.x*dt / 2.0, v.y + a1.y*dt / 2.0, v.z + a1.z*dt / 2.0);
	Vector3 v2 = V(t + dt / 2.0, _v1, _a1);
	Vector3 a2 = A(t + dt / 2.0, _v1, _a1);

	Vector3 _v2 = Vector3(r.x + v2.x*dt / 2.0, r.y + v2.y*dt / 2.0, r.z + v2.z*dt / 2.0);
	Vector3 _a2 = Vector3(v.x + a2.x*dt / 2.0, v.y + a2.y*dt / 2.0, v.z + a2.z*dt / 2.0);
	Vector3 v3 = V(t + dt / 2.0, _v2, _a2);
	Vector3 a3 = A(t + dt / 2.0, _v2, _a2);

	Vector3 _v3 = Vector3(r.x + v3.x*dt, r.y + v3.y*dt, r.z + v3.z*dt);
	Vector3 _a3 = Vector3(v.x + a3.x*dt, v.y + a3.y*dt, v.z + a3.z*dt);
	Vector3 v4 = V(t + dt, _v3, _a3);
	Vector3 a4 = A(t + dt, _v3, _a3);

	dr.x = dt / 6.0 * (v1.x + 2.0 * v2.x + 2.0 * v3.x + v4.x);
	dr.y = dt / 6.0 * (v1.y + 2.0 * v2.y + 2.0 * v3.y + v4.y);
	dr.z = dt / 6.0 * (v1.z + 2.0 * v2.z + 2.0 * v3.z + v4.z);
	dv.x = dt / 6.0 * (a1.x + 2.0 * a2.x + 2.0 * a3.x + a4.x);
	dv.y = dt / 6.0 * (a1.y + 2.0 * a2.y + 2.0 * a3.y + a4.y);
	dv.z = dt / 6.0 * (a1.z + 2.0 * a2.z + 2.0 * a3.z + a4.z);

}