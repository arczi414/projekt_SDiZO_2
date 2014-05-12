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
			manageMGrafMenu();
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

	int n;
	double gestosc;
	bool ujemneK, skierK;
	char temp;
	int n1, n2, waga;
	int N, M;

	char *str_plik;
	ifstream plik;
	LGraf* graf = new LGraf();
	Graf* tempG = NULL;

	int *koszty1 = NULL, **koszty2 = NULL, *poprz = NULL;

	while (true)
	{
		choice = showLGrafMenu();

		switch (choice)
		{
		case '1':
			cin.ignore(cin.rdbuf()->in_avail());
			if (graf != NULL)
			{
				delete graf;
				graf = NULL;
			}
			graf = new LGraf();
			str_plik = new char[30];
			cout << "Podaj nazwe pliku (razem z rozszerzeniem .txt): ";
			cin >> str_plik;
			plik.open(str_plik);
			if (plik)
			{
				plik.clear();
				plik.seekg(ios::beg);


				plik >> M >> N;	// pobranie liczby krawedzi i liczby wierzcholkow

				// utworzenie wierzcholkow
				for (int i = 0; i < N; i++)
				{
					graf->dodajWierzch();
				}

				// pobranie kolejnych krawedzi
				while (!plik.eof())
				{
					plik >> n1 >> n2 >> waga;
					graf->dodajKraw(n1, n2, waga);
				}

				plik.close();
				cout << "\nzaladowano dane do struktury\n\n";
			}
			else
			cout << "\nnie udalo sie wczytac danych z pliku (sprawdz czy plik istnieje)\n\n";

			delete str_plik;
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
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Wierzcholek poczatkowy: ";
			cin >> n1;
			cout << "Wierzcholek koncowy: ";
			cin >> n2;
			if (n1 >= graf->getNumOfVertices() || n1 < 0 || n2 >= graf->getNumOfVertices() || n2 < 0)
			{
				cout << "Podano nieprawidlowe dane.\n";
				continue;
			}
			cout << "Waga krawedzi: ";
			cin >> waga;
			graf->dodajKraw(n1, n2, waga);
			cout << "\n";
			break;
		case '5':
			tempG = graf->mstPrim(skierK);
			cout << "\n\nMinimalne drzewo rozpinajace: \n";
			tempG->pokazGraf();
			cout << "\n";
			if (tempG != NULL)
			{
				delete tempG;
				tempG = NULL;
			}
			break;
		case '6':
			tempG = graf->mstKruskal(skierK);
			cout << "\n\nMinimalne drzewo rozpinajace: \n";
			tempG->pokazGraf();
			cout << "\n";
			if (tempG != NULL)
			{
				delete tempG;
				tempG = NULL;
			}
			break;
		case '7':
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Wierzcholek startowy: ";
			cin >> n1;
			cout << "Wierzcholek koncowy: ";
			cin >> n2;
			graf->sptDijkstra(n1, koszty1, poprz);
			cout << "\n";

			cout << "\nDlugosc najkrotszej sciezki wynosi " << koszty1[n2];
			
			if (koszty1 != NULL)
			{
				delete[] koszty1;
				koszty1 = NULL;
			}
			
			if (poprz != NULL)
			{
				delete[] poprz;
				poprz = NULL;
			}

			cout << "\n\n";

			break;
		case '8':
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Wierzcholek startowy: ";
			cin >> n1;
			cout << "Wierzcholek koncowy: ";
			cin >> n2;
			graf->sptBellmanFord(n1, koszty2, poprz);
			cout << "\n";

			cout << "\nDlugosc najkrotszej sciezki wynosi " << *(koszty2[n2]);

			if (koszty2 != NULL)
			{
				for (int i = 0; i < graf->getNumOfVertices(); i++)
				{
					delete koszty2[i];
				}
				delete[] koszty2;
				koszty1 = NULL;
			}

			if (poprz != NULL)
			{
				delete[] poprz;
				poprz = NULL;
			}

			cout << "\n\n";
			break;
		case '9':
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Zrodlo: ";
			cin >> n1;
			cout << "Ujscie: ";
			cin >> n2;
			cout << "Wybierz sposob wyszukiwania sciezek rozszerzajacych (b - BFS, d - DFS)\n";
			temp = _getch();
			temp -= 32;
			if (!(temp == 'B' || temp == 'D'))
			{
				cout << "\nPodano nieprawidlowe dane.\n\n";
				continue;
			}
			cout << "\nMaksymalny przeplyw sieci wynosi " << graf->findMaxflowFordFulkerson(n1, n2, temp);
			cout << "\n\n";
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

char showMGrafMenu()
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

void manageMGrafMenu()
{
	char choice;

	int n;
	double gestosc;
	bool ujemneK, skierK;
	char temp;
	int n1, n2, waga;
	int N, M;

	char *str_plik;
	ifstream plik;
	MGraf* graf = new MGraf();
	Graf* tempG = NULL;

	int *koszty1 = NULL, **koszty2 = NULL, *poprz = NULL;

	while (true)
	{
		choice = showLGrafMenu();

		switch (choice)
		{
		case '1':
			cin.ignore(cin.rdbuf()->in_avail());
			if (graf != NULL)
			{
				delete graf;
				graf = NULL;
			}
			graf = new MGraf();
			str_plik = new char[30];
			cout << "Podaj nazwe pliku (razem z rozszerzeniem .txt): ";
			cin >> str_plik;
			plik.open(str_plik);
			if (plik)
			{
				plik.clear();
				plik.seekg(ios::beg);


				plik >> M >> N;	// pobranie liczby krawedzi i liczby wierzcholkow

				// utworzenie wierzcholkow
				for (int i = 0; i < N; i++)
				{
					graf->dodajWierzch();
				}

				// pobranie kolejnych krawedzi
				while (!plik.eof())
				{
					plik >> n1 >> n2 >> waga;
					graf->dodajKraw(n1, n2, waga);
				}

				plik.close();
				cout << "\nzaladowano dane do struktury\n\n";
			}
			else
				cout << "\nnie udalo sie wczytac danych z pliku (sprawdz czy plik istnieje)\n\n";

			delete str_plik;
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
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Wierzcholek poczatkowy: ";
			cin >> n1;
			cout << "Wierzcholek koncowy: ";
			cin >> n2;
			if (n1 >= graf->getNumOfVertices() || n1 < 0 || n2 >= graf->getNumOfVertices() || n2 < 0)
			{
				cout << "Podano nieprawidlowe dane.\n";
				continue;
			}
			cout << "Waga krawedzi: ";
			cin >> waga;
			graf->dodajKraw(n1, n2, waga);
			cout << "\n";
			break;
		case '5':
			tempG = graf->mstPrim(skierK);
			cout << "\n\nMinimalne drzewo rozpinajace: \n";
			tempG->pokazGraf();
			cout << "\n";
			if (tempG != NULL)
			{
				delete tempG;
				tempG = NULL;
			}
			break;
		case '6':
			tempG = graf->mstKruskal(skierK);
			cout << "\n\nMinimalne drzewo rozpinajace: \n";
			tempG->pokazGraf();
			cout << "\n";
			if (tempG != NULL)
			{
				delete tempG;
				tempG = NULL;
			}
			break;
		case '7':
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Wierzcholek startowy: ";
			cin >> n1;
			cout << "Wierzcholek koncowy: ";
			cin >> n2;
			graf->sptDijkstra(n1, koszty1, poprz);
			cout << "\n";

			cout << "\nDlugosc najkrotszej sciezki wynosi " << koszty1[n2];

			if (koszty1 != NULL)
			{
				delete[] koszty1;
				koszty1 = NULL;
			}

			if (poprz != NULL)
			{
				delete[] poprz;
				poprz = NULL;
			}

			cout << "\n\n";

			break;
		case '8':
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Wierzcholek startowy: ";
			cin >> n1;
			cout << "Wierzcholek koncowy: ";
			cin >> n2;
			graf->sptBellmanFord(n1, koszty2, poprz);
			cout << "\n";

			cout << "\nDlugosc najkrotszej sciezki wynosi " << *(koszty2[n2]);

			if (koszty2 != NULL)
			{
				for (int i = 0; i < graf->getNumOfVertices(); i++)
				{
					delete koszty2[i];
				}
				delete[] koszty2;
				koszty1 = NULL;
			}

			if (poprz != NULL)
			{
				delete[] poprz;
				poprz = NULL;
			}

			cout << "\n\n";
			break;
		case '9':
			cin.ignore(cin.rdbuf()->in_avail());
			cout << "Zrodlo: ";
			cin >> n1;
			cout << "Ujscie: ";
			cin >> n2;
			cout << "Wybierz sposob wyszukiwania sciezek rozszerzajacych (b - BFS, d - DFS)\n";
			temp = _getch();
			temp -= 32;
			if (!(temp == 'B' || temp == 'D'))
			{
				cout << "\nPodano nieprawidlowe dane.\n\n";
				continue;
			}
			cout << "\nMaksymalny przeplyw sieci wynosi " << graf->findMaxflowFordFulkerson(n1, n2, temp);
			cout << "\n\n";
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