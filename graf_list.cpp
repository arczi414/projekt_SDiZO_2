#include "stdafx.h"
#include "graf_list.h"
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

LGraf::LGraf() :
listy(NULL), N(0), M(0)
{}

LGraf::LGraf(const LGraf& lg)
{
	N = lg.N;
	M = lg.M;

	nr_krawedzi = lg.nr_krawedzi;

	listy = NULL;

	if (lg.listy != NULL)
	{
		listy = new List<SasWierzcholek>[N];

		for (int i = 0; i < N; i++)
		{
			listy[i] = lg.listy[i];
		}
	}
}

LGraf* LGraf::Clone()
{
	LGraf *graf = new LGraf();

	graf->N = N;
	graf->M = M;
	graf->nr_krawedzi = nr_krawedzi;

	if (listy != NULL)
	{
		graf->listy = new List<SasWierzcholek>[N];

		for (int i = 0; i < N; i++)
		{
			graf->listy[i] = listy[i];
		}
	}
	else
	{
		graf->listy = NULL;
	}

	return graf;
}

LGraf* LGraf::Create()
{
	return new LGraf();
}

LGraf::~LGraf()
{
	if (listy != NULL)
	{
		delete[] listy;
	}

	listy = NULL;
}

LGraf& LGraf::operator =(const LGraf &lg)
{
	if (&lg == this)
	{
		return *this;
	}
	else
	{
		if (listy != NULL) { delete[] listy; listy = NULL; }

		N = lg.N;
		M = lg.M;
		nr_krawedzi = lg.nr_krawedzi;

		if (lg.listy != NULL)
		{
			for (int i = 0; i < N*M; i++)
			{
				listy[i] = lg.listy[i];
			}
		}
	}
}

bool LGraf::hasMax(int n, bool podwojne_kraw)
{
	// jesli wierzcholek posiada wszystkie mozliwe krawedzie
	// to suma numerow wierzcholkow z ktorymi sie laczy powinna byc
	// rowna sumie numerow wszystkich wierzcholkow
	// aby nie pominac zerowego wierzcholka zakladam, ze kazdy wierzcholek ma
	// numer o jeden wiekszy
	int sumaNrW = (1 + N)*N / 2;
	sumaNrW -= n + 1;

	if (podwojne_kraw && listy[n].getSize() == N - 1)
	{
		return true;
	}
	// brak podwójnych krawedzi
	else
	{
		SasWierzcholek *tempW = NULL;
		// zliczenie nr_wierzcholkow w n
		listy[n].reset(1);
		while (tempW = listy[n].next())
		{
			sumaNrW -= tempW->nr_wierzch + 1;
		}

		// przeszukanie pozostalych wierzcholkow
		for (int i = 0; i < N; i++)
		{
			if (i == n) { continue; }
			listy[i].reset(1);
			while (tempW = listy[i].next())
			{
				if (tempW->nr_wierzch == n)
				{
					sumaNrW -= i + 1;
				}
			}
		}

		if (sumaNrW == 0) { return true; }
	}

	return false;
}

/*
	Jeœli dwa wierzcholki lacza sie w kierunku n1->n2 funkcja zwraca 1,
	jesli n2->n1 funkcja zwraca -1, jesli posiada obydwa polaczenia 2.
	Zwraca 0, gdy brak polaczen.
*/
char LGraf::hasConnection(int n1, int n2)
{
	int n1n2 = 0;
	int n2n1 = 0;

	SasWierzcholek *akt_w = NULL;

	// przeszukanie wierzcholka n1
	listy[n1].reset(1);
	while (akt_w = listy[n1].next())
	{
		if (akt_w->nr_wierzch == n2)
		{
			n1n2 = 1;
			break;
		}
	}

	akt_w = NULL;

	// przeszukanie wierzcholka n2
	listy[n2].reset(1);
	while (akt_w = listy[n2].next())
	{
		if (akt_w->nr_wierzch == n1)
		{
			n2n1 = 1;
			break;
		}
	}

	if (n1n2 == 1 && n2n1 == 1)
		return 2;
	else if (n1n2 == 1)
		return 1;
	else if (n2n1 == 1)
		return -1;

	return 0;
}

