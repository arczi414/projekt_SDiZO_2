#include "stdafx.h"
#include "graf_mac.h"
#include "kopiec.h"
#include "krawedz_mac.h"
#include "list_int.h"
#include "lista.h"
#include "drzewoCzerCzar.h"
#include "zbiory_rozlaczne.h"
#include <cstdlib>
#include <iomanip>
#include <ctime>

using namespace std;

MGraf::MGraf() :
macierz(NULL), wagi(NULL), N(0), M(0)
{ }

MGraf::MGraf(const MGraf& mg)
{
	N = mg.N;
	M = mg.M;

	macierz = NULL;
	wagi = NULL;

	if (mg.macierz != NULL)
	{
		if (macierz != NULL) { delete[] macierz; }

		macierz = new char[N*M];

		for (int i = 0; i < N*M; i++)
		{
			macierz[i] = mg.macierz[i];
		}
	}

	if (mg.wagi != NULL)
	{
		if (wagi != NULL) { delete[] wagi; }

		wagi = new int[N*M];

		for (int i = 0; i < M; i++)
		{
			wagi[i] = mg.wagi[i];
		}
	}
}

MGraf* MGraf::Clone()
{
	MGraf *nowy_graf = new MGraf();

	nowy_graf->N = N;
	nowy_graf->M = M;
	
	nowy_graf->macierz = new char[N*M];
	nowy_graf->wagi = new int[M];

	for (int i = 0; i < N*M; i++)
	{
		nowy_graf->macierz[i] = macierz[i];
	}

	for (int i = 0; i < M; i++)
	{
		nowy_graf->wagi[i] = wagi[i];
	}

	return nowy_graf;
}

MGraf* MGraf::Create()
{
	MGraf *nowy_graf = new MGraf();
	return nowy_graf;
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

	macierz = NULL;
	wagi = NULL;
}

MGraf& MGraf::operator = (const MGraf &mg)
{
	if (&mg == this)
	{
		return *this;
	}
	else
	{
		if (macierz != NULL) { delete[] macierz; macierz = NULL; }
		if (wagi != NULL) { delete[] wagi; wagi = NULL; }

		N = mg.N;
		M = mg.M;

		if (mg.macierz != NULL)
		{
			for (int i = 0; i < N*M; i++)
			{
				macierz[i] = mg.macierz[i];
			}
		}

		if (mg.wagi != NULL)
		{
			for (int i = 0; i < M; i++)
			{
				wagi[i] = mg.wagi[i];
			}
		}
	}
}

/*
	Funkcja zwraca true, jesli dany wierzcholek posiada juz wszystkie
	mozliwe krawedzie.
	Wynik funkcji zalezy rowniez od tego czy rozpatrywany jest graf
	o podwojnych krawedziach(skierowany) czy mozliwe sa jedynie pojedyncze krawedzie.
	UWAGA! Funkcja sprawdza tylko ilosc krawedzi dla wierzcholka, a nie czy te krawedzie
	sa dopuszczalne.
*/
bool MGraf::hasMax(int n, bool podwojne_kraw)
{
	int n_kraw = 0; // liczba powiazanych krawedzi

	for (int i = n * M; i < n * M + M; i++)
	{
		if (macierz[i] == 1 || macierz[i] == -1)
		{
			n_kraw++;
		}
	}

	if (n_kraw == 2 * (N - 1) && podwojne_kraw)
	{
		return true;
	}

	if (n_kraw == N - 1 && !podwojne_kraw)
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
	else if(n2n1 == 1)
		return -1;

	return 0;
}

