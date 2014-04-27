#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//manageMenu();

	MGraf graf;

	graf.losujGraf(5, 0.5);
	graf.pokazGraf();

	return 0;
}

