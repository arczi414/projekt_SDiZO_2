#include "stdafx.h"
#include "menu.h"
#include "graf_mac.h"
#include "graf_list.h"
#include "krawedz_mac.h"
#include "wierzcholek_mac.h"
#include "zbiory_rozlaczne.h"
#include "drzewoCzerCzar.h"
#include "lista.h"
#include "kopiec.h"
#include "list_int.h"
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vld.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Graf *g = new MGraf();
	g->losujGraf(50, 1);
	
	int **temp = NULL, *temp1 = NULL, *temp2 = NULL;

	
	g->sptBellmanFord(10, temp, temp1);
	
	for(int i = 0; i < g->getNumOfVertices(); i++)
		delete temp[i];
	delete temp;
	delete temp1;

	temp = NULL;
	temp1 = NULL;

	g->sptDijkstra(10, temp1, temp2);
	delete temp1;
	delete temp2;

	delete g;

	cout << "\n\nWcisnij dowolny klawisz, aby zakonczyc program...";
	_getch();
	return 0;
}

