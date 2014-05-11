#include "stdafx.h"
#include "graf.h"
#include "graf_list.h"
#include "graf_mac.h"
#include "stoper.h"
#include <cstdlib>
#include <time.h>

using namespace std;

// M S T
void pomiarMST_MGraf(string plik)
{
	Stoper stPrim;
	Stoper stKrusk;

	string prim = "mGraf_prim_" + plik;
	string krusk = "mGraf_krusk_" + plik;
	stPrim.openFile(prim);
	stKrusk.openFile(krusk);
	srand(time(NULL));

	// graf
	Graf *graf = new MGraf();

	// dodawanie elementu
	int l_wierzch[] = { 10, 50, 100, 200, 350 };
	double gestosc[] = { 0.25, 0.5, 0.75, 0.99 };
	int iloscJedn = 10;	// liczba pomiarow pojedynczej operacji
	double sredniaPomiaru_prim, sredniaPomiaru_krusk;

	// dodawanie na poczatku
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sredniaPomiaru_prim = 0;
			sredniaPomiaru_krusk = 0;

			for (int k = 0; k < iloscJedn; k++)
			{
				cout << "\n\nPomiar " << k << " dla " << l_wierzch[i] << "w i gestosci " << gestosc[j] << "\n\n";

				graf->losujGraf(l_wierzch[i], gestosc[j], false, false);
				
				stPrim.start();
				graf->mstPrim();
				stPrim.stop();
				sredniaPomiaru_prim += stPrim.inMicrosec();

				cout << endl;

				stKrusk.start();
				graf->mstKruskal();
				stKrusk.stop();
				sredniaPomiaru_krusk += stKrusk.inMicrosec();
			}

			sredniaPomiaru_prim /= iloscJedn;
			stPrim.saveScore(sredniaPomiaru_prim, l_wierzch[i], gestosc[j]);

			sredniaPomiaru_krusk /= iloscJedn;
			stKrusk.saveScore(sredniaPomiaru_krusk, l_wierzch[i], gestosc[j]);
		}
	}

	if (graf != NULL)
	{
		delete graf;
	}

	stPrim.closeFile();
	stKrusk.closeFile();
	std::cout << endl;
}

void pomiarMST_LGraf(string plik)
{
	Stoper stPrim;
	Stoper stKrusk;

	string prim = "lGraf_prim_" + plik;
	string krusk = "lGraf_krusk_" + plik;
	stPrim.openFile(prim);
	stKrusk.openFile(krusk);
	srand(time(NULL));

	// graf
	Graf *graf = new LGraf();
	Graf *mst;

	// dodawanie elementu
	int l_wierzch[] = { 10, 50, 100, 200, 350 };
	double gestosc[] = { 0.25, 0.5, 0.75, 0.99 };
	int iloscJedn = 10;	// liczba pomiarow pojedynczej operacji
	double sredniaPomiaru_prim, sredniaPomiaru_krusk;

	// dodawanie na poczatku
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sredniaPomiaru_prim = 0;
			sredniaPomiaru_krusk = 0;

			for (int k = 0; k < iloscJedn; k++)
			{
				cout << "\n\nPomiar " << k << " dla " << l_wierzch[i] << "w i gestosci " << gestosc[j] << "\n\n";

				graf->losujGraf(l_wierzch[i], gestosc[j], false, false);

				stPrim.start();
				mst = graf->mstPrim();
				stPrim.stop();
				if (mst != 0) { delete mst; }
				sredniaPomiaru_prim += stPrim.inMicrosec();

				cout << endl;

				stKrusk.start();
				mst = graf->mstKruskal();
				stKrusk.stop();
				if (mst != 0) { delete mst; }
				sredniaPomiaru_krusk += stKrusk.inMicrosec();
			}

			sredniaPomiaru_prim /= iloscJedn;
			stPrim.saveScore(sredniaPomiaru_prim, l_wierzch[i], gestosc[j]);

			sredniaPomiaru_krusk /= iloscJedn;
			stKrusk.saveScore(sredniaPomiaru_krusk, l_wierzch[i], gestosc[j]);
		}
	}

	if (graf != NULL)
	{
		delete graf;
	}

	stPrim.closeFile();
	stKrusk.closeFile();
	std::cout << endl;
}

