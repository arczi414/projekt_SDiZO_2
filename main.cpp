#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"
#include "krawedz_mac.h"
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

	graf.dodajKraw(0, 1, 31886);
	graf.dodajKraw(1, 2, 11224);
	graf.dodajKraw(2, 3, 9910);
	graf.dodajKraw(0, 2, 27361);
	graf.dodajKraw(0, 3, 22559);
	graf.dodajKraw(1, 3, 18810);

	MGraf *mst;

	mst = graf.mstKruskal();
	cout << "\n\n";
	mst->pokazGraf();

	cout << "\n\n";
	mst = graf.mstPrim();
	cout << "\n\n";
	mst->pokazGraf();

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

