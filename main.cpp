#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"
#include "graf_list.h"
#include "krawedz_mac.h"
#include "wierzcholek_mac.h"
#include "zbiory_rozlaczne.h"
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

	Graf *graf = new LGraf();
	for (int i = 0; i < 1000; i++)
	graf->losujGraf(100, 0.5);
	//graf->pokazGraf();
	//graf->findMaxflowFordFulkerson(0, 30);

	//graf->usunPodwojne();

	delete graf;
	
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







	// TESTY JEDNOSTKOWE

	/*
	TEST JEDNOSTKOWY - usunPodwojne()
	for (int t = 0; t < 100000; t++)
	{
	graf->losujGraf(4, 1);
	graf->usunPodwojne();

	for (int k = 0; k < graf->getNumOfEdges(); k++)
	{
	int s_a = graf->getStart(k);
	int k_a = graf->getEnd(k);

	for (int i = 0; i < graf->getNumOfEdges(); i++)
	{
	int s_s = graf->getStart(i);
	int k_s = graf->getEnd(i);

	if (i != k)
	{
	if ((s_a == s_s && k_a == k_s) || (s_a == k_s && k_a == s_s))
	{
	cout << "Znaleziono podwojna krawedz w " << t << " tescie.\n";
	}
	}
	}
	}
	}
	*/


	cout << "\n\nWcisnij dowolny klawisz, aby zakonczyc program...";
	_getch();
	return 0;
}