// S P T

void pomiarSPT_MGraf(string plik)
{
	srand(time(NULL));

	Stoper stDijkstra;
	Stoper stBellFord;

	string dijkstra = "mGraf_dijkstra_" + plik;
	string bellford = "mGraf_bellford_" + plik;
	stDijkstra.openFile(dijkstra);
	stBellFord.openFile(bellford);
	srand(time(NULL));

	// graf
	Graf *graf = new MGraf();

	int **koszty = NULL, *koszty1 = NULL, *poprz = NULL;

	// dodawanie elementu
	int l_wierzch[] = { 10, 50, 100, 200, 280, 350 };
	double gestosc[] = { 0.25, 0.5, 0.75, 0.99 };
	int iloscJedn = 10;	// liczba pomiarow pojedynczej operacji
	double sredniaPomiaru_dijkstra, sredniaPomiaru_bellford;

	// dodawanie na poczatku
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sredniaPomiaru_dijkstra = 0;
			sredniaPomiaru_bellford = 0;

			for (int k = 0; k < iloscJedn; k++)
			{
				cout << "\n\nPomiar " << k << " dla " << l_wierzch[i] << "w i gestosci " << gestosc[j] << "\n\n";

				int w = rand() % l_wierzch[i]; // wierzcholek odwolania
				graf->losujGraf(l_wierzch[i], gestosc[j], false, false);

				stDijkstra.start();
				graf->sptDijkstra(w, koszty1, poprz);
				stDijkstra.stop();
				if (poprz != NULL) { delete[] poprz; poprz = NULL; }
				if (koszty1 != NULL) { delete[] koszty1; koszty1 = NULL; }
				sredniaPomiaru_dijkstra += stDijkstra.inMicrosec();

				stBellFord.start();
				graf->sptBellmanFord(w, koszty, poprz);
				stBellFord.stop();
				if (poprz != 0) { delete poprz; poprz = NULL; }
				if (koszty != NULL)
				{
					for (int a = 0; a < graf->getNumOfVertices(); a++)
					{
						if (koszty[a] != NULL)
						{
							delete koszty[a];
						}
					}

					delete[] koszty;
					koszty = NULL;
				}

				sredniaPomiaru_bellford += stBellFord.inMicrosec();
			}

			sredniaPomiaru_dijkstra /= iloscJedn;
			stDijkstra.saveScore(sredniaPomiaru_dijkstra, l_wierzch[i], gestosc[j]);

			sredniaPomiaru_bellford /= iloscJedn;
			stBellFord.saveScore(sredniaPomiaru_bellford, l_wierzch[i], gestosc[j]);
		}
	}

	if (graf != NULL)
	{
		delete graf;
	}

	stDijkstra.closeFile();
	stBellFord.closeFile();
	std::cout << endl;
}

