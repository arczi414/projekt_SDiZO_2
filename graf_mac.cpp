#include "stdafx.h"
#include "graf_mac.h"
#include "kopiec.h"
#include "krawedz_mac.h"
#include "drzewoCzerCzar.h"
#include "zbiory_rozlaczne.h"
#include <cstdlib>
#include <iomanip>
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
	Je�li dwa wierzcholki lacza sie w kierunku n1->n2 funkcja zwraca 1,
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
	wag, algorytm wybierze mniejsza.

	Jesli podwojne_kraw ustawione sa jako true, funkcja przed
	wykonaniem algorymtu prima pousuwa z grafu podwojne krawedzie
	pomiedzy wierzcholkami.
*/
MGraf* MGraf::mstPrim(bool podwojne_kraw)
{
	if (podwojne_kraw)
	{
		usunPodwojne();
	}

	MGraf *mst = new MGraf();
	DrzewoCzerCzar dodaneKraw; // zawiera numery dodanych krawedzi
	DrzewoCzerCzar dodaneWierzch; // zawiera numery dodanych wierzcholkow

	// dodanie do drzewa mst wierzcholkow
	for (int i = 0; i < N; i++)
	{
		mst->dodajWierzch();
	}

	Kopiec<MKrawedz> kp; // kolejka priorytetowa do przetrzymywania krawedzi
	kp.setMin(); 

	int poprz = 0; // indeks poprzednio dodanego wierzcholka do grafu

	dodaneWierzch.insert(poprz);

	cout << "Trwa wyszukiwanie MST (algorytm Prima):";
	cout << setw(5) << " ";

	for (int i = 0; i < N-1; i++)
	{
		// pokazuje postep algorytmu
		int wart = (static_cast<float>(i) / (N - 1)) * 100;
		cout << "\b\b\b\b\b" << setw(4) << wart << "%";

		// przeszukanie dostepnych krawedzi wierzcholka 'poprz'
		int *dostepne = getAvailableEdges(poprz);
		if (dostepne != NULL)
		{
			for (int k = 1; k <= dostepne[0]; k++)
			{
				int nr_k = dostepne[k];
				MKrawedz kraw(wagi[nr_k], nr_k);
				if (dodaneKraw.find(nr_k) == NULL)
				{
					kp.push(kraw);
				}
			}
			delete[] dostepne;
		}

		// dodanie krawedzi o najmniejszej wadze prowadzacej do jeszcze
		// nie dodanego wierzcholka
		MKrawedz *kr = kp.pop();

		int start = getStart(kr->nr_kraw);
		int end = getEnd(kr->nr_kraw);

		while (dodaneWierzch.find(start) && dodaneWierzch.find(end))
		{
			kr = kp.pop();
			start = getStart(kr->nr_kraw);
			end = getEnd(kr->nr_kraw);
		}
		
		mst->dodajKraw(start, end, kr->waga);
		dodaneKraw.insert(kr->nr_kraw);

		poprz = (start == poprz ? end : start); // nowo dodany wierzcholek

		dodaneWierzch.insert(poprz);
		
		delete kr;
	}

	// pokazuje postep algorytmu
	cout << "\b\b\b\b\b" << setw(4) << 100 << "%";

	return mst;
}

/*
	Algorytm traktuje krawedzie skierowane tak jakby byly
	krawedziami nieskierowanymi, dlatego kazda krawedz skierowana
	stanie sie krawedzia nieskierowana. Jesli nastapi konflikt
	wag, algorytm wybierze mniejsza.

	Jesli podwojne_kraw ustawione sa jako true, funkcja przed
	wykonaniem algorymtu prima pousuwa z grafu podwojne krawedzie
	pomiedzy wierzcholkami.
*/
MGraf* MGraf::mstKruskal(bool podwojne_kraw)
{
	if (podwojne_kraw)
	{
		usunPodwojne();
	}

	MGraf *mst = new MGraf();

	// dodanie do drzewa mst wierzcholkow
	for (int i = 0; i < N; i++)
	{
		mst->dodajWierzch();
	}

	// kolejka przechowujaca krawedzie
	Kopiec<MKrawedz> krawedzie;
	krawedzie.setMin();

	// zbiory rozlaczne do przechowywania dodanych wierzcholkow
	ZbioryRozlaczne wierzcholki(N);

	// dodanie wszystkich krawedzi do kopca
	for (int i = 0; i < M; i++)
	{
		int *w = getWeight(i);
		MKrawedz k(*w, i);
		delete w;
		krawedzie.push(k);
	}

	// wstawienie krawedzi
	int wstawione = 0;
	MKrawedz *mk;

	cout << "Trwa wyszukiwanie MST (algorytm Kruskala):";
	cout << setw(5) << " ";

	while (wstawione < N - 1)
	{
		// pokazuje postep algorytmu
		int wart = (static_cast<float>(wstawione) / (N - 1)) * 100;
		cout << "\b\b\b\b\b" << setw(4) << wart << "%";

		mk = krawedzie.pop();

		int start = getStart(mk->nr_kraw);
		int end = getEnd(mk->nr_kraw);

		// sprawdzenie czy laczone wierzcholki znajduja sie w rozlacznych zbiorach
		if (wierzcholki.FindSet(start) != wierzcholki.FindSet(end))
		{
			int *w = getWeight(mk->nr_kraw);
			mst->dodajKraw(start, end, *w);
			delete w;
			wierzcholki.UnionSets(start, end);

			wstawione++;
		}
	}

	// pokazuje postep algorytmu
	cout << "\b\b\b\b\b" << setw(4) << 100 << "%";

	return mst;
}

