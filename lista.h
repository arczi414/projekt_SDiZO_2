#pragma once

template <class T>
class List
{
private:
	T *pointer;
	T *head;

	// liczba elementow
	int n = 0;

public:
	List()
	{
		n = 0;
		head = new T();
		pointer = head;
	};

	List(List<T>& ls)
	{
		n = ls.n;
		head = new T();
		
		ls.reset(1);
		if (ls.pointer != ls.head)
		{
			head->nextElement = new T(*(ls.pointer));
			pointer = head->nextElement;
			T *temp = NULL;
			while (temp = ls.next())
			{
				pointer->nextElement = new T(*temp);
				pointer->prevElement = pointer;
				pointer = pointer->nextElement;
			}

			head->nextElement->prevElement = NULL;
		}
	};

	~List()
	{
		makenull();
		
		if (head != NULL)
		{
			delete head;
			head = NULL;
		}

		pointer = NULL;
	};

	List<T>& operator=(List<T>& ls)
	{
		if (this == &ls)
		{
			return *this;
		}

		makenull();

		if (head != NULL)
		{
			delete head;
			head = NULL;
		}

		n = ls.n;
		head = new T();

		ls.reset(1);
		if (ls.pointer != ls.head)
		{
			head->nextElement = new T(*(ls.pointer));
			pointer = head->nextElement;
			T *temp = NULL;
			ls.next();
			while (temp = ls.next())
			{
				pointer->nextElement = new T(*temp);
				pointer->prevElement = pointer;
				pointer = pointer->nextElement;
			}

			head->nextElement->prevElement = NULL;
		}

		return *this;
	};

	/*
		Wstawia 'wstaw' za elementem o zawartosci 'pop'.
		Jesli nie istnieje element 'pop' to element jest
		wstawiany na poczatku listy. Jesli istnieje kilka
		elementow o wartosci 'pop' to 'wstaw' wstawiany jest
		za pierwszym znalezionym.
		Dodatkowo, jesli ustawiona jest wartosc beg, wymuszane
		jest wstawienie 'wstaw' na poczatek listy.
	*/
	void insert(T poprz, T nowy, bool beg = false)
	{
		T *p_pop;

		if (beg == false)
			p_pop = find(poprz);
		else
			p_pop = NULL;

		if (p_pop != NULL)
		{
			// zlinkowanie nowego elementu i jego poprzednika
			T *next = p_pop->nextElement;
			p_pop->nextElement = new T(nowy);
			p_pop->nextElement->nextElement = next;
			p_pop->nextElement->prevElement = p_pop;

			// ustawienie pointera na nowym elemencie
			pointer = p_pop->nextElement;

			// zlinkowanie nastepnika
			if (next != NULL)
				next->prevElement = p_pop->nextElement;

			n++;
		}
		else
		{
			// zlinkowanie nowego elementu i head'a
			T *next = head->nextElement;
			head->nextElement = new T(nowy);
			head->nextElement->nextElement = next;
			head->nextElement->prevElement = NULL;

			// ustawienie pointera na nowym elemencie
			pointer = head->nextElement;

			// zlinkowanie nastepnika
			if (next != NULL)
				next->prevElement = head->nextElement;

			n++;
		}
	};
	
	/*
		Usuwa element zawierajacy 'usuw'. Jesli nie  uda sie znalezc 'usuw'
		funkcja zwraca 'false', w przeciwnym wypadku zwraca 'true'. Jesli
		pointer wskazuje na usuwany element, pointer zmienia swoje wskazanie
		na pierwszy element.
	*/
	bool remove(T usuw)
	{
		T *p_usuw = NULL;

		if (pointer != NULL && usuw == *pointer) { p_usuw = pointer; }
		else { p_usuw = find(usuw); }

		if (p_usuw != NULL)
		{
			// zapisanie elementow sasiadujacych
			T *next = p_usuw->nextElement;
			T *prev = p_usuw->prevElement;

			// aktualizacja elementow sasiadujacych u sasiadow usuwanego
			if (next != NULL) { next->prevElement = prev; }
			if (prev != NULL) { prev->nextElement = next; }

			// sprawdzenie czy usuwany element nie jest pierwszym
			if (p_usuw == head->nextElement) { head->nextElement = next; }

			// przestawienie pointera jesli to konieczne
			if (pointer == p_usuw) { pointer = head->nextElement; }

			// usuniecie elementu
			delete p_usuw;

			n--;

			return true;
		}

		return false;
	};
	
	/*
		Zwraca pierwszy napotkany element o zawartosci 'szuk'.
		Jesli nie istnieje on na liscie, funkcja zwraca NULL.
	*/
	T* find(T szuk)
	{
		T *bierz = head;

		while (bierz->nextElement != NULL)
		{
			bierz = bierz->nextElement;
			if (*bierz == szuk)
				return bierz;
		}

		return NULL;
	};
	
	/*
		Przenosi wskaznik na m-ty element listy.
	*/
	void reset(int m)
	{
		T *bierz = head;

		if (m <= n && m > 0)
		{
			for (int i = 0; i < m; i++)
			{
				bierz = bierz->nextElement;
			}

			pointer = bierz;
		}
	};
	
	/*
		Zwraca wartosc przechowywana w elemencie wskazywanym przez pointer,
		a nastepnie przesuwa pointer na nastepny element. Jesli na liscie nie
		ma zadnych elementow funkcja zwraca NULL.
	*/
	T* next()
	{
		T* temp;

		// przypadek I - brak elementow na liscie
		if (pointer == head || pointer == NULL)
		{
			return NULL;
		}
		// przypadek II - pointer to ostatni element
		else if (pointer->nextElement == NULL)
		{
			temp = pointer;
			pointer = NULL;

			return temp;
		}
		// przypadek III - element wewnatrz listy
		else
		{
			temp = pointer;
			pointer = pointer->nextElement;
			return temp;
		}
	};
	
	/*
		Kasuje zawartosc calej listy.
	*/
	void makenull()
	{
		T *bierz = head->nextElement;
		T *temp;

		while (bierz != NULL)
		{
			temp = bierz->nextElement;
			delete bierz;
			bierz = temp;

			n--;
		}

		head->nextElement = NULL;
		pointer = head;
	};
	
	/*
		Wypisuje na ekran zawartosc listy.
	*/
	void print()
	{
		T *bierz = head->nextElement;

		if (bierz != NULL)
		{
			std::cout << *bierz;

			while (bierz->nextElement != NULL)
			{
				bierz = bierz->nextElement;
				std::cout << ", " << *bierz;
			}
		}
	};

	/*
		Zwraca pierwszy element i usuwa go z listy.
	*/
	T* popFirst()
	{
		if (n == 0)
		{
			return NULL;
		}

		reset(1);

		T* first = new T();
		*first = *pointer;

		remove(*pointer);

		return first;
	};

	/*
		Zwraca ostatni element i usuwa go z listy.
	*/
	T* popLast()
	{
		if (n == 0)
		{
			return NULL;
		}

		reset(n);

		T* last = new T();
		*last = *pointer;

		remove(*last);

		return last;
	};
	
	// pomocnicze

	/*
		Zwraca rozmiar listy.
	*/
	int getSize() { return n; };

	/*
		Zwraca true jesli istnieje element dalszy niz ten wskazywany
		przez pointer. Jesli nie, zwraca false.
	*/
	bool hasNext()
	{
		if (pointer == NULL)
			return false;
		if (pointer->nextElement != NULL)
			return true;
		else
			return  false;
	};

};

