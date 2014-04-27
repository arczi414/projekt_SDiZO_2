#include "stdafx.h"
#include "graf_mac.h"
#include <cstdlib>
#include <ctime>

using namespace std;

MGraf::MGraf()
{

}

MGraf::~MGraf()
{

}

void MGraf::losujGraf(int n, float gestosc, bool ujemne_wagi)
{
	srand(time(NULL));

	delete[] macierz;
	delete[] wagi;

	/* ******budowa drzewa rozpinajacego*********** */

	N = n;	// liczba wierzcholkow
	M = n * (n - 1) * gestosc;	// liczba krawedzi
	int m = M; // liczba krawedzi pozostalych do wypelnienia

	macierz = new char[N * M];
	wagi = new int[N * M];

	// wypelnienie macierzy i wag zerami
	for (int i = 0; i < N*M; i++)
	{
		macierz[i] = wagi[i] = 0;
	}

	// i - wierzcholek, j - krawedz
	for (int i = 0, j = 0; i < N - 1; i++, j++)
	{
		int a = rand();

		if (a % 2 == 0) a = 1;
		else a = -1;

		const int begin = i * M + j;
		const int end = (i + 1) * M + j;

		macierz[begin] = a;
		macierz[end] = -a;

		// +/- rand
		a = a * rand();

		wagi[begin] = a;
		wagi[end] = a;
	}
}

void MGraf::pokazGraf()
{
	int size = N * M;

	// wypisanie zawartosci macierzy incydencji
	cout << "Macierz incydencji: \n";
	for (int i = 0; i < size; i++)
	{
		if (i % M == 0)
		{
			cout << "\n" << macierz[i] << "\t";
		}
		else
		{
			cout << macierz[i] << "\t";
		}
	}

	cout << "\n\n";

	// wypisanie zawartosci macierzy wag
	cout << "Macierz wag: \n";
	for (int i = 0; i < size; i++)
	{
		if (i % M == 0)
		{
			cout << "\n" << wagi[i] << "\t";
		}
		else
		{
			cout << wagi[i] << "\t";
		}
	}
}