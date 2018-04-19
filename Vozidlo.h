#ifndef VOZIDLO_H
#define VOZIDLO_H

#include "structures\stack\explicit_stack.h"
#include "StrukturyPreVozidlo.h"
#include "Objednavka.h"
class Objednavka;

#include <string>
using std::string;

#include <fstream>
using std::fstream;

extern time_t dnesnyDatum;

// bod 3
class Vozidlo {
	friend class DatabazaVozidiel;

protected:
	structures::ExplicitStack<Objednavka*> objednavky;
	string SPZ = ""; //statna poznavacia znacka	
	ulong mnozstvo = 0; //mnozstvo naplneneho tovaru v tonach (nanajvys rovne NOSNOSTi)
	ulong NOSNOST = 0; //v tonach
	ulong KOEFICIENT = 0; //prevadzkove naklady na 1 region v EUR
	TypTovaru druh = TypTovaru::hranolceky; //druh vozidla podla rozvazaneho tovaru
	ulong prevadzkoveNaklady = 0; //celkove prevadzkove naklady od zaradenia do evidencie
	time_t datumZaradenia = dnesnyDatum; //datum zaradenia do evidencie

public:
	Vozidlo() {}

	Vozidlo(string SPZ, ulong nosnost, ulong koeficient, TypTovaru druh)
		:SPZ(SPZ)
		, NOSNOST(nosnost)
		, KOEFICIENT(koeficient)
		, druh(druh) {};

	//aktualizuje celkove prevadzkove naklady
	//na zaklade dnesnych prevadzkovych nakladov
	//ziskanych z poctu regionov
	//ktore vozidlo dnesny den navstivilo
	void updatePrevadzkoveNaklady(ulong pocetRegionov) {
		ulong dnesnePrevadzkoveNaklady =
			KOEFICIENT * pocetRegionov;

		prevadzkoveNaklady += dnesnePrevadzkoveNaklady;
	}

	//bod 4
	//vrati celkove prevadzkove naklady od datumu zaradenia do evidencie
	ulong getPrevadzkoveNaklady() const {
		return prevadzkoveNaklady;
	}

	string getSPZ() const {
		return SPZ;
	}

	TypTovaru getDruh() const {
		return druh;
	}

	time_t getDatumZaradenia() const {
		return datumZaradenia;
	}

	ulong getNosnost() const {
		return NOSNOST;
	}

	ulong getMnozstvo() const {
		return mnozstvo;
	}

	// vypis objektu Vozidlo do suboru
	// umozni napriklad nasledovny prikaz
	// file << vozidlo;
	friend fstream& operator<<(fstream&, const Vozidlo&);

	// nacitanie objektu Vozidlo zo suboru
	// umozni napriklad nasledovny prikaz
	// file >> vozidlo;
	friend fstream& operator>>(fstream&, Vozidlo&);

	bool naplnVozidlo(Objednavka*);
	void spracujObjednavku(Objednavka*);

};

class VozidloHranolceky : public Vozidlo {
public:

	VozidloHranolceky(string SPZ)
		: Vozidlo(SPZ, 5, 100, TypTovaru::hranolceky) {};
};


class VozidloLupienky : public Vozidlo {
public:

	VozidloLupienky(string SPZ)
		: Vozidlo(SPZ, 2, 70, TypTovaru::lupienky) {};
};

#endif
