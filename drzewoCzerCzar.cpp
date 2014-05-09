#include "stdafx.h"
#include "drzewoCzerCzar.h"

DrzewoCzerCzar::DrzewoCzerCzar()
{
	guard = new Node();
	guard->color = 'b';
	guard->data = 0;
	guard->data2 = 0;
	guard->up = guard;
	guard->left = guard;
	guard->right = guard;
	root = guard;
}

DrzewoCzerCzar::DrzewoCzerCzar(const DrzewoCzerCzar& dcz)
{
	if (dcz.root == dcz.guard)
	{
		guard = new Node();
		guard->color = 'b';
		guard->data = 0;
		guard->data2 = 0;
		guard->up = guard;
		guard->left = guard;
		guard->right = guard;
		root = guard;	

		n = dcz.n;
	}
	else
	{
		guard = new Node();
		guard->color = dcz.guard->color;
		guard->data = dcz.guard->data;
		guard->data2 = dcz.guard->data2;
		guard->up = guard;
		guard->left = guard;
		guard->right = guard;

		n = dcz.n;

		root = copyNode(dcz.root, dcz.guard);
		root->up = guard;
	}
}

DrzewoCzerCzar& DrzewoCzerCzar::operator = (const DrzewoCzerCzar& dcz)
{
	DrzewoCzerCzar nowe(dcz);
	return nowe;
}

Node* DrzewoCzerCzar::copyNode(const Node* nd, const Node* guard)
{
	if (nd == guard)
		return this->guard;
	
	Node* result = new Node();

	result->color = nd->color;
	result->data = nd->data;
	result->data2 = nd->data2;
	result->left = copyNode(nd->left, guard);
	result->right = copyNode(nd->right, guard);
	if (result->left != this->guard)
		result->left->up = result;
	if (result->right != this->guard)
		result->right->up = result;

	return result;
}

DrzewoCzerCzar::~DrzewoCzerCzar()
{
	makenull();
	delete root;
}

// wstawianie elementu do drzewa czerwono-czarnego
void DrzewoCzerCzar::insert(int data, int data2)
{
	Node* tempRoot = guard;
	Node* temp = root;
	// wstawianie elementu jak do BST
	while (temp != guard)
	{
		tempRoot = temp;
		if (data > temp->data)
			temp = temp->right;
		else
			temp = temp->left;
	}

	if (tempRoot == guard)
	{
		root = new Node();
		root->up = guard; root->left = guard; root->right = guard;
		root->data = data; root->data2 = data2; root->color = 'b';
		return;
	}
	else
	{
		if (data > tempRoot->data)
		{
			tempRoot->right = new Node();
			temp = tempRoot->right;
		}
		else
		{
			tempRoot->left = new Node();
			temp = tempRoot->left;
		}

		temp->left = guard; temp->right = guard;
		temp->up = tempRoot; temp->data = data;
		temp->data2 = data2; temp->color = 'r';
	}

	// przywracanie warunkow drzewa czerwono-czarnego
	while (true)
	{
		if (temp->up->color == 'b') return;
		else
		{
			// ustalenie wujka
			Node* wujek = NULL;
			char stronaPokr;	// l - lewy syn, p - prawy syn
			int przypadek = -1; // 0 - domyslnie, 1 - przypadek lustrzany
			if (temp->up->up->left->left == temp)
			{
				stronaPokr = 'l';
				wujek = temp->up->up->right;
				przypadek = 0;
			}
			else if (temp->up->up->left->right == temp)
			{
				stronaPokr = 'p';
				wujek = temp->up->up->right;
				przypadek = 0;
			}
			else if (temp->up->up->right->left == temp)
			{
				stronaPokr = 'l';
				wujek = temp->up->up->left;
				przypadek = 1;
			}
			else if (temp->up->up->right->right == temp)
			{
				stronaPokr = 'p';
				wujek = temp->up->up->left;
				przypadek = 1;
			}
			else return;

			// Przypadek I: wujek czerwony
			if (wujek->color == 'r')
			{
				temp->up->color = 'b';		// kolor ojca
				wujek->color = 'b';			// kolor wujka
				temp->up->up->color = 'r';	// kolor dziadka
				if (temp->up->up == root)
				{
					temp->up->up->color = 'b';
					return;
				}
				else
				{
					temp = temp->up->up;
					continue;
				}
			}
			// Przypadek II: wujek czarny, a temp jest prawym dzieckiem (lub lustro)
			else if (wujek->color == 'b' && stronaPokr == 'p' && przypadek == 0)
			{
				temp = temp->up;
				rotationLeft(temp);
				continue;
			}
			else if (wujek->color == 'b' && stronaPokr == 'l' && przypadek == 1)
			{
				temp = temp->up;
				rotationRight(temp);
				continue;
			}
			// Przypadek III: wujek jest czarny, a temp jest lewym dzieckiem (lub lustro)
			else if (wujek->color == 'b' && stronaPokr == 'l' && przypadek == 0)
			{
				if (temp->up->up != guard)
				{
					if (temp->up->up->color == 'r')
						temp->up->up->color = 'b';
					else
						temp->up->up->color = 'r';
				}

				if (temp->up != guard)
				{
					if (temp->up->color == 'r')
						temp->up->color = 'b';
					else
						temp->up->color = 'r';
				}

				rotationRight(temp->up->up);

				return;
			}
			else if (wujek->color == 'b' && stronaPokr == 'p' && przypadek == 1)
			{
				if (temp->up->up != guard)
				{
					if (temp->up->up->color == 'r')
						temp->up->up->color = 'b';
					else
						temp->up->up->color = 'r';
				}

				if (temp->up != guard)
				{
					if (temp->up->color == 'r')
						temp->up->color = 'b';
					else
						temp->up->color = 'r';
				}

				rotationLeft(temp->up->up);

				return;
			}
		}
	}
}

