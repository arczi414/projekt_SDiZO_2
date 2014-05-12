#include "stdafx.h"
#include "menu.h"
#include "graf.h"
#include "graf_list.h"
#include "graf_mac.h"
#include <fstream>

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
			manageLGrafMenu();
			break;
		case '2':
			//manageMGraf();
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
		<< " 1 - graf - listy sasiedztwa\n"
		<< " 2 - graf - macierz incydencji\n"
		<< " q - wyjscie z programu\n\n";

	return _getch();
}

char showLGrafMenu()
{
	cout << "Menu struktury 'tablica':\n"
		<< " 1 - wczytaj graf z pliku\n"
		<< " 2 - losuj graf\n"
		<< " 3 - dodaj wierzcholek\n"
		<< " 4 - dodaj krawedz\n"
		<< " 5 - algorytm Prima\n"
		<< " 6 - algorytm Kruskala\n"
		<< " 7 - algorytm Dijkstry\n"
		<< " 8 - algorytm Bellmana-Forda\n"
		<< " 9 - algorytm Forda-Fulkersona\n"
		<< " p - pokaz graf\n"
		<< " q - wyjscie do menu\n\n";

	return _getch();
}

void manageLGrafMenu()
{
	char choice;

	int n, gestosc;
	bool ujemneK, skierK;
	char temp;

	char *str_plik;
	ifstream plik;
	LGraf* graf = new LGraf();

	while (true)
	{
		choice = showLGrafMenu();

		switch (choice)
		{
		case '1':
			/*str_plik = new char[30];
			cout << "Podaj nazwe pliku (razem z rozszerzeniem .txt): ";
			cin >> str_plik;
			plik.open(str_plik);
			if (plik)
			{
			plik.clear();
			plik.seekg(ios::beg);


			plik >> data;	//pobranie rozmiaru struktury (pominiecie - nie potrzebne)

			while (!plik.eof())
			{
			plik >> data;
			tablica->add(0, data);
			}

			plik.close();
			cout << "\nzaladowano dane do struktury\n\n";
			}
			else
			cout << "\nnie udalo sie wczytac danych z pliku (sprawdz czy plik istnieje)\n\n";

			delete str_plik;*/
			break;
		case '2':
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "liczba wierzcholkow: "; cin >> n;
			cout << "gestosc (0-1): "; cin >> gestosc;
			cout << "czy graf jest skierowany? (t - tak, n - nie)\n";
			temp = _getch();
			if (temp == 't') skierK = true;
			else skierK = false;
			cout << "czy graf jest zawiera ujemne wagi? (t - tak, n - nie)\n";
			temp = _getch();
			if (!(temp = 't' || temp != 'n'))
			{
				cout << "\nPodano nieprawidlowe dane.\n\n";
				continue;
			}
			if (temp == 't') ujemneK = true;
			else ujemneK = false;
			if (gestosc < 0 || gestosc > 1 || !(temp = 't' || temp != 'n'))
			{
				cout << "Podano nieprawidlowe dane.\n";
				continue;
			}

			cout << "\n\n";
			graf->losujGraf(n, gestosc, ujemneK, skierK);

			cout << "\n\n";

			break;
		case '3':
			graf->dodajWierzch();
			cout << "\nDodano wierzcholek do grafu.\n\n";
			break;
		case '4':
			break;
		case '5':
			break;
		case '6':
			break;
		case '7':
			break;
		case '8':
			break;
		case '9':
			break;
		case 'p':
			graf->pokazGraf();
			cout << "\n\n";
			break;
		default:;
		}

		if (choice == 'q')
			break;
	}

	if (graf != NULL)
	{
		delete graf;
	}
}