/*
	Funkcja tworzaca losowy graf o zadanej gestosci.
*/
void MGraf::losujGraf(int n, float gestosc, bool ujemne_wagi, bool podwojne_kraw)
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
	M = n * (n - 1);	// liczba krawedzi
	if (!podwojne_kraw) M /= 2;
	M *= gestosc;
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

		wagi[j] = ujemne_wagi ? a : abs(a);
	}

	/* ***********dodanie pozostalych krawedzi************* */
	int nr_m = N - 1; // numer aktualnie dodawanej krawedzi
	int m_pomiar = m, nr_wysw = 1; // potrzebne do wyswietlania postepu
	cout << "Trwa losowanie grafu:";
	cout << setw(5) << " ";
	for (m, nr_m; m > 0; m--, nr_m++)
	{
		// pokazuje postep losowania grafu
		if (((1 - (static_cast<double>(m) / m_pomiar)) * 100) > nr_wysw)
		{
			int wart = 100 - (static_cast<double>(m) / m_pomiar) * 100;
			cout << "\b\b\b\b\b" << setw(4) << wart << "%";
			nr_wysw++;
		}

		// losowanie wierzcholka startowego
		int begin = rand() % N;

		while (hasMax(begin, podwojne_kraw))
		{
			begin = (begin + 1) % N;
		}

		// losowanie wierzcholka koncowego
		int end = rand() % N;

		char connection = 0;

		// w zaleznosci od tego czy maja byc losowane podwojne krawedzie
		if (podwojne_kraw)
		{
			while (end == begin || (connection = hasConnection(begin, end)) == 2)
			{
				end = (end + 1) % N;
			}
		}
		else
		{
			connection = hasConnection(begin, end);
			while (end == begin || connection == 1 || connection == -1)
			{
				end = (end + 1) % N;
				connection = hasConnection(begin, end);
			}
		}

		int r = rand();

		if (r % 2 == 0) r = -1 * rand();
		else r = rand();

		const int i_begin = begin * M + nr_m;
		const int i_end = end * M + nr_m;

		if (connection == 1)
		{
			macierz[i_begin] = -1;
			macierz[i_end] = 1;

			wagi[nr_m] = ujemne_wagi ? r : abs(r);
		}
		else
		{
			macierz[i_begin] = 1;
			macierz[i_end] = -1;

			wagi[nr_m] = ujemne_wagi ? r : abs(r);
		}
	}

	cout << "\b\b\b\b\b" << setw(4) << 100 << "%";
	cout << endl;
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

		delete[] wagi_temp;

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

		delete[] macierz_temp;
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
	Funkcja zwraca indeks krawedzi, ktora laczy wierzcholek o indeksie 'start'
	z wierzcholkiem o indeksie 'end'. Jesli krawedz nie istnieje, funkcja zwraca
	-1;