void LGraf::losujGraf(int n, float gestosc, bool ujemne_wagi, bool podwojne_kraw)
{
	srand(time(NULL));

	if (listy != NULL)
	{
		delete[] listy;
		listy = NULL;
	}

	nr_krawedzi.makenull();

	/* ******budowa drzewa rozpinajacego*********** */

	N = n;	// liczba wierzcholkow
	M = n * (n - 1);	// liczba krawedzi
	if (!podwojne_kraw) M /= 2;
	M *= gestosc;
	M = (M > n - 1 ? M : n - 1); // zabezpieczenie, aby zawsze utworzyc przynajmniej graf spojny
	int m = M; // liczba krawedzi pozostalych do wypelnienia

	listy = new List<SasWierzcholek>[N];

	for (int i = 0; i < N - 1; i++)
	{
		int waga = rand();
		
		if (ujemne_wagi)
		{
			int znak = rand() % 2 == 0 ? -1 : 1;
			waga *= znak;
		}

		SasWierzcholek nowy(i + 1, waga, i);
		listy[i].insert(nowy, nowy, true);
		nr_krawedzi.insert(i, i);
		m--;
	}

	/* *************dodanie pozostalych krawedzi************* */
	int m_pomiar = m, nr_wysw = 1; // potrzebne do wyswietlania postepu
	int nr_m = N - 1;
	std::cout << "Trwa losowanie grafu:";
	std::cout << setw(5) << " ";
	for (m; m > 0; m--, nr_m++)
	{
		// pokazuje postep losowania grafu
		if (((1 - (static_cast<double>(m) / m_pomiar)) * 100) > nr_wysw)
		{
			int wart = 100 - (static_cast<double>(m) / m_pomiar) * 100;
			std::cout << "\b\b\b\b\b" << setw(4) << wart << "%";
			nr_wysw++;
		}

		int begin = rand() % N; // losowanie wierzcholka startowego

		// upewnienie sie ze do wierzcholka mozna dodac krawedz
		while (hasMax(begin, podwojne_kraw))
		{
			begin = (begin + 1) % N;
		}

		int end = rand() % N; // losowanie wierzcholka koncowego

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

		int zn = rand() % 2;
		int r = zn == 0 ? rand()*(-1) : rand();
		r = ujemne_wagi ? r : abs(r);

		if (connection == 1)
		{
			SasWierzcholek insW(begin, r, nr_m);
			listy[end].insert(insW, insW, true);
			nr_krawedzi.insert(nr_m, end);
		}
		else
		{
			SasWierzcholek insW(end, r, nr_m);
			listy[begin].insert(insW, insW, true);
			nr_krawedzi.insert(nr_m, begin);
		}
	}

	std::cout << "\b\b\b\b\b" << setw(4) << 100 << "%";
	std::cout << endl;
}

void LGraf::pokazGraf()
{
	std::cout << "Lista sasiedztwa:\n";
	for (int i = 0; i < N; i++)
	{
		stringstream ind;
		ind << "[" << i << "]:";
		std::cout << left << setw(8) << ind.str();

		SasWierzcholek *sas;
		listy[i].reset(1);
		while (sas = listy[i].next())
		{
			stringstream sasiad;
			sasiad << sas->nr_wierzch << "(" << sas->waga_kraw << ")";
			std::cout << left << setw(15) << sasiad.str();
		}

		std::cout << endl;
	}
}

bool LGraf::dodajKraw(int start, int koniec, int waga)
{
	// jesli nie istnieje jeden z podanych wierzcholkow
	if (start >= N || start < 0 || koniec >= N || koniec < 0)
	{
		return false;
	}
	// sprawdzenie czy dana krawedz juz nie istnieje
	else if (znajdzKrawedz(start, koniec))
	{
		return false;
	}
	else
	{
		SasWierzcholek w(koniec, waga, M);
		listy[start].insert(w, w, true);
		nr_krawedzi.insert(M, start);
		M++;
	}

	return true;
}

