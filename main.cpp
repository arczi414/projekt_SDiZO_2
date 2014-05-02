#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"
#include "krawedz_mac.h"
#include "wierzcholek_mac.h"
#include "zbiory_rozlaczne.h"
#include "lista.h"
#include "kopiec.h"
#include <iomanip>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//manageMenu();

	MGraf graf;

	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();
	graf.dodajWierzch();

	graf.dodajKraw(0, 1, 1);
	graf.dodajKraw(1, 2, 1);
	graf.dodajKraw(0, 5, 10);
	graf.dodajKraw(1, 4, 3);
	graf.dodajKraw(2, 3, 1);
	graf.dodajKraw(3, 4, 1);
	graf.dodajKraw(5, 6, 1);
	graf.dodajKraw(4, 6, 8);
	graf.dodajKraw(4, 7, 1);
	graf.dodajKraw(6, 7, 2);
	graf.dodajKraw(3, 8, 10);

	graf.pokazGraf();
	cout << "\n\n";

	int *wagi = graf.sptDijkstra(0);

	for (int i = 0; i < graf.getNumOfVertices(); i++)
	{
		cout << wagi[i] << ", ";
	}

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

