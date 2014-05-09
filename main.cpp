#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"
#include "graf_list.h"
#include "krawedz_mac.h"
#include "wierzcholek_mac.h"
#include "zbiory_rozlaczne.h"
#include "drzewoCzerCzar.h"
#include "lista.h"
#include "kopiec.h"
#include "list_int.h"
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vld.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//manageMenu();

	srand(time(NULL));

	Graf *g = new LGraf();
	
	g->dodajWierzch();
	g->dodajWierzch();
	g->dodajWierzch();
	g->dodajWierzch();
	g->dodajWierzch();

	g->dodajKraw(0, 1, 10);
	g->dodajKraw(0, 4, 421);
	g->dodajKraw(0, 2, 12);
	g->dodajKraw(4, 3, 422);
	g->dodajKraw(1, 2, 12);
	g->dodajKraw(3, 2, 5523);
	g->dodajKraw(4, 2, 42);
	g->dodajKraw(3, 1, 12);
	g->dodajKraw(2, 0, 3);

	g->pokazGraf();
	cout << "\n\n";

	g->usunKraw(0);

	g->pokazGraf();

	delete g;

	//Graf *graf = new LGraf();
	//for (int i = 0; i < 1000; i++)
	//graf->losujGraf(100, 0.5);
	//graf->pokazGraf();
	//graf->findMaxflowFordFulkerson(0, 30);

	//graf->usunPodwojne();

	//delete graf;
	
	/*
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();

	graf.dodajKraw(0, 1, 9);
	graf.dodajKraw(0, 2, 9);
	graf.dodajKraw(1, 4, 7);
	graf.dodajKraw(2, 5, 6);
	graf.dodajKraw(2, 3, 3);
	graf.dodajKraw(1, 3, 3);
	graf.dodajKraw(4, 3, 4);
	graf.dodajKraw(3, 5, 2);
	graf.dodajKraw(4, 6, 6);
	graf.dodajKraw(3, 6, 9);
	graf.dodajKraw(5, 6, 8);

	MGraf *g = new MGraf();

	cout << graf.findMaxflowFordFulkerson(0, 6, 'D', g);

	cout << "\n\n";
	g->pokazGraf();
	delete g;
	*/

	cout << "\n\nWcisnij dowolny klawisz, aby zakonczyc program...";
	_getch();
	return 0;
}

