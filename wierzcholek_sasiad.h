#pragma once
#include <ostream>

class SasWierzcholek
{
public:
	SasWierzcholek();
	SasWierzcholek(SasWierzcholek &sw);
	SasWierzcholek(int nr_wierzch, int waga_kraw, int nr_kraw);

	~SasWierzcholek();

	int nr_wierzch;
	int waga_kraw; // waga krawedzi, ktora wierzcholek tworzy
	int nr_kraw;

	// operatory
	bool operator ==(const SasWierzcholek &sw);
	bool operator !=(const SasWierzcholek &sw);
	bool operator >(const SasWierzcholek &sw);
	bool operator <(const SasWierzcholek &sw);
	bool operator >=(const SasWierzcholek &sw);
	bool operator <=(const SasWierzcholek &sw);

	SasWierzcholek& operator =(const SasWierzcholek &sw);

	friend std::ostream & operator<< (std::ostream &wyjscie, const SasWierzcholek &sw);

	// elementy potrzebne na liscie
	SasWierzcholek* nextElement;
	SasWierzcholek* prevElement;
};