bool LGraf::usunKraw(int k)
{
	if (k < 0 || k >= M)
	{
		return false;
	}
	else
	{
		// wyszukanie podanej krawedzi
		Node *szukana = nr_krawedzi.find(k);
		// przeszukanie powiazanego wierzcholka
		listy[szukana->data2].reset(1);
		SasWierzcholek *sas = NULL;
		while (sas = listy[szukana->data2].next())
		{
			if (sas->nr_kraw == k)
			{
				listy[szukana->data2].remove(*sas);
				nr_krawedzi.remove(k);
				M--;
				break;
			}
		}

		// zmniejszenie numeracji krawedzi dla wszystkich powyzej k
		for (int i = k + 1; i <= M; i++)
		{
			Node *node = nr_krawedzi.find(i);
			node->data--;
			listy[node->data2].reset(1);
			SasWierzcholek *sW = NULL;
			while (sW = listy[node->data2].next())
			{
				if (sW->nr_kraw == i)
				{
					sW->nr_kraw--;
					break;
				}
			}
		}

		return true;
	}

	return false;
}

int LGraf::dodajWierzch()
{
	N++; // zwiekszenie liczby wierzcholkow

	List<SasWierzcholek> *temp_listy = listy;
	listy = new List<SasWierzcholek>[N];

	// przepisanie poprzedniej zawartosci
	for (int i = 0; i < N - 1; i++)
	{
		listy[i] = temp_listy[i];
	}

	if (temp_listy != NULL)
	{
		delete[] temp_listy;
		temp_listy = NULL;
	}

	return N-1;
}

/*
	Funkcja sprawdza czy krawedz zaczynajaca sie w wierzcholku 'start' i konczaca w 'end'
	juz istnieje w grafie. Jesli tak zwraca true, jesli nie, zwraca false.
*/
bool LGraf::znajdzKrawedz(int start, int end)
{
	SasWierzcholek *w = NULL;
	listy[start].reset(1);
	while (w = listy[start].next())
	{
		if (w->nr_wierzch == end)
		{
			return true;
		}
	}

	return false;
}

/*
	Funkcja zwraca indeks krawedzi, ktora laczy wierzcholek o indeksie 'start'
	z wierzcholkiem o indeksie 'end'. Jesli krawedz nie istnieje, funkcja zwraca
	-1
*/
int LGraf::getIndexOfEdge(int start, int end)
{
	SasWierzcholek *w = NULL;
	listy[start].reset(1);
	while (w = listy[start].next())
	{
		if (w->nr_wierzch == end)
		{
			return w->nr_kraw;
		}
	}

	return -1;
}

/*
	Funkcja zwraca indeks wierzcholka poczatkowego krawedzi.
	Jesli funkcja zwroci -1, oznacza to, ze podana krawedz nie istnieje.
*/
int LGraf::getStart(int k)
{
	if (k >= M || k < 0)
	{
		return -1;
	}
	
	Node *node = nr_krawedzi.find(k);

	return node->data2;
}

/*
	Funkcja zwraca indeks wierzcholka koncowego krawedzi.
	Jesli funkcja zwroci -1, oznacza to, ze podana krawedz nie istnieje.
*/
int LGraf::getEnd(int k)
{
	if (k >= M || k < 0)
	{
		return -1;
	}

	Node *node = nr_krawedzi.find(k);
	SasWierzcholek *sW = NULL;
	listy[node->data2].reset(1);
	while (sW = listy[node->data2].next())
	{
		if (sW->nr_kraw == k)
		{
			return sW->nr_wierzch;
		}
	}

	return -1;
}

