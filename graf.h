#ifndef _GRAF_H_
#define _GRAF_H_

#include "wierzcholek_mac.h"

class Graf
{
protected:
	virtual bool hasMax(int n, bool podwojne_kraw = true) = 0;	// mowi czy dany wierzcholek posiada wszystkie mozliwe krawedzie
	virtual char hasConnection(int n1, int n2) = 0;				// mowi czy dwa wierzcholki sa polaczone

public:
	Graf() {};
	Graf(const Graf &mg) {};

	virtual ~Graf() {};

	virtual Graf* Clone() = 0;
	virtual Graf* Create() = 0;

	virtual void losujGraf(int n, float gestosc, bool ujemne_wagi = true, bool podwojne_kraw = true) = 0;	// n - liczba wierzcholkow
	virtual void pokazGraf() = 0;										// wypisuje graf na stdout
	virtual bool dodajKraw(int start, int koniec, int waga) = 0;		// dodaje krawedz
	virtual bool usunKraw(int k) = 0;									// usuwa podana krawedz
	virtual void usunPodwojne() = 0;									// usuwa podwojne krawedzie w drzewie (zostawia te o mniejszej wadze)
	virtual int dodajWierzch() = 0;										// dodaje wierzcholek
	virtual bool znajdzKrawedz(int start, int end) = 0;					// sprawdza czy dana krawedz istnieje
	virtual int getIndexOfEdge(int start, int end) = 0;					// zwraca numer krawedzi laczacej podane wierzcholki
	virtual int getStart(int k) = 0;									// zwraca indeks wierzcholka startowego krawedzi
	virtual int getEnd(int k) = 0;										// zwraca indeks wierzcholka koncowego krawedzi
	virtual int* getAvailableEdges(int w, bool skierowany = false) = 0; // zwraca dostepne krawedzie z danego wierzcholka
	virtual MWierzcholek* getAvailableVertices(int w, bool skierowany = true) = 0;
	virtual int* getWeight(int k) = 0;									// zwraca wage danej krawedzi
	virtual bool setWeight(int k, int waga) = 0;						// ustawia wage danej krawedzi

	virtual int getNumOfVertices() = 0;									// zwraca liczbe wierzcholkow
	virtual int getNumOfEdges() = 0;									// zwraca liczbe krawedzi


	/*__________________________A L G O R Y T M Y______________________*/

	/* ******************Minimalne drzewo rozpinajace**************** */
	void mstPrim(Graf* mst, bool podwojne_kraw = false){};
	void mstKruskal(Graf* mst, bool podwojne_kraw = false){};

	/* ****************Wyszukiwanie najkrotszej sciezki************** */
	void sptDijkstra(int w, int *&koszty, int *&poprz){};
	bool sptBellmanFord(int w, int **&koszty, int *&poprz){ return false; };

	/* *********************Przeszukiwanie grafu********************* */
	int* findAugPathDFS(int start, int end, Graf* const rsGraph);
	int* findAugPathBFS(int start, int end, Graf* const rsGraph);

	/* *********************Najwiekszy przeplyw********************* */
	int findMaxflowFordFulkerson(int source, int sink, char path_finding = 'B', Graf* flowGraph = NULL);

};

#endif