/*_____________wyszukiwanie najkrotszej sciezki_______*/

int* MGraf::sptDijkstra(int w)
{
	// tablica z informacja o koszcie dojscia z poszczegolnych wierzcholkow
	int* koszty = new int[N];

	// tablica poprzednikow
	int *poprz = new int[N];

	// kopiec zawierajacy dostepne wierzcholki
	Kopiec<MWierzcholek> dostepne;
	dostepne.setMin();

	// tablica przechowujaca informacje o tym, czy dany wierzcholek zostal juz
	// przeniesiony do zbioru S (obliczono dla niego koszt przejscia)
	// jesli SQ[i] jest rowne false oznacza to, ze 'w' nalezy do zbioru Q
	// jesli SQ[i] wynosi true to 'w' znajduje sie w S
	bool *SQ = new bool[N];

	// poczatkowo wszystkie wierzcholki naleza do Q
	// a wszystkie koszty i nr poprzednikow wynosza -1
	for (int i = 0; i < N; i++)
	{
		SQ[i] = false;
		koszty[i] = -1;
		poprz[i] = -1;
	}

	// dodanie 'w' do zbioru S
	koszty[w] = 0;
	SQ[w] = true;

	// wyszukanie wierzcholkow dostepnych wierzcholkow z 'w'
	MWierzcholek *mozliweW = getAvailableVertices(w);

	// jesli zaden wierzcholek nie jest dostepny z 'w' to nie istnieje droga
	// do zadnego innego wierzcholka
	if (mozliweW != NULL)
	{
		for (int i = 1; i <= mozliweW[0].koszt_dojscia; i++)
		{
			int akt_w = mozliweW[i].nr_wierzch; // aktualnie sprawdzany wierzcholek

			// wierzcholek zostanie dodany do dostepnych, pod warunkiem, ze nalezy do zbioru Q
			// oraz ze koszt dojscia do niego przewyzsza aktualny koszt lub wierzcholek nie zostal
			// jeszcze dodany
			if (!SQ[akt_w] && (koszty[akt_w] > (koszty[akt_w] + mozliweW[i].koszt_dojscia) || koszty[akt_w] == -1))
			{
				koszty[akt_w] = koszty[akt_w] == -1 ? mozliweW[i].koszt_dojscia : koszty[akt_w] + mozliweW[i].koszt_dojscia;
				poprz[akt_w] = w;
				dostepne.push(mozliweW[i]);
			}
		}

		if (mozliweW != NULL)
		{
			delete[] mozliweW;
			mozliweW = NULL;
		}

		cout << "Trwa wyszukiwanie najkrotszych sciezek (algorytm Dijkstry):";
		cout << setw(5) << " ";

		// wlasciwy algorytm
		MWierzcholek *w_main;
		for (int j = 1; j < N; j++)
		{
			// pokazuje postep algorytmu
			int wart = (static_cast<float>(j) / (N - 1)) * 100;
			cout << "\b\b\b\b\b" << setw(4) << wart << "%";

			w_main = dostepne.pop();
			
			if (w_main != NULL)
			{
				SQ[w_main->nr_wierzch] = true;
				mozliweW = getAvailableVertices(w_main->nr_wierzch);

				if (mozliweW != NULL)
				{
					for (int i = 1; i <= mozliweW[0].koszt_dojscia; i++)
					{
						int akt_w = mozliweW[i].nr_wierzch; // aktualnie sprawdzany wierzcholek

						if (!SQ[akt_w] && (koszty[akt_w] > (koszty[akt_w] + mozliweW[i].koszt_dojscia) || koszty[akt_w] == -1))
						{
							koszty[akt_w] = koszty[akt_w] == -1 ? mozliweW[i].koszt_dojscia : koszty[akt_w] + mozliweW[i].koszt_dojscia;
							poprz[akt_w] = w;
							dostepne.push(mozliweW[i]);
						}
					}
				}
			}

			if (mozliweW != NULL)
			{
				delete[] mozliweW;
				mozliweW = NULL;
			}

			if (w_main != NULL)
			{
				delete w_main;
				w_main = NULL;
			}
		}
	}

	// pokazuje postep algorytmu
	cout << "\b\b\b\b\b" << setw(4) << 100 << "%\n";

	return koszty;
}