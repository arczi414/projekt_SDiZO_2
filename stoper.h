#include <fstream>
#include <Windows.h>

#ifndef _STOPER_H_
#define _STOPER_H_

/*
	Naglowek zawiera deklaracje funkcji ulatwiajacych
	przeprowadzanie pomiarow na strukturach.
*/

class Stoper
{
private:
	LARGE_INTEGER startTimer();
	LARGE_INTEGER endTimer();

	LARGE_INTEGER performanceCountStart;
	LARGE_INTEGER performanceCountEnd;

	LARGE_INTEGER freq;
	double tm;

	// oblsuga plikow
	std::ofstream plik;

public:
	void start();
	void stop();
	double inMicrosec();
	double inMilisec();
	double inSec();
	bool openFile(std::string);
	bool closeFile();
	bool saveScore(double score, unsigned int n, double gestosc);

};

#endif