// znajdowanie elementu w drzewie
Node* DrzewoCzerCzar::find(int data)
{
	Node *temp = root;

	if (temp != guard)
	{
		while (true)
		{
			if (temp->data == data) return temp;

			if (data > temp->data)
				temp = temp->right;
			else
				temp = temp->left;

			if (temp == guard)
				break;
		}
	}

	return NULL;
}

// zerowanie drzewa
void DrzewoCzerCzar::makenull()
{
	makenull(root);
	root = guard;
}

void DrzewoCzerCzar::makenull(Node *root)
{
	while (root->left != guard || root->right != guard)
	{
		if (root->left != guard) { makenull(root->left); }
		if (root->right != guard) { makenull(root->right); }
	}

	if (root->left == guard && root->right == guard && root != guard) 
	{ 
		if (root->up != guard)
		{
			if (root == root->up->left) root->up->left = guard;
			else if (root == root->up->right) root->up->right = guard;
		}

		delete root; 
	}
}

// rotacja w lewo wzgledem wezelOdnies
void DrzewoCzerCzar::rotationLeft(Node* wezelOdnies)
{
	Node *root_temp = wezelOdnies->up;
	if (wezelOdnies->right != guard)
	{
		Node *newRoot = wezelOdnies->right;

		if (wezelOdnies->right->left != guard)
		{
			wezelOdnies->right->left->up = wezelOdnies;
		}

		wezelOdnies->right = wezelOdnies->right->left;

		wezelOdnies->up = newRoot;
		newRoot->up = root_temp;
		newRoot->left = wezelOdnies;

		if (root_temp != guard)
		{
			if (root_temp->left == wezelOdnies)
				root_temp->left = newRoot;
			else if (root_temp->right == wezelOdnies)
				root_temp->right = newRoot;
		}
		else root = newRoot;
	}
}

// rotacja w prawo wzgledem wezelOdnies
void DrzewoCzerCzar::rotationRight(Node* wezelOdnies)
{
	Node *root_temp = wezelOdnies->up;
	if (wezelOdnies->left != guard)
	{
		Node *newRoot = wezelOdnies->left;

		if (wezelOdnies->left->right != guard)
		{
			wezelOdnies->left->right->up = wezelOdnies;
		}

		wezelOdnies->left = wezelOdnies->left->right;

		wezelOdnies->up = newRoot;
		newRoot->up = root_temp;
		newRoot->right = wezelOdnies;

		if (root_temp != guard)
		{
			if (root_temp->left == wezelOdnies)
				root_temp->left = newRoot;
			else if (root_temp->right == wezelOdnies)
				root_temp->right = newRoot;
		}
		else root = newRoot;
	}
}

