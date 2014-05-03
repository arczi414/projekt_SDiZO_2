#pragma once
#include <ostream>

class Integer
{
public:
	Integer(int wartosc = 0)
	{
		this->wartosc = wartosc;
		nextElement = prevElement = NULL;
	};

	Integer(Integer &mw)
	{
		wartosc = mw.wartosc;
		nextElement = mw.nextElement;
		prevElement = mw.prevElement;
	};

	~Integer()
	{
		nextElement = prevElement = NULL;
	};

	int wartosc;

	// operatory
	bool operator ==(const Integer &mw)
	{
		if (wartosc == mw.wartosc)
			return true;

		return false;
	};

	bool operator !=(const Integer &mw)
	{
		if (wartosc == mw.wartosc)
			return false;

		return true;
	};

	bool operator >(const Integer &mw)
	{
		if (wartosc > mw.wartosc)
			return true;

		return false;
	};

	bool operator <(const Integer &mw)
	{
		if (wartosc < mw.wartosc)
			return true;

		return false;
	};

	bool operator >=(const Integer &mw)
	{
		if (wartosc >= mw.wartosc)
			return true;

		return false;
	};

	bool operator <=(const Integer &mw)
	{
		if (wartosc <= mw.wartosc)
			return true;

		return false;
	};

	Integer& operator =(const Integer &mw)
	{
		if (*this == mw)
		{
			return *this;
		}
		else
		{
			wartosc = mw.wartosc;
			nextElement = mw.nextElement;
			prevElement = mw.prevElement;
		}

		return *this;
	};

	friend std::ostream & operator<< (std::ostream &wyjscie, const Integer &mw)
	{
		return wyjscie << mw.wartosc;
	};

	// elementy potrzebne na liscie
	Integer* nextElement;
	Integer* prevElement;
};