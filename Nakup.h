#pragma once

#include "Struktury.h"
#include "StrukturyPrePolotovary.h"

#include <string>
using std::string;

#include <fstream>
using std::fstream;

class Nakup {
	friend class DatabazaNakupov;
	friend class Perzistencia;

	time_t datum = 0; //den predaja
	string obchodnyNazov = ""; //biofarmar (nakupca)
	DruhyPolotovarov druh; //typ polotovaru
	ulong mnozstvo = 0;
	ulong cena = 0;

public:
	Nakup() {}

	Nakup(
		time_t datum,
		string obchodnyNazov,
		DruhyPolotovarov druh,
		ulong mnozstvo,
		ulong cena
	);

	// vypis objekt Nakup do suboru
	// umozni napriklad nasledovny prikaz
	// file << nakup;
	friend fstream& operator<<(fstream&, const Nakup&);

	// nacitanie objektu Nakup zo suboru
	// umozni napriklad nasledovny prikaz
	// file >> nakup;
	friend fstream& operator>>(fstream&, Nakup&);
};