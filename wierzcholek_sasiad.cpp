#include "stdafx.h"
#include "wierzcholek_sasiad.h"


SasWierzcholek::SasWierzcholek() :
nextElement(NULL), prevElement(NULL), nr_wierzch(-1), waga_kraw(0), nr_kraw(-1)
{ }

SasWierzcholek::SasWierzcholek(SasWierzcholek &sw)
{
	waga_kraw = sw.waga_kraw;
	nr_wierzch = sw.nr_wierzch;
	nr_kraw = sw.nr_kraw;
	nextElement = sw.nextElement;
	prevElement = sw.prevElement;
}

SasWierzcholek::SasWierzcholek(int nr_wierzch, int waga_kraw, int nr_kraw) :
nextElement(NULL), prevElement(NULL), waga_kraw(waga_kraw), nr_wierzch(nr_wierzch), nr_kraw(nr_kraw)
{ }

SasWierzcholek::~SasWierzcholek()
{
	nextElement = NULL;
	prevElement = NULL;
}

bool SasWierzcholek::operator ==(const SasWierzcholek &sw)
{
	if (sw.nr_wierzch == nr_wierzch)
		return true;

	return false;
}

bool SasWierzcholek::operator !=(const SasWierzcholek &sw)
{
	if (sw.nr_wierzch == nr_wierzch)
		return false;

	return false;
}

bool SasWierzcholek::operator >(const SasWierzcholek &sw)
{
	if (waga_kraw > sw.waga_kraw)
		return true;

	return false;
}

bool SasWierzcholek::operator <(const SasWierzcholek &sw)
{
	if (waga_kraw < sw.waga_kraw)
		return true;

	return false;
}

bool SasWierzcholek::operator >=(const SasWierzcholek &sw)
{
	if (waga_kraw >= sw.waga_kraw)
		return true;

	return false;
}

bool SasWierzcholek::operator <=(const SasWierzcholek &sw)
{
	if (waga_kraw <= sw.waga_kraw)
		return true;

	return false;
}

SasWierzcholek& SasWierzcholek::operator =(const SasWierzcholek &sw)
{
	if (*this == sw)
	{
		return *this;
	}
	else
	{
		waga_kraw = sw.waga_kraw;
		nr_wierzch = sw.nr_wierzch;
		nr_kraw = sw.nr_kraw;

		nextElement = sw.nextElement;
		prevElement = sw.prevElement;
	}

	return *this;
}

std::ostream & operator<< (std::ostream &wyjscie, const SasWierzcholek &sw)
{
	return wyjscie << sw.waga_kraw;
}