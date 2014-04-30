#include "stdafx.h"
#include "graf_mac.h"
#include "kopiec.h"
#include "krawedz_mac.h"
#include <cstdlib>
#include <ctime>

using namespace std;

MGraf::MGraf() :
macierz(NULL), wagi(NULL), N(0), M(0)
{ }

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

	macierz = NULL;
	wagi = NULL;
}

/*
	Funkcja zwraca true, jesli dany wierzcholek posiada juz wszystkie
	mozliwe krawedzie.
*/
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

/*
	Funkcja tworzaca losowy graf o zadanej gestosci.
*/
void MGraf::losujGraf(int n, float gestosc, bool ujemne_wagi)
{
	srand(time(NULL));

	if (macierz != NULL)
	{
		delete[] macierz;
	}
	
	if (wagi != NULL)
	{
		delete[] wagi;
	}

	/* ******budowa drzewa rozpinajacego*********** */

	N = n;	// liczba wierzcholkow
	M = n * (n - 1) * gestosc;	// liczba krawedzi
	int size = N * M;	// rozmiar tablic 'macierz' i 'wagi'
	int m = M; // liczba krawedzi pozostalych do wypelnienia

	macierz = new char[N * M];
	wagi = new int[M];

	// wypelnienie macierzy i wag zerami
	for (int i = 0; i < N*M; i++)
	{
		macierz[i] = 0;
	}

	for (int i = 0; i < M; i++)
	{
		wagi[i] = 0;
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

		wagi[j] = a;
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

			wagi[nr_m] = r;
		}
		else
		{
			macierz[i_begin] = 1;
			macierz[i_end] = -1;

			wagi[nr_m] = r;
		}
	}
}

/*
	Funkcja wyswietlajaca na ekranie graf w postaci macierzowej.
*/
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
	cout << "Tablica wag:";
	for (int i = 0; i < M; i++)
	{
		cout << wagi[i] << ", ";
	}
}

/*
	Funkcja dodaje do grafu nowa krawedz, na podstawie argumentow.
	Jesli dodanie powiodlo sie zwraca true, jesli nie, zwraca false.
*/
bool MGraf::dodajKraw(int start, int koniec, int waga)
{
	// jesli nie istnieje jeden z podanych wierzcholkow
	if (start >= N || start < 0 || koniec >= N || koniec < 0)
	{
		return false;
	}
	else
	{
		// zwiekszenie liczby krawedzi
		M++;

		char *macierz_temp = macierz;
		int *wagi_temp = wagi;

		macierz = new char[N*M];
		wagi = new int[N*M];

		// przepisanie istniejacych wartosci
		int diff = 0; // zawiera roznice w indeksach nowej i starej macierzy
		for (int i = 0; i < N*M; i++)
		{
			// jesli indeks nie wskazuje na nowa krawedz
			if (i % M != 0)
			{
				macierz[i] = macierz_temp[i-diff];
				wagi[i] = wagi_temp[i - diff];
			}
			// jesli indeks wskazuje na nowa krawedz
			else
			{
				// sprawdzenie czy aktualny wierzcholek jest jednym z argumentow
				int w = static_cast<int>(i / M);
				if (w == start)
				{
					macierz[i] = 1;
					wagi[i] = waga;
				}
				else if (w == koniec)
				{
					macierz[i] = -1;
					wagi[i] = waga;
				}
				else
				{
					macierz[i] = 0;
					wagi[i] = 0;
				}

				diff++;
			}
		}
	}

	return true;
}

/*
	Funkcja zwraca indeks wierzcholka poczatkowego krawedzi.
	Jesli funkcja zwroci -1, oznacza to, ze podana krawedz nie istnieje.
*/
int MGraf::getStart(int k)
{
	int start = -1;

	for (int i = 0; i < N; i++)
	{
		if (macierz[M*i + k] == 1)
		{
			start = i;
			break;
		}
	}

	return start;
}

