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

	bool hasMax(int n);	// mowi czy dany wierzcholek posiada wszystkie mozliwe krawedzie
	char hasConnection(int n1, int n2); // mowi czy dwa wierzcholki sa polaczone

public:
	MGraf();
	~MGraf();

	void losujGraf(int n, float gestosc, bool ujemne_wagi = false);	// n - liczba wierzcholkow
	void pokazGraf();	// wypisuje graf na stdout

};

#endif