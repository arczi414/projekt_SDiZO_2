#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//manageMenu();

	MGraf graf;

	for (int i = 0; i < 1000000; i++)
	{
		graf.losujGraf(5, 1);
	}

	graf.pokazGraf();

	_getch();
	return 0;
}

