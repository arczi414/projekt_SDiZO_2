#pragma once
#include "lista.h"

template <class T>
class Kopiec
{
private:
	T* kopiec;
	int n; // rozmiar kopca
	bool max_min = false; // false - kopiec max, true - kopiec min
	
	/*
		Funkcja rekurencyjna, pomocnicza dla find().
	*/
	bool find(T data, int wezel)
	{
		int child1 = wezel * 2 + 1;
		int child2 = wezel * 2 + 2;
		bool z = false;

		if (kopiec[wezel] == data)
			return true;
		else if (wezel < n && kopiec[wezel] != data)
		{
			if (child1 < n)
				z = find(data, child1);

			if (z == true)
				return true;

			if (child2 < n)
				z = find(data, child2);

			if (z == true)
				return true;
		}

		return false;
	};

	/* 
		Funkcja glowna wypisania na ekran
	*/
	void printTree(int wezel, int glebokosc)
	{
		int child1 = wezel * 2 + 1;
		int child2 = wezel * 2 + 2;

		if (child1 < n)
		{
			for (int i = 0; i < glebokosc; i++)
				std::cout << "\t";
			std::cout << kopiec[child1] << "\n";
			printTree(child1, glebokosc + 1);
		}

		if (child2 < n)
		{
			for (int i = 0; i < glebokosc; i++)
				std::cout << "\t";
			std::cout << kopiec[child2] << "\n";
			printTree(child2, glebokosc + 1);
		}
	};

public:
	Kopiec()
	{
		kopiec = NULL;
		n = 0;
	};

	~Kopiec()
	{
		delete[] kopiec;
		kopiec = NULL;
	};

	/*
		Dok³ada nowy element do kopca.
	*/
	void push(T data)
	{
		// dodanie nowego elementu i przepisanie tablicy
		T* kopiec_temp = new T[n + 1];
		for (int i = 0; i < n; i++)
		{
			kopiec_temp[i] = kopiec[i];
		}

		delete[] kopiec;
		kopiec = kopiec_temp;

		n++;
		kopiec_temp[n - 1] = data;

		// przywracanie wlasnosci kopca
		int nowyElement, rodzic;
		nowyElement = n - 1;
		rodzic = floor((nowyElement - 1) / 2);

		if (max_min == false)
		{
			while (rodzic >= 0 && kopiec[rodzic] < kopiec[nowyElement])
			{
				T temp = kopiec[rodzic];
				kopiec[rodzic] = kopiec[nowyElement];
				kopiec[nowyElement] = temp;
				nowyElement = rodzic;
				rodzic = floor((nowyElement - 1) / 2);
			}
		}
		else
		{
			while (rodzic >= 0 && kopiec[rodzic] > kopiec[nowyElement])
			{
				T temp = kopiec[rodzic];
				kopiec[rodzic] = kopiec[nowyElement];
				kopiec[nowyElement] = temp;
				nowyElement = rodzic;
				rodzic = floor((nowyElement - 1) / 2);
			}
		}
	};

	/*
		Zdejmuje i zwraca korzen kopca.
	*/
	T* pop()
	{
		// zamiana korzenia z ostatnim lisciem
		if (n == 1)
		{
			n--;
			T save = kopiec[0];
			delete[] kopiec;
			kopiec = NULL;
			return new T(save);
		}
		else if (n > 1)
		{
			T save = kopiec[0];	// korzen
			kopiec[0] = kopiec[n - 1];	// zamiana z ostatnim lisciem

			int akt = 0;
			int left = 1;
			int right = 2;

			if (max_min == false)
			{
				while ((left < n - 1) && (kopiec[left] > kopiec[akt] || kopiec[right] > kopiec[akt]))
				{
					if (right >= n - 1)
					{
						T saveL = kopiec[akt];
						kopiec[akt] = kopiec[left];
						kopiec[left] = saveL;

						akt = left;
						left = 2 * akt + 1;
						right = 2 * akt + 2;
					}
					else
					{
						if (kopiec[left] > kopiec[right])
						{
							T saveL = kopiec[akt];
							kopiec[akt] = kopiec[left];
							kopiec[left] = saveL;

							akt = left;
							left = 2 * akt + 1;
							right = 2 * akt + 2;
						}
						else
						{
							T saveR = kopiec[akt];
							kopiec[akt] = kopiec[right];
							kopiec[right] = saveR;

							akt = right;
							left = 2 * akt + 1;
							right = 2 * akt + 2;
						}
					}
				}
			}
			else
			{
				while ((left < n - 1) && (kopiec[left] < kopiec[akt] || kopiec[right] < kopiec[akt]))
				{
					if (right >= n - 1)
					{
						T saveL = kopiec[akt];
						kopiec[akt] = kopiec[left];
						kopiec[left] = saveL;

						akt = left;
						left = 2 * akt + 1;
						right = 2 * akt + 2;
					}
					else
					{
						if (kopiec[left] < kopiec[right])
						{
							T saveL = kopiec[akt];
							kopiec[akt] = kopiec[left];
							kopiec[left] = saveL;

							akt = left;
							left = 2 * akt + 1;
							right = 2 * akt + 2;
						}
						else
						{
							T saveR = kopiec[akt];
							kopiec[akt] = kopiec[right];
							kopiec[right] = saveR;

							akt = right;
							left = 2 * akt + 1;
							right = 2 * akt + 2;
						}
					}
				}
			}

			// usuniecie ostatniego liscia drzewa
			T *temp = kopiec;
			kopiec = new T[n - 1];
			for (int i = 0; i < n - 1; i++)
				kopiec[i] = temp[i];

			delete[] temp;
			n--;
			return new T(save);
		}

		return NULL;
	};

	/*
		Wyszukuje i usuwa wybrany element z kopca. Zwraca false
		gdy zadany element nie istnieje.
	*/
	bool usunWybrany(T data)
	{
		bool found = false;

		if (n > 0)
		{
			int *temp = NULL;
			int prev = 0;
			Lista *tmpLst = new Lista();

			// sciaganie elementow do natrafienia na szukany
			do
			{
				temp = pop();
				if (*temp != data) { tmpLst->insert(prev, *temp); }
				prev = *temp;
			} while (*temp != data && n > 0);

			found = *temp == data ? true : false;

			// wprowadzenie œci¹gniêtych elementów spowrotem do kopca
			tmpLst->reset(1);
			do
			{
				temp = tmpLst->next();
				if (temp != NULL) { push(*temp); }
			} while (tmpLst->hasNext());

			// ostani element na liscie
			temp = tmpLst->next();
			if (temp != NULL) { push(*temp); }

			if (found) { return true; }
		}

		return false;
	};

	/*
		Sprawdza czy zadany element istnieje w strukturze.
	*/
	bool find(T data)
	{
		if (n>0)
			return find(data, 0);

		return false;
	};

	/* 
		Wypisuje zawartosc kopca
	*/
	void print()
	{
		if (kopiec != NULL)
		{
			std::cout << kopiec[0] << "\n";
			printTree(0, 1);
		}
	};

	void setMax()
	{
		max_min = false;
	};

	void setMin()
	{
		max_min = true;
	};

};