// zwraca nastepnik danego wezla
Node* DrzewoCzerCzar::nastepnik(Node* wezel)
{
	if (wezel->right == guard)
		return guard;
	else
	{
		wezel = wezel->right;
		while (wezel->left != guard)
			wezel = wezel->left;
	}

	return wezel;
}

// zwraca poprzednik danego wezla
Node* DrzewoCzerCzar::poprzednik(Node* wezel)
{
	if (wezel->left == guard)
		return guard;
	else
	{
		wezel = wezel->left;
		while (wezel->right != guard)
		{
			for (int i = 0; i < 1000; i++);

			wezel = wezel->right;
		}
	}

	return wezel;
}

// usuwa wezel z zadanymi danymi
bool DrzewoCzerCzar::remove(int data)
{
	Node* X = find(data);

	if (X != NULL)
	{
		Node * W, *Y, *Z;

		if ((X->left == guard) || (X->right == guard)) Y = X;
		else                                    Y = poprzednik(X);

		if (Y->left != guard) Z = Y->left;
		else              Z = Y->right;

		Z->up = Y->up;

		if (Y->up == guard) root = Z;
		else if (Y == Y->up->left) Y->up->left = Z;
		else                      Y->up->right = Z;

		if (Y != X) X->data = Y->data;

		// przywracanie struktury drzewa czerwono-czarnego
		if (Y->color == 'b')
		while ((Z != root) && (Z->color == 'b'))
		{
			//opoznienie do pomiarow
			for (int i = 0; i < 2000; i++);

			if (Z == Z->up->left)
			{
				W = Z->up->right;

				if (W->color == 'r')
				{              
					W->color = 'b';
					Z->up->color = 'r';
					rotationLeft(Z->up);
					W = Z->up->right;
				}

				if ((W->left->color == 'b') && (W->right->color == 'b'))
				{            
					W->color = 'r';
					Z = Z->up;
					continue;
				}

				if (W->right->color == 'b')
				{            
					W->left->color = 'b';
					W->color = 'r';
					rotationRight(W);
					W = Z->up->right;
				}

				W->color = Z->up->color;
				Z->up->color = 'b';
				W->right->color = 'b';
				rotationLeft(Z->up);
				Z = root;  
			}
			else
			{             
				W = Z->up->left;

				if (W->color == 'r')
				{      
					W->color = 'b';
					Z->up->color = 'r';
					rotationRight(Z->up);
					W = Z->up->left;
				}

				if ((W->left->color == 'b') && (W->right->color == 'b'))
				{        
					W->color = 'r';
					Z = Z->up;
					continue;
				}

				if (W->left->color == 'b')
				{      
					W->right->color = 'b';
					W->color = 'r';
					rotationLeft(W);
					W = Z->up->left;
				}

				W->color = Z->up->color; 
				Z->up->color = 'b';
				W->left->color = 'b';
				rotationRight(Z->up);
				Z = root;         
			}
		}

		Z->color = 'b';

		delete Y;

		return true;
	}

	return false;
}

// wypisuje zawartosc drzewa
void  DrzewoCzerCzar::print()
{
	if (root != guard)
	{
		std::cout << root->data << " (" << (root->color=='r'?"red":"black") << ")" << "\n";
		printTree(root, 1);
	}
}

// funkcja glowna wypisania na ekran
void DrzewoCzerCzar::printTree(Node* rt, int glebokosc)
{
	if (rt->left != guard)
	{
		for (int i = 0; i < glebokosc; i++)
			std::cout << "\t";
		std::cout << rt->left->data << " (" << (rt->color == 'r' ? "red" : "black") << ")" << "\n";
		printTree(rt->left, glebokosc + 1);
	}

	if (rt->right != guard)
	{
		for (int i = 0; i < glebokosc; i++)
			std::cout << "\t";
		std::cout << rt->right->data << " (" << (rt->color == 'r' ? "red" : "black") << ")" << "\n";
		printTree(rt->right, glebokosc + 1);
	}
}