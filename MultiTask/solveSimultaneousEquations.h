#pragma once
#ifndef __solveSimultaneousEquations_H__INCLUDED
#define __solveSimultaneousEquations_H__INCLUDED

#include "stdafx.h"

void readMatrix(int I, double *M) {
	for (int i = 0; i<I; i++) {
		std::cout << M[i] << " ";
		_RPT1(_CRT_WARN, "%f\\,", M[i]);
	}
	std::cout << std::endl;
	_RPT0(_CRT_WARN, "\n");
}
void readMatrix(int I, int J, double **M) {
	for (int i = 0; i<I; i++) {
		for (int j = 0; j<J; j++) {
			std::cout << M[i][j] << " ";
			_RPT1(_CRT_WARN, "%f\\,", M[i][j]);
		}
		std::cout << std::endl;
		_RPT0(_CRT_WARN, "\n");
	}
}

//�K�E�X�̏����@�ɂ��A���������̉�
int solveSimultaneousEquations(int n, double **M, double *ans) {

	// �O�i����
	for (int k = 0; k < n - 1; k++) {

		//�s�{�b�g����
		int p = k;
		double max = abs(M[k][k]);

		for (int i = k + 1; i < n; i++) {  // �s�{�b�g�I��
			if (abs(M[i][k]) > max) {
				p = i;
				max = abs(M[i][k]);
			}
		}
		if (abs(max) < 1E-12) {
			for (int i = 0; i < n; i++) ans[i] = 0;
			std::cout << "�O�i�������̃s�{�b�g�����������܂��i�������̐�������Ȃ��\��������܂��j" << std::endl;
			_RPT0(_CRT_ERROR, "�O�i�������̃s�{�b�g�����������܂��i�������̐�������Ȃ��\��������܂��j\n");
			return 0;
		}
		if (p != k) {
			for (int i = k; i <= n; i++) {
				double tmp = M[k][i];
				M[k][i] = M[p][i];
				M[p][i] = tmp;
			}
		}

		for (int i = k + 1; i < n; i++) {
			for (int j = k + 1; j <= n; j++) {
				M[i][j] = M[i][j] - M[k][j] * M[i][k] / M[k][k];
			}
		}
	}
	for (int i = 0; i < n; i++) {
		ans[i] = 0;
	}
	// ��ޑ��
	for (int k = n - 1; k >= 0; k--) {
		for (int j = k + 1; j < n; j++) {
			M[k][n] = M[k][n] - M[k][j] * ans[j];
		}
		if (abs(M[k][k]) < 1E-12) {
			std::cout << "�O�i�������̃s�{�b�g�����������܂��i�������̐�����������\��������܂��j" << std::endl;
			_RPT0(_CRT_ERROR, "�O�i�������̃s�{�b�g�����������܂��i�������̐�����������\��������܂��j");
			ans[k] = 0;
		}
		else {
			ans[k] = M[k][n] / M[k][k];
		}

	}
	return 1;
}
#endif
