#include "stdafx.h"
#include "graf_mac.h"
#include "kopiec.h"
#include "krawedz_mac.h"
#include "drzewoCzerCzar.h"
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
	M = (M > n - 1 ? M : n - 1); // zabezpieczenie, aby zawsze utworzyc przynajmniej graf spojny
	int size = N * M;	// rozmiar macierzy 'macierz'
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
	if (M > 0)
	{
		cout << "Tablica wag:";
		cout << "\n" << wagi[0];
		for (int i = 1; i < M; i++)
		{
			cout << ", " << wagi[i];
		}
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
	// sprawdzenie czy dana krawedz juz nie istnieje
	else if ( znajdzKrawedz(start, koniec))
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
		wagi = new int[M];

		// przepisanie starej tablicy wag
		for (int i = 0; i < M - 1; i++)
		{
			wagi[i] = wagi_temp[i];
		}

		// dodanie wagi nowej krawedzi
		wagi[M - 1] = waga;

		// przepisanie istniejacych wartosci
		int diff = 0; // zawiera roznice w indeksach nowej i starej macierzy
		for (int i = 0; i < N*M; i++)
		{
			// jesli indeks 'i' nie wskazuje na nowa krawedz
			if ((i+1) % M != 0)
			{
				macierz[i] = macierz_temp[i-diff];
			}
			// jesli indeks wskazuje na nowa krawedz
			else
			{
				// sprawdzenie czy aktualny wierzcholek jest jednym z argumentow
				int w = static_cast<int>(i / M);
				if (w == start)
				{
					macierz[i] = 1;
				}
				else if (w == koniec)
				{
					macierz[i] = -1;
				}
				else
				{
					macierz[i] = 0;
				}

				diff++;
			}
		}
	}

	return true;
}

/*
	Funkcja sprawdza czy krawedz zaczynajaca sie w wierzcholku 'start' i konczaca w 'end'
	juz istnieje w grafie. Jesli tak zwraca true, jesli nie, zwraca false.
*/
bool MGraf::znajdzKrawedz(int start, int end)
{
	// przeszukanie wierzcholka 'start'
	for (int i = 0; i < M; i++)
	{
		if (macierz[start*M + i] == 1)
		{
			// sprawdzenie czy znaleziona krawedz konczy sie w 'end'
			if (macierz[end*M + i] == -1)
			{
				return true;
			}
		}
	}

	return false;
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
		weight = new int(wagi[k]);
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

		macierz = new char[(N + 1)*M];

		for (int i = 0; i < N*M; i++)
		{
			macierz[i] = macierz_temp[i];
		}

		delete[] macierz_temp;

		// wpisanie zer w nowym wierszu kazdej krawedzi
		for (int i = N*M; i < N*M + M; i++)
		{
			macierz[i] = 0;
		}

		// zwiekszenie liczby wierzcholkow
		N++;
	}

	return N-1;
}

/*
	Funkcja usuwa podwojne krawedzie w grafie. Jesli znajdzie dwa wierzcholki miedzy
	ktorymi znajduja sie dwie krawedzie pozostawia te o mniejszej wadze.
*/
void MGraf::usunPodwojne()
{
	int start_a, start_s;
	int koniec_a, koniec_s;

	for (int k = 0; k < M; k++)
	{
		start_a = getStart(k);
		koniec_a = getEnd(k);

		for (int i = 0; i < M; i++)
		{
			if (i == k) continue;
			else
			{
				start_s = getStart(i);
				koniec_s = getEnd(i);

				if (
					(start_s == start_a && koniec_s == koniec_a) ||
					(start_s == koniec_a && koniec_s == start_a)
					)
				{
					int w_s = wagi[i];
					int w_a = wagi[k];

					// usuniecie krawedzi o wiekszej wadze
					if (w_s > w_a)
					{
						usunKraw(i);
					}
					else
					{
						usunKraw(k);

						// w przypadku usuniecia bierzacej krawedzi
						// konieczne jest przesuniecie wskaznika, aby nie pominac
						// kolejnej krawedzi
						k--;
					}

					// nie moga wystapic trzy krawedzie pomiedzy dwoma wierzcholkami
					// dlatego nie ma sensu porownywac reszte krawedzi
					break;
				}
			}
		}
	}
}

/*
	Funkcja usuwa zadana krawedz z grafu. Jesli operacja sie powiedzie zwraca true,
	jesli nie (np. krawedz nie istnieje) zwraca false.
*/
bool MGraf::usunKraw(int k)
{
	if (k < 0 || k >= M)
	{
		return NULL;
	}
	else
	{
		char *temp_macierz = macierz;
		int *temp_wagi = wagi;

		M--;

		macierz = new char[N*M];
		wagi = new int[M];

		// przepisanie macierzy
		for (int i = 0; i < N; i++)
		{
			int kr = 0;

			for (kr; kr < k; kr++)
			{
				macierz[i*M + kr] = temp_macierz[i*(M + 1) + kr];
			}

			for (kr; kr < M; kr++)
			{
				macierz[i*M + kr] = temp_macierz[i*(M + 1) + kr + 1];
			}
		}

		// przepisanie wag
		int i = 0;
		for (i; i < k; i++)
		{
			wagi[i] = temp_wagi[i];
		}

		for (i; i < M; i++)
		{
			wagi[i] = temp_wagi[i+1];
		}

		delete[] temp_macierz;
		delete[] temp_wagi;
	}
}

/*_____________minimalne drzewo rozpinajace___________*/

/*
	Algorytm traktuje krawedzie skierowane tak jakby byly
	krawedziami nieskierowanymi, dlatego kazda krawedz skierowana
	stanie sie krawedzia nieskierowana. Jesli nastapi konflikt
	wag, algorytm wybierze jadna z nich arbitralnie.
*/
/*
MGraf* MGraf::mstPrime()
{
	MGraf *mst = new MGraf();
	DrzewoCzerCzar wierzch;

	// dodanie do drzewa mst wierzcholkow
	for (int i = 0; i < N; i++)
	{
		mst->dodajWierzch();
	}

	Kopiec<MKrawedz> kp; // kolejka priorytetowa do przetrzymywania krawedzi
	kp.setMin(); 

	int poprz = 0; // indeks poprzednio dodanego wierzcholka do grafu

	//________________wybranie dostepnych krawedzi_________________________
	for (int i = 0; i < N; i++)
	{
		// przeszukanie krawedzi wierzcholka 'poprz'
		for (int k = 0; k < M; k++) // k - nr krawedzi
		{
			if (macierz[poprz*M + k] == 1 || macierz[poprz*M + k] == -1)
			{
				MKrawedz kraw(wagi[k], k);
				if (!kp.find(kraw) && )
				{
					kp.push(kraw);
				}
			}
		}

		// dodanie krawedzi o najmniejszej wadze
		MKrawedz *kr = kp.pop();

		int start = getStart(kr->nr_kraw);
		int end = getEnd(kr->nr_kraw);

		while (wierzch.find(start) || wierzch.find(end))
		{
			kr = kp.pop();
			start = getStart(kr->nr_kraw);
			end = getEnd(kr->nr_kraw);
		}
		
		mst->dodajKraw(getStart(kr->nr_kraw), getEnd(kr->nr_kraw), kr->waga);
		
		// dodanie nowego wierzcholka
		wierzch.insert(poprz);

		poprz = (start == poprz ? end : start); // nowo dodany wierzcholek
		
		delete kr;
	}

	return mst;
}
*/