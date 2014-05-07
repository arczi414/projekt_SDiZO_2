#include "stdafx.h"
#include "graf_list.h"

LGraf::LGraf()
{}

LGraf::LGraf(const LGraf& mg)
{}

LGraf* LGraf::Clone()
{
	return NULL;
}

LGraf* LGraf::Create()
{
	return NULL;
}

LGraf::~LGraf()
{}

LGraf& LGraf::operator =(const LGraf &mg)
{
	LGraf g;
	return g;
}

bool LGraf::hasMax(int n, bool podwojne_kraw)
{
	return false;
}

char LGraf::hasConnection(int n1, int n2)
{
	return 0;
}

void LGraf::losujGraf(int n, float gestosc, bool ujemne_wagi, bool podwojne_kraw)
{}

void LGraf::pokazGraf()
{}

bool LGraf::dodajKraw(int start, int koniec, int waga)
{
	return false;
}

bool LGraf::usunKraw(int k)
{
	return false;
}

int LGraf::dodajWierzch()
{
	return 0;
}

bool LGraf::znajdzKrawedz(int start, int end)
{
	return false;
}

int LGraf::getIndexOfEdge(int start, int end)
{
	return 0;
}

int LGraf::getStart(int k)
{
	return 0;
}

int LGraf::getEnd(int k)
{
	return 0;
}

int* LGraf::getAvailableEdges(int w, bool skierowany)
{
	return NULL;
}

MWierzcholek* LGraf::getAvailableVertices(int w, bool skierowany)
{
	return NULL;
}

int* LGraf::getWeight(int k)
{
	return NULL;
}

bool LGraf::setWeight(int k, int waga)
{
	return false;
}