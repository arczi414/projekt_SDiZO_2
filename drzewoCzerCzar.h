struct Node;

class DrzewoCzerCzar
{
private:
	Node *guard; // wezel straznika
	Node *root; // korzen drzewa
	unsigned int n;	// l. wezlow
	void printTree(Node* rt, int glebokosc);
	void makenull(Node *root);

public:
	DrzewoCzerCzar();
	~DrzewoCzerCzar();

	void insert(int);	// wstawianie do wezla
	bool remove(int);	// usuwanie danych
	Node *find(int);	// znajdowanie danych
	void print();		// wypisanie na ekran
	void makenull();	// usuwa drzewo

	// metody pomocnicze
	void rotationLeft(Node*);
	void rotationRight(Node*);
	Node *nastepnik(Node*);
	Node *poprzednik(Node*);

};

struct Node
{
	Node *up;
	Node *left;
	Node *right;
	int data;
	char color;		// 'r' lub 'b'
};