*/
int MGraf::getIndexOfEdge(int start, int end)
{
	// przeszukanie wierzcholka 'start'
	for (int i = 0; i < M; i++)
	{
		if (macierz[start*M + i] == 1)
		{
			// sprawdzenie czy znaleziona krawedz konczy sie w 'end'
			if (macierz[end*M + i] == -1)
			{
				return i;
			}
		}
	}

	return -1;
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
	Funkcja ustawia wage podanej krawedzi. Jesli krawedz nie istnieje zwraca
	false. Jesli istnieje - true.
*/
bool MGraf::setWeight(int k, int waga)
{
	if (k >= 0 && k < M)
	{
		wagi[k] = waga;
		return true;
	}

	return false;
}

/*
	Funkcja zwraca w tablicy dostepne krawedzie z danego wierzcholka.
	Jesli 'skierowany' wynosi true, za dostepne, zostana uznane jedynie
	krawedzie wychodzace, jesli wynosi false, wszystkie krawedzie zwiazane
	z wierzcholkiem 'w'.
	Funkcja zwraca tablice int gdzie element o indeksie 0 informuje o liczbie
	dostepnych krawedzi.
*/
int* MGraf::getAvailableEdges(int w, bool skierowany)
{
	int *dostepne = new int[1];
	int *temp_dostepne = NULL;
	dostepne[0] = 0;

	// przeszukanie wierzcholka
	for (int i = 0; i < M; i++)
	{
		if (skierowany)
		{
			if (macierz[w*M + i] == 1)
			{
				temp_dostepne = dostepne;
				
				// liczba dostepnych krawedzi w tablicy
				int n_dostepne = temp_dostepne[0];

				dostepne = new int[n_dostepne + 2];

				// przepisanie poprzednich wartosci tablicy
				for (int j = 0; j <= n_dostepne; j++)
				{
					dostepne[j] = temp_dostepne[j];
				}

				dostepne[0]++;
				dostepne[ dostepne[0] ] = i;

				if (temp_dostepne != NULL)
				{
					if (n_dostepne == 0)
					{
						delete temp_dostepne;
					}
					else
					{
						delete[] temp_dostepne;
					}

					temp_dostepne = NULL;
				}
			}
		}
		else
		{
			if (macierz[w*M + i] == 1 || macierz[w*M + i] == -1)
			{
				temp_dostepne = dostepne;

				// liczba dostepnych krawedzi w tablicy
				int n_dostepne = temp_dostepne[0];

				dostepne = new int[n_dostepne + 2];

				// przepisanie poprzednich wartosci tablicy
				for (int j = 0; j <= n_dostepne; j++)
				{
					dostepne[j] = temp_dostepne[j];
				}

				dostepne[0]++;
				dostepne[dostepne[0]] = i;

				if (temp_dostepne != NULL)
				{
					if (n_dostepne == 0)
					{
						delete temp_dostepne;
					}
					else
					{
						delete[] temp_dostepne;
					}

					temp_dostepne = NULL;
				}
			}
		}
	}

	return dostepne;
}

/*
	Funkcja zwraca w tablicy dostepne wierzcholki z danego wierzcholka.
	Jesli 'skierowany' wynosi true, za dostepne, zostana uznane jedynie
	krawedzie wychodzace, jesli wynosi false, wszystkie krawedzie zwiazane
	z wierzcholkiem 'w'.
	Funkcja zwraca tablice int gdzie koszt_dojscia elementu o indeksie 0 informuje o liczbie
	dostepnych wierzcholkow.
*/
MWierzcholek* MGraf::getAvailableVertices(int w, bool skierowany)
{
	MWierzcholek *dostepne = new MWierzcholek[1];
	MWierzcholek *temp_dostepne = NULL;
	dostepne[0] = MWierzcholek(0, -1, -1);

	for (int i = 0; i < M; i++)
	{
		if (skierowany)
		{
			if (macierz[w*M + i] == 1)
			{
				for (int k = 0; k < N; k++)
				{
					if (macierz[k*M + i] == -1)
					{
						int *waga_kraw = getWeight(i);

						MWierzcholek a(*waga_kraw, k, w);
						temp_dostepne = dostepne;

						if (waga_kraw != NULL) { delete waga_kraw; waga_kraw = NULL; }

						dostepne = new MWierzcholek[temp_dostepne[0].koszt_dojscia + 2];

						// przepisanie poprzednich dostepnych
						for (int j = 0; j <= temp_dostepne[0].koszt_dojscia; j++)
						{
							dostepne[j] = temp_dostepne[j];
						}

						// zwiekszenie liczby dostepnych
						dostepne[0].koszt_dojscia++;

						// dopisanie nowego
						dostepne[dostepne[0].koszt_dojscia] = a;

						if (temp_dostepne != NULL)
						{
							delete[] temp_dostepne;
							temp_dostepne = NULL;
						}

						break;
					}
				}
			}
		}
		else
		{
			if (macierz[w*M + i] == 1 || macierz[w*M + i] == -1)
			{
				for (int k = 0; k < N; k++)
				{
					if ((macierz[k*M + i] == -1 || macierz[w*M + i] == 1) && k != w)
					{
						MWierzcholek a(*getWeight(i), k, w);
						temp_dostepne = dostepne;

						dostepne = new MWierzcholek[temp_dostepne[0].koszt_dojscia + 2];

						// przepisanie poprzednich dostepnych
						for (int j = 0; j <= temp_dostepne[0].koszt_dojscia; j++)
						{
							dostepne[j] = temp_dostepne[j];
						}

						// zwiekszenie liczby dostepnych
						dostepne[0].koszt_dojscia++;

						// dopisanie nowego
						dostepne[dostepne[0].koszt_dojscia] = a;

						if (temp_dostepne != NULL)
						{
							delete[] temp_dostepne;
							temp_dostepne = NULL;
						}

						break;
					}
				}
			}
		}
	}

	return dostepne;
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
	Funkcja usuwa zadana krawedz z grafu. Jesli operacja sie powiedzie zwraca true,
	jesli nie (np. krawedz nie istnieje) zwraca false.
*/
bool MGraf::usunKraw(int k)
{
	if (k < 0 || k >= M)
	{
		return false;
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

		return true;
	}
}