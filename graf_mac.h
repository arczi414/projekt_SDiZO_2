#ifndef _MGRAF_H_
#define _MGRAF_H_

/*
	Klasa grafu oparta na macierzy incydencji.
*/
class MGraf
{
private:
	/*
		Wskaznik na macierz incydencji. Ulozenie wartosci odbywa sie
		wedlug nastepujacej zasady: w tablicy ulozone sa kolejne wiersze
		jeden za drugim. Zakladajac, ze n - l. wierszy, m - l. krawedzi, 
		tablica ma dlugosc n*m. Np. element o indeksie 2n + m odpowiada
		informacji o zwiazku m-tej krawedzi z trzecim wierzcholkiem.
		Wartosc pod zadanym indeksem jest rowna wadze danej krawedzi.
	*/
	char *macierz;	
	int *wagi;			// macierz wag grafu, o takiej samiej konstrukcji jak 'macierz'

	int N;				// liczba wierzcholkow
	int M;				// liczba krawedzi

	bool hasMax(int n, bool podwojne_kraw = true);	// mowi czy dany wierzcholek posiada wszystkie mozliwe krawedzie
	char hasConnection(int n1, int n2); // mowi czy dwa wierzcholki sa polaczone

public:
	MGraf();
	~MGraf();

	void losujGraf(int n, float gestosc, bool ujemne_wagi = true, bool podwojne_kraw = true); // n - liczba wierzcholkow
	void pokazGraf(); // wypisuje graf na stdout
	bool dodajKraw(int start, int koniec, int waga); // dodaje krawedz
	bool usunKraw(int k); // usuwa podana krawedz
	void usunPodwojne(); // usuwa podwojne krawedzie w drzewie (zostawia te o mniejszej wadze)
	int dodajWierzch(); // dodaje wierzcholek
	bool znajdzKrawedz(int start, int end); // sprawdza czy dana krawedz istnieje
	int getStart(int k); // zwraca indeks wierzcholka startowego krawedzi
	int getEnd(int k); // zwraca indeks wierzcholka koncowego krawedzi
	int* getWeight(int k); // zwraca wage danej krawedzi
	
	int getNumOfVertices() { return N; }; // zwraca liczbe wierzcholkow
	int getNumOfEdges() { return M; }; // zwraca liczbe krawedzi

	/* ******************Minimalne drzewo rozpinajace**************** */
	MGraf* mstPrim(bool podwojne_kraw = false);
	MGraf* mstKruskal(bool podwojne_kraw = false);

};

#endif