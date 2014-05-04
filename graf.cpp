#include "stdafx.h"
#include "graf.h"
#include "kopiec.h"
#include "krawedz_mac.h"
#include "lista.h"
#include "drzewoCzerCzar.h"
#include "zbiory_rozlaczne.h"
#include <iomanip>

using namespace std;

/*_____________wyszukiwanie istniejacych sciezek_______*/

/*
Algorytm wyszukuje dowolna istniejaca sciezke w residualnym grafie 'rsGraph'
i zwraca jej przebieg w tablicy int (kolejne wierzcholki na sciezce).
Zerowy element zwracanej tablicy informuje o dlugosci sciezki.
Jesli funkcja zwraca NULL oznacza to, ze nie znaleziono zadnej sciezki.
*/
int* Graf::findAugPathDFS(int start, int end, Graf* const rsGraph)
{
	// init

	// tablica informujaca o tym z jakiego wierzcholka trafiono do bierzacego
	// jesli NULL oznacza to, ze wierzcholka jeszcze nie sprawdzano
	int *poprz = new int[rsGraph->getNumOfVertices()];

	List<MWierzcholek> kolejka; // kolejka do przechowywania dostepnych wierzcholkow
	MWierzcholek* sasiedziW = NULL; // tablica do przechowywania dostepnych wierzcholkow z bierzacego

	MWierzcholek *w = new MWierzcholek(start);

	for (int i = 0; i < getNumOfVertices(); i++)
	{
		poprz[i] = -1;
	}

	// dodanie startowego wierzcholka do kolejki
	kolejka.insert(*w, *w, true);
	poprz[w->nr_wierzch] = w->nr_wierzch;

	if (w != NULL) { delete w; w = NULL; }

	// przeszukanie wszystkich wierzcholkow
	for (int i = 0; i < getNumOfVertices(); i++)
	{
		w = kolejka.popFirst();

		// zakonczenie przeszukiwania jesli w kolejce nie ma wiecej wierzcholkow
		if (w == NULL) { break; }
		else
		{
			sasiedziW = rsGraph->getAvailableVertices(w->nr_wierzch);

			int n_sasiadow = sasiedziW[0].koszt_dojscia;

			// wstawienie niesprawdzonych sasiadow do kolejki
			for (int j = 1; j <= n_sasiadow; j++)
			{
				int nr_kraw = rsGraph->getIndexOfEdge(w->nr_wierzch, sasiedziW[j].nr_wierzch);
				int *waga_kraw = rsGraph->getWeight(nr_kraw);
				if (poprz[sasiedziW[j].nr_wierzch] == -1 && *waga_kraw > 0)
				{
					kolejka.insert(sasiedziW[j], sasiedziW[j], true);
					poprz[sasiedziW[j].nr_wierzch] = w->nr_wierzch;
				}

				if (waga_kraw != NULL) { delete waga_kraw; waga_kraw = NULL; }
			}

			if (sasiedziW != NULL) { delete[] sasiedziW; sasiedziW = NULL; }
			if (w != NULL) { delete w; w = NULL; }

			// jesli osiagnieto wierzcholek 'end' mozna zakonczyc
			if (poprz[end] != -1) { break; }
		}
	}

	if (w != NULL) { delete w; w = NULL; }

	// analiza znalezionych poprzednikow 'end' i stworzenie 'sciezki'
	int *sciezka = NULL; // znaleziona sciezka (max dlugosc N)

	// jesli znaleziono sciezke do 'end'
	if (poprz[end] != -1)
	{
		int length = 0; // dlugosc sciezki (liczba wierzcholkow posrednich)

		int p = end;
		while (p != start)
		{
			p = poprz[p];
			length++;
		}

		// uwzglednienie pierwszego wierzcholka
		length++;

		sciezka = new int[length + 1];

		sciezka[0] = length; // pierwszy element zawiera dlugosc sciezki (ilosc elementow - 1)
		sciezka[length] = end; // ostatnim elementem sciezki jest 'end'

		// wpisanie do 'sciezka' kolejnych wierzcholkow
		p = end;
		for (int i = length - 1; i >= 1; i--)
		{
			p = poprz[p];
			sciezka[i] = p;
		}
	}

	// sprzatanie
	if (poprz != NULL) { delete[] poprz; poprz = NULL; }

	return sciezka;
}

