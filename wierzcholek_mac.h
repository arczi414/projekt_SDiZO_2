#pragma once
#include <ostream>

class MWierzcholek
{
public:
	MWierzcholek();
	MWierzcholek(MWierzcholek &mw);
	MWierzcholek(int nr_wierzch);
	MWierzcholek(int koszt_dojscia, int nr_wierzch, int w_main);

	~MWierzcholek();

	int koszt_dojscia;
	int nr_wierzch;

	int w_main; // wierzcholek wzgledem ktorego rozpatrywany jest koszt dojscia

	// operatory
	bool operator ==(const MWierzcholek &mw);
	bool operator !=(const MWierzcholek &mw);
	bool operator >(const MWierzcholek &mw);
	bool operator <(const MWierzcholek &mw);
	bool operator >=(const MWierzcholek &mw);
	bool operator <=(const MWierzcholek &mw);

	MWierzcholek& operator =(const MWierzcholek &mw);

	friend std::ostream & operator<< (std::ostream &wyjscie, const MWierzcholek &mw);

	// elementy potrzebne na liscie
	MWierzcholek* nextElement;
	MWierzcholek* prevElement;
};

