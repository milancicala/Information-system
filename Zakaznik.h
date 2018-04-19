#pragma once

#include "Struktury.h"

#include <string>
using std::string;

#include <fstream>
using std::fstream;

class Zakaznik {
	string obchodnyNazov = "neplatny";
	ulong region = 0; //cislo regionu

public:
	Zakaznik() {}

	Zakaznik(string obchodnyNazov, ulong region)
		: obchodnyNazov(obchodnyNazov)
		, region(region) {}

	string getObchodnyNazov() const {
		return obchodnyNazov;
	}

	ulong getRegion() const {
		return region;
	}

	// vypis objekt Zakaznik do suboru
	// umozni napriklad nasledovny prikaz
	// file << zakaznik;
	friend fstream& operator<<(fstream&, const Zakaznik&);

	// nacitanie objektu Zakaznik zo suboru
	// umozni napriklad nasledovny prikaz
	// file >> zakaznik;
	friend fstream& operator>>(fstream&, Zakaznik&);
};