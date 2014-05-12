#ifndef _POMAIRY_H_
#define _POMIARY_H_

#include <string>

/*
	Naglowek zawiera deklaracje funkcji pomiarowych.
*/

void pomiarMST_MGraf(std::string plik);
void pomiarMST_LGraf(std::string plik);

void pomiarSPT_MGraf(std::string plik);
void pomiarSPT_LGraf(std::string plik);

void pomiarMaxFlow_MGraf(std::string plik);
void pomiarMaxFlow_LGraf(std::string plik);

#endif