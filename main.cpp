#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"
#include "krawedz_mac.h"
#include "lista.h"
#include "kopiec.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//manageMenu();

	/*
	MGraf graf;

	for (int i = 0; i < 10000; i++)
	{
		graf.losujGraf(5, 1);
	}

	graf.pokazGraf();
	*/

	List<MKrawedz> ls;
	Kopiec<MKrawedz> kp;

	MKrawedz k1(32, 0);
	MKrawedz k2(322, 1);
	MKrawedz k3(5321, 2);
	MKrawedz k4(74, 3);
	MKrawedz k5(1, 4);
	MKrawedz k6(12, 5);
	MKrawedz k7(64, 6);
	MKrawedz k8(86, 7);
	MKrawedz k9(960, 8);
	MKrawedz k10(556, 9);
	MKrawedz k11(2, 10);

	/*
	ls.insert(k1, k1, 1);
	ls.insert(k1, k2, 1);
	ls.insert(k1, k3, 1);
	ls.insert(k1, k4, 1);
	ls.insert(k1, k5, 1);
	ls.insert(k1, k6, 1);
	ls.insert(k1, k7, 1);
	ls.insert(k1, k8, 1);
	ls.insert(k1, k9, 1);

	ls.print();
	*/

	kp.push(k1);
	kp.push(k2);
	kp.push(k3);
	kp.push(k4);
	kp.push(k5);
	kp.push(k6);
	kp.push(k7);
	kp.push(k8);
	kp.push(k9);
	kp.push(k10);
	kp.push(k11);

	kp.print();

	_getch();
	return 0;
}

