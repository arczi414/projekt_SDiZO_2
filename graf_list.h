#ifndef _LGRAF_H_
#define _LGRAF_H_

#include "graf.h"
#include "wierzcholek_mac.h"
#include "lista.h"
#include "wierzcholek_sasiad.h"
#include "drzewoCzerCzar.h"

/*
Klasa grafu oparta na macierzy incydencji.
*/
class LGraf : public Graf
{
private:
	List<SasWierzcholek> *listy; // tablica list sasiadow
	DrzewoCzerCzar nr_krawedzi; // tablica zawierajaca informacje o numerach krawedzi i przynaleznym wierzcholku

	int N;				// liczba wierzcholkow
	int M;				// liczba krawedzi

	bool hasMax(int n, bool podwojne_kraw = true);	// mowi czy dany wierzcholek posiada wszystkie mozliwe krawedzie
	char hasConnection(int n1, int n2); // mowi czy dwa wierzcholki sa polaczone

public:
	LGraf();
	LGraf(const LGraf& lg);

	LGraf* Clone();
	LGraf* Create();

	~LGraf();

	LGraf& operator =(const LGraf &lg);

	bool hasCon(int n1, int n2) { return hasConnection(n1, n2); }
	void losujGraf(int n, float gestosc, bool ujemne_wagi = true, bool podwojne_kraw = true); // n - liczba wierzcholkow
	void pokazGraf(); // wypisuje graf na stdout
	bool dodajKraw(int start, int koniec, int waga); // dodaje krawedz
	bool usunKraw(int k); // usuwa podana krawedz
	int dodajWierzch(); // dodaje wierzcholek
	bool znajdzKrawedz(int start, int end); // sprawdza czy dana krawedz istnieje
	int getIndexOfEdge(int start, int end); // zwraca numer krawedzi laczacej podane wierzcholki
	int getStart(int k); // zwraca indeks wierzcholka startowego krawedzi
	int getEnd(int k); // zwraca indeks wierzcholka koncowego krawedzi
	int* getAvailableEdges(int w, bool skierowany = false); // zwraca dostepne krawedzie z danego wierzcholka
	MWierzcholek* getAvailableVertices(int w, bool skierowany = true);
	int* getWeight(int k); // zwraca wage danej krawedzi
	bool setWeight(int k, int waga); // ustawia wage danej krawedzi

	int getNumOfVertices() { return N; }; // zwraca liczbe wierzcholkow
	int getNumOfEdges() { return M; }; // zwraca liczbe krawedzi

};

#endif