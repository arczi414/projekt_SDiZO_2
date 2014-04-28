#include "stdafx.h"
#include "zbiory_rozlaczne.h"

ZbioryRozlaczne::ZbioryRozlaczne(int n) :
n(n)
{ 
	zbior = new Node[n];
}

ZbioryRozlaczne::~ZbioryRozlaczne()
{
	if (zbior != NULL)
	{
		delete[] zbior;
	}
}

/*
	Funkcja tworzy zbior, w ktorym korzeniem jest podany wierzcholek.
*/
void ZbioryRozlaczne::MakeSet(Node *node)
{
	zbior[node->w].up = &zbior[node->w];
}

/*
	Funkcja zwaca wierzcholek bedacy korzeniem zbioru.
*/
Node* ZbioryRozlaczne::FindSet(Node *node)
{
	Node *temp = node;
	while (temp->up != temp)
	{
		temp = temp->up;
	}

	return temp;
}

/*
	Funkcja dolacza zbior z node2 do zbioru z node1. Jesli
	obydwa wierzcholki maja wspolny korzen, funkcja konczy dzialanie.
*/
void ZbioryRozlaczne::UnionSets(Node *node1, Node *node2)
{
	Node *root1 = node1;
	Node *root2 = node2;

	// wyszukanie korzenia dla node1
	while (root1->up != root1)
	{
		root1 = root1->up;
	}

	// wyszukanie korzenia dla node2
	while (root2->up != root2)
	{
		root2 = root2->up;
	}

	// wierzcholki nie moga miec tego samego korzenia
	if (root1 == root2)
	{
		return;
	}
	else
	{
		root2->up = root1;
	}
}