/*
Algorytm posiada niemal identyczna implementacje jak dla DFS, jedyna roznica
jest sposob pobierania elementow z kolejki. W tym przypadku jest ona kolejka
FIFO, w przeciwienstwie do LIFO w DFS.

Algorytm wyszukuje dowolna istniejaca sciezke w residualnym grafie 'rsGraph'
i zwraca jej przebieg w tablicy int (kolejne wierzcholki na sciezce).
Zerowy element zwracanej tablicy informuje o dlugosci sciezki.
Jesli funkcja zwraca NULL oznacza to, ze nie znaleziono zadnej sciezki.
*/
int* Graf::findAugPathBFS(int start, int end, Graf* const rsGraph)
{
	// init

	// tablica informujaca o tym z jakiego wierzcholka trafiono do bierzacego
	// jesli NULL oznacza to, ze wierzcholka jeszcze nie sprawdzano
	int *poprz = new int[rsGraph->getNumOfVertices()];

	List<MWierzcholek> kolejka; // kolejka do przechowywania dostepnych wierzcholkow
	MWierzcholek* sasiedziW = NULL; // tablica do przechowywania dostepnych wierzcholkow z bierzacego

	MWierzcholek *w = new MWierzcholek(start);

	for (int i = 0; i < getNumOfVertices(); i++)
	{
		poprz[i] = -1;
	}

	// dodanie startowego wierzcholka do kolejki
	kolejka.insert(*w, *w, true);
	poprz[w->nr_wierzch] = w->nr_wierzch;

	if (w != NULL) { delete w; w = NULL; }

	// przeszukanie wszystkich wierzcholkow
	for (int i = 0; i < getNumOfVertices(); i++)
	{
		w = kolejka.popLast();

		// zakonczenie przeszukiwania jesli w kolejce nie ma wiecej wierzcholkow
		if (w == NULL) { break; }
		else
		{
			sasiedziW = rsGraph->getAvailableVertices(w->nr_wierzch);

			int n_sasiadow = sasiedziW[0].koszt_dojscia;

			// wstawienie niesprawdzonych sasiadow do kolejki
			for (int j = 1; j <= n_sasiadow; j++)
			{
				int nr_kraw = rsGraph->getIndexOfEdge(w->nr_wierzch, sasiedziW[j].nr_wierzch);
				int *waga_kraw = rsGraph->getWeight(nr_kraw);
				if (poprz[sasiedziW[j].nr_wierzch] == -1 && *waga_kraw > 0)
				{
					kolejka.insert(sasiedziW[j], sasiedziW[j], true);
					poprz[sasiedziW[j].nr_wierzch] = w->nr_wierzch;
				}

				if (waga_kraw != NULL) { delete waga_kraw; waga_kraw = NULL; }
			}

			if (sasiedziW != NULL) { delete[] sasiedziW; sasiedziW = NULL; }
			if (w != NULL) { delete w; w = NULL; }

			// jesli osiagnieto wierzcholek 'end' mozna zakonczyc
			if (poprz[end] != -1) { break; }
		}
	}

	if (w != NULL) { delete w; w = NULL; }

	// analiza znalezionych poprzednikow 'end' i stworzenie 'sciezki'
	int *sciezka = NULL; // znaleziona sciezka (max dlugosc N)

	// jesli znaleziono sciezke do 'end'
	if (poprz[end] != -1)
	{
		int length = 0; // dlugosc sciezki (liczba wierzcholkow posrednich)

		int p = end;
		while (p != start)
		{
			p = poprz[p];
			length++;
		}

		// uwzglednienie pierwszego wierzcholka
		length++;

		sciezka = new int[length + 1];

		sciezka[0] = length; // pierwszy element zawiera dlugosc sciezki (ilosc elementow - 1)
		sciezka[length] = end; // ostatnim elementem sciezki jest 'end'

		// wpisanie do 'sciezka' kolejnych wierzcholkow
		p = end;
		for (int i = length - 1; i >= 1; i--)
		{
			p = poprz[p];
			sciezka[i] = p;
		}
	}

	// sprzatanie
	if (poprz != NULL) { delete[] poprz; poprz = NULL; }

	return sciezka;
}




/*_____________wyszukiwanie najwiekszego przeplywu_______*/

