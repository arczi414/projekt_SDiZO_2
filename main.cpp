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
#include "pomiary.h"
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	manageMenu();
	
	//pomiarMST_MGraf("mst.txt");
	//pomiarMST_LGraf("mst.txt");
	
	//pomiarSPT_MGraf("spt.txt");
	//pomiarSPT_LGraf("spt.txt");

	//pomiarMaxFlow_MGraf("maxFlow.txt");
	//pomiarMaxFlow_LGraf("FordFulkersonMaxFlow.txt");

	//cout << "\n\nWcisnij dowolny klawisz, aby zakonczyc program...";
	_getch();
	return 0;
}

