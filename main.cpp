#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//manageMenu();

	MGraf graf;

	for (int i = 0; i < 10000; i++)
	{
		graf.losujGraf(5, 0.82365);
	}

	graf.pokazGraf();

	_getch();
	return 0;
}

