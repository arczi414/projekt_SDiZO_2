#pragma once
#include <ostream>

class MKrawedz
{
public:
	MKrawedz();
	MKrawedz(MKrawedz &mk);
	MKrawedz(int waga, int nr_kraw);

	~MKrawedz();

	int waga;
	int nr_kraw;

	// operatory
	bool operator ==(const MKrawedz &mk);
	bool operator !=(const MKrawedz &mk);
	bool operator >(const MKrawedz &mk);
	bool operator <(const MKrawedz &mk);
	bool operator >=(const MKrawedz &mk);
	bool operator <=(const MKrawedz &mk);

	MKrawedz& operator =(const MKrawedz &mk);

	friend std::ostream & operator<< (std::ostream &wyjscie, const MKrawedz &mk);

	// elementy potrzebne na liscie
	MKrawedz* nextElement;
	MKrawedz* prevElement;
};

