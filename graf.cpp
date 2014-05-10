#include "stdafx.h"
#include "graf.h"
#include "kopiec.h"
#include "krawedz_mac.h"
#include "lista.h"
#include "drzewoCzerCzar.h"
#include "zbiory_rozlaczne.h"
#include <iomanip>

using namespace std;

/*
Funkcja usuwa podwojne krawedzie w grafie. Jesli znajdzie dwa wierzcholki miedzy
ktorymi znajduja sie dwie krawedzie pozostawia te o mniejszej wadze.
*/
void Graf::usunPodwojne()
{
	int start_a, start_s;
	int koniec_a, koniec_s;

	for (int k = 0; k < getNumOfEdges(); k++)
	{
		start_a = getStart(k);
		koniec_a = getEnd(k);

		for (int i = 0; i < getNumOfEdges(); i++)
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
					int *w_s = getWeight(i);
					int *w_a = getWeight(k);

					// usuniecie krawedzi o wiekszej wadze
					if (*w_s > *w_a)
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

					if (w_s != NULL) { delete w_s; w_s = NULL; }
					if (w_a != NULL) { delete w_a; w_a = NULL; }

					// nie moga wystapic trzy krawedzie pomiedzy dwoma wierzcholkami
					// dlatego nie ma sensu porownywac reszte krawedzi
					break;
				}
			}
		}
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
Graf* Graf::mstPrim(bool podwojne_kraw)
{
	if (podwojne_kraw)
	{
		usunPodwojne();
	}

	Graf *mst = Create();
	DrzewoCzerCzar dodaneKraw; // zawiera numery dodanych krawedzi
	DrzewoCzerCzar dodaneWierzch; // zawiera numery dodanych wierzcholkow

	// dodanie do drzewa mst wierzcholkow
	for (int i = 0; i < getNumOfVertices(); i++)
	{
		mst->dodajWierzch();
	}

	Kopiec<MKrawedz> kp; // kolejka priorytetowa do przetrzymywania krawedzi
	kp.setMin();

	int poprz = 0; // indeks poprzednio dodanego wierzcholka do grafu

	dodaneWierzch.insert(poprz);

	cout << "Trwa wyszukiwanie MST (algorytm Prima):";
	cout << setw(5) << " ";

	for (int i = 0; i < getNumOfVertices() - 1; i++)
	{
		// pokazuje postep algorytmu
		int wart = (static_cast<float>(i) / (getNumOfVertices() - 1)) * 100;
		cout << "\b\b\b\b\b" << setw(4) << wart << "%";

		// przeszukanie dostepnych krawedzi wierzcholka 'poprz'
		int *dostepne = getAvailableEdges(poprz);
		if (dostepne != NULL)
		{
			for (int k = 1; k <= dostepne[0]; k++)
			{
				int nr_k = dostepne[k];
				
				int *weight = getWeight(nr_k);
				MKrawedz kraw(*weight, nr_k);
				if (weight != NULL) { delete weight; weight = NULL; }
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
			if (kr != NULL) { delete kr; kr = NULL; }
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
Graf* Graf::mstKruskal(bool podwojne_kraw)
{
	if (podwojne_kraw)
	{
		usunPodwojne();
	}

	Graf *mst = Create();

	// dodanie do drzewa mst wierzcholkow
	for (int i = 0; i < getNumOfVertices(); i++)
	{
		mst->dodajWierzch();
	}

	// kolejka przechowujaca krawedzie
	Kopiec<MKrawedz> krawedzie;
	krawedzie.setMin();

	// zbiory rozlaczne do przechowywania dodanych wierzcholkow
	ZbioryRozlaczne wierzcholki(getNumOfVertices());

	// dodanie wszystkich krawedzi do kopca
	for (int i = 0; i < getNumOfEdges(); i++)
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

	while (wstawione < getNumOfVertices() - 1)
	{
		// pokazuje postep algorytmu
		int wart = (static_cast<float>(wstawione) / (getNumOfVertices() - 1)) * 100;
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

		if (mk != NULL) { delete mk; mk = NULL; }
	}


	// pokazuje postep algorytmu
	cout << "\b\b\b\b\b" << setw(4) << 100 << "%";

	return mst;
}



/*_____________wyszukiwanie najkrotszej sciezki_______*/

/*
Algorytm Dijkstry znajdowania najkrotszej sciezki w grafie.
Graf musi posiadac nieujemne wagi.

w - wierzcholek wzgledem ktorego wyszukiwana jest sciezka
koszty - tablica kosztow
poprz - tablica poprzednikow na najkrotszej sciezce
*/
void Graf::sptDijkstra(int w, int *&koszty, int *&poprz)
{
	// init

	if (koszty != NULL)
	{
		delete[] koszty;
		koszty = NULL;
	}

	if (poprz != NULL)
	{
		delete[] poprz;
		poprz = NULL;
	}

	// tablica z informacja o koszcie dojscia z poszczegolnych wierzcholkow
	koszty = new int[getNumOfVertices()];

	// tablica poprzednikow
	poprz = new int[getNumOfVertices()];

	// kopiec zawierajacy dostepne wierzcholki
	Kopiec<MWierzcholek> dostepne;
	dostepne.setMin();

	// tablica przechowujaca informacje o tym, czy dany wierzcholek zostal juz
	// przeniesiony do zbioru S (obliczono dla niego koszt przejscia)
	// jesli SQ[i] jest rowne false oznacza to, ze 'w' nalezy do zbioru Q
	// jesli SQ[i] wynosi true to 'w' znajduje sie w S
	bool *SQ = new bool[getNumOfVertices()];

	// poczatkowo wszystkie wierzcholki naleza do Q
	// a wszystkie koszty i nr poprzednikow wynosza -1
	for (int i = 0; i < getNumOfVertices(); i++)
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
		// przeszukanie sasiadow 'w'
		int n_mozliweW = mozliweW[0].koszt_dojscia; // liczba sasiadow

		for (int i = 1; i <= n_mozliweW; i++)
		{
			int akt_w = mozliweW[i].nr_wierzch;

			// sprawdzenie czy wierzcholek nalezy do zbioru Q
			if (SQ[akt_w] == false)
			{
				// dodanie akt_w do dostepnych wierzcholkow
				if (koszty[akt_w] == -1)
				{
					koszty[akt_w] = mozliweW[i].koszt_dojscia;
					dostepne.push(MWierzcholek(koszty[akt_w], akt_w, w));
				}
			}
		}

		if (mozliweW != NULL)
		{
			delete[] mozliweW;
			mozliweW = NULL;
		}

		//___________analiza pozostalych wierzcholkow_______________________
		MWierzcholek *main_w = NULL;
		int akt_w_int = -1;
		n_mozliweW = -1;

		// pokazuje postep wykonania algorytmu
		cout << "Trwa wyszukiwanie najkrotszych sciezek (algorytm Dijkstry):";
		cout << setw(5) << " ";

		for (int i = 1; i < getNumOfVertices(); i++)
		{
			// pokazuje postep algorytmu
			int wart = (static_cast<float>(i) / (getNumOfVertices() - 1)) * 100;
			cout << "\b\b\b\b\b" << setw(4) << wart << "%";

			main_w = dostepne.pop(); // zdjecie z kopca wierzcholka o najmniejszym koszcie

			// zakonczenie algorymtu, jesli na stosie nie ma juz wierzcholkow
			if (main_w != NULL)
			{
				akt_w_int = main_w->nr_wierzch;
				SQ[akt_w_int] = true;

				mozliweW = getAvailableVertices(akt_w_int);

				// jesli nie akt_w nie ma sasiadow koszt dojscia do niego nie istnieje
				if (mozliweW != NULL)
				{
					n_mozliweW = mozliweW[0].koszt_dojscia; // liczba sasiadow

					for (int i = 1; i <= n_mozliweW; i++)
					{
						int akt_w = mozliweW[i].nr_wierzch;

						// sprawdzenie czy wierzcholek nalezy do zbioru Q
						if (SQ[akt_w] == false)
						{
							// dodanie akt_w do dostepnych wierzcholkow
							if (koszty[akt_w] == -1)
							{
								koszty[akt_w] = mozliweW[i].koszt_dojscia + koszty[akt_w_int];
								dostepne.push(MWierzcholek(koszty[akt_w], akt_w, w));
								poprz[akt_w] = akt_w_int;
							}
							else if (mozliweW[i].koszt_dojscia + koszty[akt_w_int] < koszty[akt_w])
							{
								koszty[akt_w] = mozliweW[i].koszt_dojscia + koszty[akt_w_int];
								dostepne.push(MWierzcholek(koszty[akt_w], akt_w, w));
								poprz[akt_w] = akt_w_int;
							}
						}
					}
				}
			}

			if (main_w != NULL) { delete main_w; main_w = NULL; }
			if (mozliweW != NULL) { delete[] mozliweW; mozliweW = NULL; }
		}
	}

	if (SQ != NULL) { delete[] SQ; SQ = NULL; }

	// pokazuje postep algorytmu
	cout << "\b\b\b\b\b" << setw(4) << 100 << "%\n";
}

/*
Algorytm Bellmana-Forda znajdowania najkrotszej sciezki w grafie.
Graf moze miec ujemne wagi.

Funkcja zwraca true jesli w grafie istnieje ujemny cykl, co oznacza, ze mozna osiagnac
dowolnie niskie koszty przejscia miedzy wierzcholkami.

w - wierzcholek wzgledem ktorego wyszukiwana jest sciezka
koszty - tablica kosztow
poprz - tablica poprzednikow na najkrotszej sciezce
*/
bool Graf::sptBellmanFord(int w, int **&koszty, int *&poprz)
{
	// init
	if (koszty != NULL)	{ delete[] koszty; koszty = NULL; }
	if (poprz != NULL) { delete[] poprz; poprz = NULL; }

	koszty = new int*[getNumOfVertices()]; // tablica z informacja o koszcie dojscia z poszczegolnych wierzcholkow
	poprz = new int[getNumOfVertices()]; // tablica poprzednikow

	// poczatkowo wszystkie nr poprzednikow wynosza -1 oraz nie posiadaja obliczonego kosztu
	for (int i = 0; i < getNumOfVertices(); i++)
	{
		koszty[i] = NULL; // NULL traktowane jest jako nieskonczonosc
		poprz[i] = -1;
	}

	// rozpoczecie od glownego wierzcholka
	koszty[w] = new int(0);

	int *weight = NULL;
	int end = -1;
	int start = -1;

	// pokazuje postep wykonania algorytmu
	cout << "Trwa wyszukiwanie najkrotszych sciezek (algorytm Bellmana-Forda):";
	cout << setw(5) << " ";

	for (int i = 1; i < getNumOfVertices(); i++)
	{
		// pokazuje postep algorytmu
		int wart = (static_cast<float>(i) / (getNumOfVertices() + getNumOfEdges() - 2)) * 100;
		cout << "\b\b\b\b\b" << setw(4) << wart << "%";

		// relaksacja wszystkich krawedzi
		for (int k = 0; k < getNumOfEdges(); k++)
		{
			weight = getWeight(k);
			end = getEnd(k);
			start = getStart(k);

			if (koszty[start] != NULL && koszty[end] == NULL)
			{
				koszty[end] = new int(*koszty[start] + *weight);
				poprz[end] = start;
			}
			else if (koszty[end] != NULL && koszty[start] != NULL)
			{
				if (*koszty[end] > *koszty[start] + *weight)
				{
					*koszty[end] = *koszty[start] + *weight;
					poprz[end] = start;
				}
			}

			if (weight != NULL) { delete weight; weight = NULL; }
		}
	}

	// szukanie ujemnego cyklu
	bool cykl_ujemny = false;
	for (int i = 0; i < getNumOfEdges(); i++)
	{
		// pokazuje postep algorytmu
		int wart = (static_cast<float>(getNumOfVertices() + i) / (getNumOfVertices() + getNumOfEdges() - 2)) * 100;
		cout << "\b\b\b\b\b" << setw(4) << wart << "%";

		start = getStart(i); end = getEnd(i); weight = getWeight(i);

		if (koszty[end] != NULL && koszty[start] != NULL)
		{
			// jesli znalezione koszty mozna poprawic to w grafie istnieje cykl ujemny
			if (*koszty[end] > *koszty[start] + *weight) { cykl_ujemny = true; break; }
		}

		if (weight != NULL) { delete weight; weight = NULL; }
	}

	if (weight != NULL) { delete weight; weight = NULL; }

	// pokazuje postep algorytmu
	cout << "\b\b\b\b\b" << setw(4) << 100 << "%\n";

	return cykl_ujemny;
}

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

			if (sasiedziW != NULL) { 
				delete[] sasiedziW; 
				sasiedziW = NULL; 
			}

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