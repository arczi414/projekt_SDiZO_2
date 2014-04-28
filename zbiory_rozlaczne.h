#ifndef _ZBIORY_ROZL_H_
#define _ZBIORY_ROZL_H_

struct Node;

/*
	Struktura zarzadzajaca zbiorami rozlacznymi. Pozwala stworzyc nowy
	zbior, sprawdzic do jakiego zbioru nalezy dany wierzcholek oraz
	scalic dwa zbiory.

	Klasa przydatna jest przy sprawdzaniu cykli w grafie.
*/
class ZbioryRozlaczne
{
private:
	Node* zbior; // tablica wszystkich wierzcholkow
	int n; // liczba wierzcholkow

public:
	ZbioryRozlaczne(int n); // n - liczba wierzcholkow
	~ZbioryRozlaczne();

	void MakeSet(Node *node);
	Node* FindSet(Node *node);
	void UnionSets(Node *node1, Node *node2);

};

/*
	Struktura przechowujaca informacje o wierzcholku.
*/
struct Node
{
	Node() :
	up(NULL), w(N)
	{
		N = N + 1;
	}

	static int N;

	Node* up; // rodzic wezla

	int w; // numer wierzcholka
};

#endif