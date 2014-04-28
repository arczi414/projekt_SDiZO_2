#include "stdafx.h"
#include "krawedz_mac.h"


MKrawedz::MKrawedz() :
nextElement(NULL), prevElement(NULL), waga(0), nr_kraw(-1)
{ }

MKrawedz::MKrawedz(MKrawedz &mk)
{
	waga = mk.waga;
	nr_kraw = mk.nr_kraw;
	nextElement = mk.nextElement;
	prevElement = mk.prevElement;
}

MKrawedz::MKrawedz(int waga, int nr_kraw) :
nextElement(NULL), prevElement(NULL), waga(waga), nr_kraw(nr_kraw)
{ }

MKrawedz::~MKrawedz()
{
	nextElement = NULL;
	prevElement = NULL;
}

bool MKrawedz::operator ==(const MKrawedz &mk)
{
	if (mk.waga == waga && mk.nr_kraw == nr_kraw)
		return true;

	return false;
}

bool MKrawedz::operator >(const MKrawedz &mk)
{
	if (waga > mk.waga)
		return true;

	return false;
}

bool MKrawedz::operator <(const MKrawedz &mk)
{
	if (waga < mk.waga)
		return true;

	return false;
}

bool MKrawedz::operator >=(const MKrawedz &mk)
{
	if (waga >= mk.waga)
		return true;

	return false;
}

bool MKrawedz::operator <=(const MKrawedz &mk)
{
	if (waga <= mk.waga)
		return true;

	return false;
}

MKrawedz& MKrawedz::operator =(const MKrawedz &mk)
{
	if (*this == mk)
	{
		return *this;
	}
	else
	{
		waga = mk.waga;
		nr_kraw = mk.nr_kraw;

		nextElement = mk.nextElement;
		prevElement = mk.prevElement;
	}

	return *this;
}

std::ostream & operator<< (std::ostream &wyjscie, const MKrawedz &mk)
{
	return wyjscie << mk.waga;
}