/*
Algorytm Forda-Fulkersona znajdowania maksymalnego przeplywu w sieci przeplywowej.
Funkcja zwraca maksymalny przeplyw sieci. Wagi grafu sa interpretowane jako przepustowosc
residualna.

path_finding -	sposob wyszukiwania sciezek (B - BFS, D - DFS)
rsGraph -	jesli zostanie podany jako parametr zostana tam zapisane
przeplywy na poszczegolnych krawedziach
*/
int Graf::findMaxflowFordFulkerson(int source, int sink, char path_finding, Graf* flowGraph)
{
	// init
	Graf *residualGraph = this->Clone();
	int maxFlow = 0;
	bool flowGraph_return = false; // informuje o tym czy funkcja ma zwolnic pamiec, czy zajmie sie tym uzytkownik

	if (flowGraph == NULL)
	{
		flowGraph = this->Clone();
	}
	else
	{
		delete flowGraph;
		flowGraph = this->Clone();
		flowGraph_return = true;
	}

	// wyzerowanie wag w grafie przeplywow
	for (int i = 0; i < flowGraph->getNumOfEdges(); i++)
	{
		flowGraph->setWeight(i, 0);
	}

	if ((path_finding == 'B' || path_finding == 'D') && source >= 0 && source < getNumOfVertices() && sink >= 0 && sink < getNumOfVertices())
	{
		// wlasciwy algorytm

		// poczatkowo grafem residualnym jest po prostu graf sieci przeplywowej

		int* aug_path = NULL;

		if (path_finding == 'B') { aug_path = findAugPathBFS(source, sink, residualGraph); }
		else { aug_path = findAugPathDFS(source, sink, residualGraph); }

		// pokazuje postep wykonania algorytmu
		cout << "Trwa wyszukiwanie najkrotszych sciezek (algorytm Bellmana-Forda)";
		int iter = 0, dl = 7;

		while (aug_path != NULL)
		{
			// pokazuje postep algorytmu
			if (iter % dl == 0 && iter != 0)
			{
				for (int i = 0; i < dl; i++) cout << "\b";
				for (int i = 0; i < dl; i++) cout << " ";
				for (int i = 0; i < dl; i++) cout << "\b";

				iter = 0;
			}
			else
			{
				cout << ".";
				iter++;
			}

			// wyszukanie najmniejszej przepustowosci residualnej dla zwroconej sciezki
			int *min_cap = residualGraph->getWeight(residualGraph->getIndexOfEdge(aug_path[1], aug_path[2]));
			int *akt_cap = NULL;
			for (int i = 2; i < aug_path[0]; i++)
			{
				akt_cap = residualGraph->getWeight(residualGraph->getIndexOfEdge(aug_path[i], aug_path[i + 1]));

				if (*akt_cap < *min_cap)
				{
					*min_cap = *akt_cap;
				}

				if (akt_cap != NULL) { delete akt_cap; akt_cap = NULL; }
			}

			// zwiekszenie przeplywu w znalezionych krawedziach o minimalna przepustowosc residualna
			// oraz zmiana przepustowosci residualnych sieci
			int nr_kraw = -1;
			for (int i = 1; i < aug_path[0]; i++)
			{
				nr_kraw = residualGraph->getIndexOfEdge(aug_path[i], aug_path[i + 1]);
				akt_cap = flowGraph->getWeight(nr_kraw);

				flowGraph->setWeight(nr_kraw, *akt_cap + *min_cap);

				int *G_weight = residualGraph->getWeight(nr_kraw);

				residualGraph->setWeight(nr_kraw, *G_weight - *min_cap);

				if (G_weight != NULL) { delete G_weight; G_weight = NULL; }

				// obsluga krawedzi przeciwnych
				if (!residualGraph->znajdzKrawedz(aug_path[i + 1], aug_path[i]))
				{
					residualGraph->dodajKraw(aug_path[i + 1], aug_path[i], 0);
				}

				int dod_kraw = residualGraph->getIndexOfEdge(aug_path[i + 1], aug_path[i]);

				G_weight = flowGraph->getWeight(nr_kraw);
				residualGraph->setWeight(dod_kraw, *G_weight);
				if (G_weight != NULL) { delete G_weight; G_weight = NULL; }

				if (akt_cap != NULL) { delete akt_cap; }
			}

			// wzrost ogolnego przeplywu
			maxFlow += *min_cap;

			// wyczyszczenie aug_path
			if (aug_path != NULL) { delete[] aug_path; aug_path = NULL; }

			if (min_cap != NULL) { delete min_cap; min_cap = NULL; }

			// ponowne wyszukanie sciezki rozszerzajacej
			if (path_finding == 'B') { aug_path = findAugPathBFS(source, sink, residualGraph); }
			else { aug_path = findAugPathDFS(source, sink, residualGraph); }
		}

	}
	else
	{
		cout << "findMaxflowFordFulkerson: Podano bledne argumenty.";
	}

	// sprzatanie
	if (!flowGraph_return)
	{
		if (flowGraph != NULL)
		{
			delete flowGraph;
			flowGraph = NULL;
		}
	}

	if (residualGraph != NULL)
	{
		delete residualGraph;
		residualGraph = NULL;
	}

	cout << "\n";

	return maxFlow;
}