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

	listy = NULL;

	if (listy != NULL)
	{
		graf->listy = new List<SasWierzcholek>[N];

		for (int i = 0; i < N; i++)
		{
			graf->listy[i] = listy[i];
		}
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
		m--;
	}

	/* *************dodanie pozostalych krawedzi************* */
	int nr_m = N - 1; // nr aktualnie dodawanej krawedzi
	int m_pomiar = m, nr_wysw = 1; // potrzebne do wyswietlania postepu
	std::cout << "Trwa losowanie grafu:";
	std::cout << setw(5) << " ";
	for (m, nr_m; m > 0; m--, nr_m++)
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
		}
		else
		{
			SasWierzcholek insW(end, r, nr_m);
			listy[begin].insert(insW, insW, true);
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
		SasWierzcholek w(koniec, waga, M++);
		listy[start].insert(w, w, true);
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
		for (int i = 0; i < N; i++)
		{
			SasWierzcholek *w = NULL;
			listy[i].reset(1);
			while (w = listy[i].next())
			{
				if (w->nr_kraw == k)
				{

				}
			}
		}
	}
}

int LGraf::dodajWierzch()
{
	return 0;
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

int LGraf::getIndexOfEdge(int start, int end)
{
	return 0;
}

int LGraf::getStart(int k)
{
	return 0;
}

int LGraf::getEnd(int k)
{
	return 0;
}

int* LGraf::getAvailableEdges(int w, bool skierowany)
{
	return NULL;
}

MWierzcholek* LGraf::getAvailableVertices(int w, bool skierowany)
{
	return NULL;
}

int* LGraf::getWeight(int k)
{
	return NULL;
}

bool LGraf::setWeight(int k, int waga)
{
	return false;
}