#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"
#include "krawedz_mac.h"
#include "lista.h"
#include "kopiec.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//manageMenu();

	MGraf *graf = new MGraf();
	graf->losujGraf(4, 0.5);

	graf->pokazGraf();

	_getch();
	return 0;
}