/*
	Funkcja zwraca w tablicy dostepne krawedzie z danego wierzcholka.
	Jesli 'skierowany' wynosi true, za dostepne, zostana uznane jedynie
	krawedzie wychodzace, jesli wynosi false, wszystkie krawedzie zwiazane
	z wierzcholkiem 'w'.
	Funkcja zwraca tablice int gdzie element o indeksie 0 informuje o liczbie
	dostepnych krawedzi.
*/
int* LGraf::getAvailableEdges(int w, bool skierowany)
{
	int *dostepne = new int[listy[w].getSize() + 1];
	int *temp_dostepne = NULL;
	dostepne[0] = listy[w].getSize();

	// dodanie krawedzi, ktorych wierzcholkiem startowym jest w
	SasWierzcholek *sW = NULL;
	listy[w].reset(1);
	for (int i = 1; sW = listy[w].next(); i++)
	{
		dostepne[i] = sW->nr_kraw;
	}

	// dodanie pozostalych wierzcholkow w zaleznosci od 'skierowany'
	if (!skierowany)
	{
		// przeszukanie pozostalych wierzcholkow
		for (int i = 0; i < N; i++)
		{
			if (i == w)
			{
				continue;
			}

			SasWierzcholek *sW = NULL;
			listy[i].reset(1);
			while (sW = listy[i].next())
			{
				// jesli znaleziono polaczenie
				if (sW->nr_wierzch == w)
				{
					dostepne[0]++;
					temp_dostepne = dostepne;
					dostepne = new int[temp_dostepne[0] + 1];

					// przepisanie poprzedniej zawartosci
					for (int j = 0; j < temp_dostepne[0]; j++)
					{
						dostepne[j] = temp_dostepne[j];
					}

					delete[] temp_dostepne;

					dostepne[dostepne[0]] = sW->nr_kraw;
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
MWierzcholek* LGraf::getAvailableVertices(int w, bool skierowany)
{
	MWierzcholek *dostepne = new MWierzcholek[listy[w].getSize() + 1];
	MWierzcholek *temp_dostepne = NULL;
	dostepne[0] = MWierzcholek(listy[w].getSize(), -1, -1);

	// dodanie wierzcholkow, ktorych wierzcholkiem startowym jest w
	SasWierzcholek *sW = NULL;
	listy[w].reset(1);
	for (int i = 1; sW = listy[w].next(); i++)
	{
		dostepne[i] = MWierzcholek(sW->waga_kraw, sW->nr_wierzch, w);
	}

	// dodanie pozostalych wierzcholkow w zaleznosci od 'skierowany'
	if (!skierowany)
	{
		// przeszukanie pozostalych wierzcholkow
		for (int i = 0; i < N; i++)
		{
			if (i == w)
			{
				continue;
			}

			SasWierzcholek *sW = NULL;
			listy[i].reset(1);
			while (sW = listy[i].next())
			{
				// jesli znaleziono polaczenie
				if (sW->nr_wierzch == w)
				{
					dostepne[0].koszt_dojscia++;
					temp_dostepne = dostepne;
					dostepne = new MWierzcholek[temp_dostepne[0].koszt_dojscia + 1];

					// przepisanie poprzedniej zawartosci
					for (int j = 0; j < temp_dostepne[0].koszt_dojscia; j++)
					{
						dostepne[j] = temp_dostepne[j];
					}

					delete[] temp_dostepne;

					dostepne[dostepne[0].koszt_dojscia] = MWierzcholek(sW->waga_kraw, i, w);
				}
			}
		}
	}

	return dostepne;
}

/*
	Funkcja zwraca wskaznik na zmienna int, zawierajaca wage danej krawedzi.
	Jesli zwroci NULL, oznacza to, ze podana krawedz nie istnieje.
*/
int* LGraf::getWeight(int k)
{
	if (k >= M || k < 0)
	{
		return NULL;
	}

	Node *node = nr_krawedzi.find(k);
	SasWierzcholek *sW = NULL;
	listy[node->data2].reset(1);
	while (sW = listy[node->data2].next())
	{
		if (sW->nr_kraw == k)
		{
			int* waga = new int(sW->waga_kraw);
			return waga;
		}
	}

	return NULL;
}

/*
	Funkcja ustawia wage podanej krawedzi. Jesli krawedz nie istnieje zwraca
	false. Jesli istnieje - true.
*/
bool LGraf::setWeight(int k, int waga)
{
	if (k >= M || k < 0)
	{
		return false;
	}

	Node *node = nr_krawedzi.find(k);
	SasWierzcholek *sW = NULL;
	listy[node->data2].reset(1);
	while (sW = listy[node->data2].next())
	{
		if (sW->nr_kraw == k)
		{
			sW->waga_kraw = waga;
			return true;
		}
	}

	return false;
}