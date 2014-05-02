#include "stdafx.h"
#include "zbiory_rozlaczne.h"

ZbioryRozlaczne::ZbioryRozlaczne(int n)
{
	this->n = n;
	zbior = new int[n];
	repr = new int[n];

	for (int i = 0; i < n; i++)
	{
	
		zbior[i] = repr[i] = i;
	}
}

ZbioryRozlaczne::~ZbioryRozlaczne()
{
	if (zbior != NULL)
	{
		delete[] zbior;
	}

	if (repr != NULL)
	{
		delete[] repr;
	}

	zbior = NULL;
	repr = NULL;
}

/*
	Funkcja laczy dwa zbiory w jeden, reprezentantem nowego zbioru
	staje sie reprezentant pierwszego zbioru.
*/
void ZbioryRozlaczne::UnionSets(int w1, int w2)
{
	// wyszukanie wszystkich wierzcholkow w2
	for (int i = 0; i < n; i++)
	{
		if (repr[i] == repr[w2] && i != w2)
		{
			std::cout << "lol";
			repr[i] = repr[w1];
		}
	}

	repr[w2] = repr[w1];
}