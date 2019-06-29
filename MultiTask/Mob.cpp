#include "stdafx.h"
#include "CPub.h"

/* Sphere*/

#define compensationK 0.5
#define compensationGamma 0.5

Vector3 MOB_HungLoad::A(double t, Vector3& r, Vector3& v) {
	Vector3 a;
	Vector3 L_;
	
	L_= L_.subVectors(r, pHangingObj->r);
	
	double Sdivm = S()/m;

//	a.x = Sdivm * (r.x - r_box.x);
//	a.y = Sdivm * (r.y - r_box.y);
//	a.z = -G + Sdivm * (r.z - r_box.z);

	a = L_.clone().multiplyScalor(Sdivm);
	a.z -= G;

//åvéZåÎç∑Ç…ÇÊÇÈÉçÅ[Éví∑Ç∏ÇÍï‚ê≥
	Vector3 hatL = L_.clone().normalize();
	// ï‚ê≥ÇŒÇÀíeê´óÕ
	Vector3 ak = hatL.clone().multiplyScalor(-compensationK * ( pHangingObj->l- L_.length()));
	Vector3 v_ = v_.subVectors(v, pHangingObj->v);
	// ï‚ê≥îSê´íÔçRóÕ
	Vector3 agamma = hatL.clone().multiplyScalor(-compensationGamma * v_.dot(hatL));
	// í£óÕÇ…Ç–Ç‡ÇÃí∑Ç≥ÇÃï‚ê≥óÕÇâ¡Ç¶ÇÈ
	
	a.add(ak).add(agamma);
	
	return a;
}
double  MOB_HungLoad::S() {
	Vector3 v_ = v.clone().sub(pHangingObj->v);
	double v_abs2 = v_.lengthSq();
	Vector3 vectmp;
	Vector3 vecL = vectmp.subVectors(r, pHangingObj->r);
	return -m*(v_abs2 - pHangingObj->a.dot(vecL) - G * vecL.z + pHangingObj->lv*pHangingObj->lv + pHangingObj->l*pHangingObj->la) / (pHangingObj->l*pHangingObj->l);
}


/* Hanging Object*/
Vector3 MOB_HangingObj::A(double t, Vector3& r, Vector3& v) {
	return Aref;
}


//ÉãÉìÉQÅEÉNÉbÉ^ñ@Ç…ÇÊÇÈéûä‘î≠ìW
void MOB_HangingObj::timeEvolution(double t) {

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