#include "stdafx.h"
#include "menu.h"

using namespace std;

int manageMenu()
{
	char menu_choice;
	int submenu_choice = 0;

	while (true)
	{
		menu_choice = showMainMenu();

		switch (menu_choice)
		{
		case '1':
			//manageTabMenu();
			break;
		case '2':
			//manageListMenu();
			break;
		case '3':
			//manageHeapMenu();
			break;
		case '4':
			//manageTreeMenu();
			break;
		case 'q':
			submenu_choice = -1;
			break;
		default:
			cout << "Niewlasciwy wybor, wybierz ponownie.\n\n";
			submenu_choice = manageMenu();
		}

		if (submenu_choice == -1)
			break;
	}

	return submenu_choice;
}

char showMainMenu()
{
	cout << "Menu:\n"
		<< " 1 - tablica\n"
		<< " 2 - lista\n"
		<< " 3 - kopiec binarny\n"
		<< " 4 - drzewo czerwono-czarne\n"
		<< " q - wyjscie z programu\n\n";

	return _getch();
}