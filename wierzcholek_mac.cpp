#include "stdafx.h"
#include "wierzcholek_mac.h"


MWierzcholek::MWierzcholek() :
nextElement(NULL), prevElement(NULL), koszt_dojscia(0), nr_wierzch(-1), w_main(-1)
{ }

MWierzcholek::MWierzcholek(MWierzcholek &mw)
{
	koszt_dojscia = mw.koszt_dojscia;
	nr_wierzch = mw.nr_wierzch;
	w_main = mw.w_main;
	nextElement = mw.nextElement;
	prevElement = mw.prevElement;
}

MWierzcholek::MWierzcholek(int nr_wierzch) :
nextElement(NULL), prevElement(NULL), koszt_dojscia(-1), nr_wierzch(nr_wierzch), w_main(-1)
{ }

MWierzcholek::MWierzcholek(int koszt_dojscia, int nr_wierzch, int w_main) :
nextElement(NULL), prevElement(NULL), koszt_dojscia(koszt_dojscia), nr_wierzch(nr_wierzch), w_main(w_main)
{ }

MWierzcholek::~MWierzcholek()
{
	nextElement = NULL;
	prevElement = NULL;
}

bool MWierzcholek::operator ==(const MWierzcholek &mw)
{
	if (mw.koszt_dojscia == koszt_dojscia && mw.nr_wierzch == nr_wierzch)
		return true;

	return false;
}

bool MWierzcholek::operator !=(const MWierzcholek &mw)
{
	if (mw.koszt_dojscia == koszt_dojscia && mw.nr_wierzch == nr_wierzch)
		return false;

	return false;
}

bool MWierzcholek::operator >(const MWierzcholek &mw)
{
	if (koszt_dojscia > mw.koszt_dojscia)
		return true;

	return false;
}

bool MWierzcholek::operator <(const MWierzcholek &mw)
{
	if (koszt_dojscia < mw.koszt_dojscia)
		return true;

	return false;
}

bool MWierzcholek::operator >=(const MWierzcholek &mw)
{
	if (koszt_dojscia >= mw.koszt_dojscia)
		return true;

	return false;
}

bool MWierzcholek::operator <=(const MWierzcholek &mw)
{
	if (koszt_dojscia <= mw.koszt_dojscia)
		return true;

	return false;
}

MWierzcholek& MWierzcholek::operator =(const MWierzcholek &mw)
{
	if (*this == mw)
	{
		return *this;
	}
	else
	{
		koszt_dojscia = mw.koszt_dojscia;
		nr_wierzch = mw.nr_wierzch;
		w_main = mw.w_main;

		nextElement = mw.nextElement;
		prevElement = mw.prevElement;
	}

	return *this;
}

std::ostream & operator<< (std::ostream &wyjscie, const MWierzcholek &mw)
{
	return wyjscie << mw.koszt_dojscia;
}