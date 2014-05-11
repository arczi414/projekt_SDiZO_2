#include "stdafx.h"
#include "stoper.h"

LARGE_INTEGER Stoper::startTimer()
{
	LARGE_INTEGER start;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return start;
}

LARGE_INTEGER Stoper::endTimer()
{
	LARGE_INTEGER stop;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return stop;
}

void Stoper::start()
{
	QueryPerformanceFrequency(&freq);
	performanceCountStart = startTimer();
}

void Stoper::stop()
{
	performanceCountEnd = endTimer();
	tm = performanceCountEnd.QuadPart - performanceCountStart.QuadPart;
}

double Stoper::inMicrosec()
{
	return (tm / freq.QuadPart) * 1000000;
}

double Stoper::inMilisec()
{
	return ( tm / freq.QuadPart) * 1000;
}

double Stoper::inSec()
{
	return tm / freq.QuadPart;
}

bool Stoper::openFile(std::string nazwa_pliku)
{
	plik.open(nazwa_pliku);
	if (plik)
		return true;

	return false;
}

bool Stoper::closeFile()
{
	plik.close();

	return true;
}

bool Stoper::saveScore(double score, unsigned int n, double gestosc)
{
	plik << score << "; " << n << "; " << gestosc << std::endl;
	return true;
}