/*
	Funkcja zwraca indeks wierzcholka koncowego krawedzi.
	Jesli funkcja zwroci -1, oznacza to, ze podana krawedz nie istnieje.
*/
int MGraf::getEnd(int k)
{
	int end = -1;

	for (int i = 0; i < N; i++)
	{
		if (macierz[M*i + k] == -1)
		{
			end = i;
			break;
		}
	}

	return end;
}

/*
	Funkcja zwraca wskaznik na zmienna int, zawierajaca wage danej krawedzi.
	Jesli zwroci NULL, oznacza to, ze podana krawedz nie istnieje.
*/
int* MGraf::getWeight(int k)
{
	int *weight = NULL;
	int index = -1;

	if (k < 0 || k >= M)
	{
		return NULL;
	}
	else
	{
		// znalezienie -1 lub 1 w macierzy incydencji
		// aby upewnic sie, ze pobrana zostanie rzeczywista waga
		for (int i = 0; i < N; i++)
		{
			if (macierz[i*M + k] == 1 || macierz[i*M + k] == -1)
			{
				index = i*M + k;
				break;
			}
		}

		weight = new int(wagi[index]);
	}

	return weight;
}

/*
	Funkcja dodaje nowy wierzcholek do grafu i zwraca numer(indeks) dodanego
	wierzcholka. np. dla wierzcholka nr 1 indeks wynosi 0
*/
int MGraf::dodajWierzch()
{
	// jesli nie ma krawedzi, zwieksza sie tylko liczba wierzcholkow
	if (M == 0)
	{
		N++;
	}
	// w przeciwnym przypadku konieczne jest przepisanie calej struktury
	else
	{
		char *macierz_temp = macierz;
		int *wagi_temp = wagi;

		macierz = new char[(N + 1)*M];
		wagi = new int[(N + 1)*M];

		for (int i = 0; i < N*M; i++)
		{
			macierz[i] = macierz_temp[i];
			wagi[i] = wagi_temp[i];
		}

		delete[] macierz_temp;
		delete[] wagi_temp;

		// wpisanie zer w nowym wierszu kazdej krawedzi
		for (int i = N*M; i < N*M + M; i++)
		{
			macierz[i] = wagi[i] = 0;
		}

		// zwiekszenie liczby wierzcholkow
		N++;
	}

	return N-1;
}

/*_____________minimalne drzewo rozpinajace___________*/

/*
	Algorytm traktuje krawedzie skierowane tak jakby byly
	krawedziami nieskierowanymi, dlatego kazda krawedz skierowana
	stanie sie krawedzia nieskierowana. Jesli nastapi konflikt
	wag, algorytm wybierze jadna z nich arbitralnie.
*/
MGraf* MGraf::mstPrime()
{
	MGraf *mst = new MGraf();

	// dodanie do drzewa mst wierzcholkow
	for (int i = 0; i < N; i++)
	{
		mst->dodajWierzch();
	}

	int n = N - 1; // liczba krawedzi, ktore nalezy wstawic, aby uzyskac spojne drzewo
	int w = 0; // ilosc wierzcholkow z ktorych wyszukujemy krawedzie

	Kopiec<MKrawedz> kp; // kolejka priorytetowa do przetrzymywania krawedzi
	kp.setMin();

	for (n; n > 0; n--)
	{
		//________________wybranie dostepnych krawedzi_________________________
		for (int i = 0; i <= w; i++)
		{
			// przeszukanie krawedzi wierzcholka 'w'
			for (int k = 0; k < M; k++)
			{
				if (macierz[i*M + k] == 1 || macierz[i*M + k] == -1)
				{
					MKrawedz kraw(wagi[i*M + k], k);
					if (!kp.find(kraw))
					{
						kp.push(kraw);
					}
				}
			}
		}

		//________________dodanie krawedzi o najmniejszej wadze__________________
		MKrawedz *k = kp.pop();
		mst->dodajKraw(getStart(k->nr_kraw), getEnd(k->nr_kraw), k->waga);
	}

	return mst;
}