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
	if (macierz != NULL)
	{
		delete[] macierz;
	}

	if (wagi != NULL)
	{
		delete[] wagi;
	}
}

bool MGraf::hasMax(int n)
{
	int n_kraw = 0; // liczba powiazanych krawedzi

	for (int i = n * M; i < n * M + M; i++)
	{
		if (macierz[i] == 1 || macierz[i] == -1)
		{
			n_kraw++;
		}
	}

	if (n_kraw == 2 * (N - 1))
	{
		return true;
	}

	return false;
}

/*
	Jeœli dwa wierzcholki lacza sie w kierunku n1->n2 funkcja zwraca 1,
	jesli n2->n1 funkcja zwraca -1, jesli posiada obydwa polaczenia 2.
	Zwraca 0, gdy brak polaczen.
*/
char MGraf::hasConnection(int n1, int n2)
{
	int n1n2 = 0;
	int n2n1 = 0;

	for (int i = 0; i < M; i++)
	{
		if (macierz[n1 * M + i] == 1 && macierz[n2 * M + i] == -1)
		{
			n1n2 = 1;
		}

		if (macierz[n1 * M + i] == -1 && macierz[n2 * M + i] == 1)
		{
			n2n1 = 1;
		}

		if (n1n2 == 1 && n2n1 == 1)
			return 2;
	}

	if (n1n2 == 1)
		return 1;
	return -1;
}

void MGraf::losujGraf(int n, float gestosc, bool ujemne_wagi)
{
	srand(time(NULL));

	if (macierz == NULL)
	{
		delete[] macierz;
	}
	
	if (wagi == NULL)
	{
		delete[] wagi;
	}

	/* ******budowa drzewa rozpinajacego*********** */

	N = n;	// liczba wierzcholkow
	M = n * (n - 1) * gestosc;	// liczba krawedzi
	int size = N * M;	// rozmiar tablic 'macierz' i 'wagi'
	int m = M; // liczba krawedzi pozostalych do wypelnienia

	macierz = new char[N * M];
	wagi = new int[N * M];

	// wypelnienie macierzy i wag zerami
	for (int i = 0; i < N*M; i++)
	{
		macierz[i] = wagi[i] = 0;
	}

	// i - wierzcholek, j - krawedz
	for (int i = 0, j = 0; i < N - 1; i++, j++, m--)
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

	/* ***********dodanie pozostalych krawedzi************* */
	int nr_m = N - 1; // numer aktualnie dodawanej krawedzi
	for (m, nr_m; m > 0; m--, nr_m++)
	{
		// losowanie wierzcholka startowego
		int begin = rand() % N;

		while (hasMax(begin))
		{
			begin = (begin + 1) % N;
		}

		// losowanie wierzcholka koncowego
		int end = rand() % N;

		char connection = 0;

		while (end == begin || hasMax(end) || (connection = hasConnection(begin, end)) == 2)
		{
			end = (end + 1) % N;
		}

		// wylosowanie wagi
		int r = rand();

		if (r % 2 == 0) r = -1 * rand();
		else r = rand();

		const int i_begin = begin * M + nr_m;
		const int i_end = end * M + nr_m;

		if (connection == 1)
		{
			macierz[i_begin] = -1;
			macierz[i_end] = 1;

			wagi[i_begin] = r;
			wagi[i_end] = r;
		}
		else
		{
			macierz[i_begin] = 1;
			macierz[i_end] = -1;

			wagi[i_begin] = r;
			wagi[i_end] = r;
		}
	}
}

void MGraf::pokazGraf()
{
	int size = N * M;

	// wypisanie zawartosci macierzy incydencji
	cout << "Macierz incydencji:";
	for (int i = 0; i < size; i++)
	{
		if (i % M == 0)
		{
			cout << "\n" << static_cast<int>(macierz[i]) << "\t";
		}
		else
		{
			cout << static_cast<int>(macierz[i]) << "\t";
		}
	}

	cout << "\n\n";

	// wypisanie zawartosci macierzy wag
	cout << "Macierz wag:";
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