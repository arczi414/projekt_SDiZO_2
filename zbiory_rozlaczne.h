#ifndef _ZBIORY_ROZL_H_
#define _ZBIORY_ROZL_H_

/*
	Struktura zarzadzajaca zbiorami rozlacznymi. Pozwala stworzyc nowy
	zbior, sprawdzic do jakiego zbioru nalezy dany wierzcholek oraz
	scalic dwa zbiory.

	Klasa przydatna jest przy sprawdzaniu cykli w grafie.
*/
class ZbioryRozlaczne
{
private:
	int* zbior; // tablica wszystkich wierzcholkow
	int* repr; // tablica reprezentantow danych wierzcholkow
	int n; // liczba wierzcholkow

public:
	ZbioryRozlaczne(int n); // n - liczba wierzcholkow
	~ZbioryRozlaczne();

	/*
		Funkcja tworzy zbior, w ktorym korzeniem jest podany wierzcholek.
	*/
	void MakeSet(int w) { repr[w] = w; };

	/*
		Funkcja zwraca reprezentanta zbioru, do ktorego nalezy
		podany wierzcholek.
	*/
	int FindSet(int w) { return repr[w]; };

	/*
		Funkcja laczy dwa zbiory w jeden, reprezentantem nowego zbioru
		staje sie reprezentant pierwszego zbioru.
	*/
	void UnionSets(int w1, int w2);

};

#endif