void pomiarSPT_LGraf(string plik)
{
	srand(time(NULL));

	Stoper stDijkstra;
	Stoper stBellFord;

	string dijkstra = "lGraf_dijkstra_" + plik;
	string bellford = "lGraf_bellford_" + plik;
	stDijkstra.openFile(dijkstra);
	stBellFord.openFile(bellford);
	srand(time(NULL));

	// graf
	Graf *graf = new LGraf();

	int **koszty = NULL, *koszty1 = NULL, *poprz = NULL;

	// dodawanie elementu
	int l_wierzch[] = { 10, 50, 100, 200, 280, 350 };
	double gestosc[] = { 0.25, 0.5, 0.75, 0.99 };
	int iloscJedn = 10;	// liczba pomiarow pojedynczej operacji
	double sredniaPomiaru_dijkstra, sredniaPomiaru_bellford;

	// dodawanie na poczatku
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sredniaPomiaru_dijkstra = 0;
			sredniaPomiaru_bellford = 0;

			for (int k = 0; k < iloscJedn; k++)
			{
				cout << "\n\nPomiar " << k << " dla " << l_wierzch[i] << "w i gestosci " << gestosc[j] << "\n\n";

				int w = rand() % l_wierzch[i]; // wierzcholek odwolania
				graf->losujGraf(l_wierzch[i], gestosc[j], false, false);

				stDijkstra.start();
				graf->sptDijkstra(w, koszty1, poprz);
				stDijkstra.stop();
				if (poprz != NULL) { delete[] poprz; poprz = NULL; }
				if (koszty1 != NULL) { delete[] koszty1; koszty1 = NULL; }
				sredniaPomiaru_dijkstra += stDijkstra.inMicrosec();

				stBellFord.start();
				graf->sptBellmanFord(w, koszty, poprz);
				stBellFord.stop();
				if (poprz != 0) { delete poprz; poprz = NULL; }
				if (koszty != NULL)
				{
					for (int a = 0; a < graf->getNumOfVertices(); a++)
					{
						if (koszty[a] != NULL)
						{
							delete koszty[a];
						}
					}

					delete[] koszty;
					koszty = NULL;
				}

				sredniaPomiaru_bellford += stBellFord.inMicrosec();
			}

			sredniaPomiaru_dijkstra /= iloscJedn;
			stDijkstra.saveScore(sredniaPomiaru_dijkstra, l_wierzch[i], gestosc[j]);

			sredniaPomiaru_bellford /= iloscJedn;
			stBellFord.saveScore(sredniaPomiaru_bellford, l_wierzch[i], gestosc[j]);
		}
	}

	if (graf != NULL)
	{
		delete graf;
	}

	stDijkstra.closeFile();
	stBellFord.closeFile();
	std::cout << endl;
}

// M A X   F L O W

void pomiarMaxFlow_MGraf(string plik)
{
	srand(time(NULL));

	Stoper stDFS;
	Stoper stBFS;

	string dfs = "mGraf_FordFulkersonDFS_" + plik;
	string bfs = "mGraf_FordFulkersonBFS_" + plik;
	stDFS.openFile(dfs);
	stBFS.openFile(bfs);
	srand(time(NULL));

	// graf
	Graf *graf = new MGraf();

	// dodawanie elementu
	int l_wierzch[] = { 10, 50, 100, 200, 280, 350 };
	double gestosc[] = { 0.25, 0.5, 0.75, 0.99 };
	int iloscJedn = 10;	// liczba pomiarow pojedynczej operacji
	double sredniaPomiaru_dfs, sredniaPomiaru_bfs;

	// dodawanie na poczatku
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sredniaPomiaru_dfs = 0;
			sredniaPomiaru_bfs = 0;

			for (int k = 0; k < iloscJedn; k++)
			{
				cout << "\n\nPomiar " << k << " dla " << l_wierzch[i] << "w i gestosci " << gestosc[j] << "\n\n";

				int src = rand() % l_wierzch[i]; // wierzcholek zrodlo
				int snk = rand() % l_wierzch[i]; // wierzcholek zlew

				while (src == snk)
				{
					snk = rand() % l_wierzch[i];
				}

				graf->losujGraf(l_wierzch[i], gestosc[j], false);

				stBFS.start();
				graf->findMaxflowFordFulkerson(src, snk, 'B');
				stBFS.stop();
				sredniaPomiaru_bfs += stBFS.inMicrosec();

				stDFS.start();
				graf->findMaxflowFordFulkerson(src, snk, 'D');
				stDFS.stop();
				sredniaPomiaru_dfs += stDFS.inMicrosec();
			}

			sredniaPomiaru_dfs /= iloscJedn;
			stDFS.saveScore(sredniaPomiaru_dfs, l_wierzch[i], gestosc[j]);

			sredniaPomiaru_bfs /= iloscJedn;
			stBFS.saveScore(sredniaPomiaru_bfs, l_wierzch[i], gestosc[j]);
		}
	}

	if (graf != NULL)
	{
		delete graf;
	}

	stDFS.closeFile();
	stBFS.closeFile();
	std::cout << endl;
}