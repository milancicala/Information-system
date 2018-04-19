#pragma once

#include "DatabazaObjednavok.h"
#include "DatabazaVozidiel.h"
#include "DatabazaZakaznikov.h"
#include "DatabazaBiofarmarov.h"
#include "DatabazaNakupov.h"

//dopredna deklaracia
class Objednavka;

#include <fstream>
using std::fstream;

class Perzistencia {

	fstream objednavky;
	fstream zakaznici;
	fstream vozidla;
	fstream biofarmari;
	fstream nakupy;

	bool asociujSubor(const string&, fstream&);

public:

	Perzistencia();
	~Perzistencia();

	void ulozObjednavku(const Objednavka&);
	bool nacitajObjednavku(DatabazaObjednavok&, const DatabazaVozidiel&);

	void ulozZakaznika(const Zakaznik&);
	bool nacitajZakaznika(DatabazaZakaznikov&);

	void ulozVozidlo(const Vozidlo&);
	bool nacitajVozidlo(DatabazaVozidiel&);

	void ulozBiofarmara(const Biofarmar&);
	bool nacitajBiofarmara(DatabazaBiofarmarov&);

	void ulozNakup(const Nakup&);
	bool nacitajNakup(DatabazaNakupov&);

	// ulozi aktualny stav evidencie z programu do suboru
	void uloz(
		DatabazaObjednavok&,
		DatabazaZakaznikov&,
		DatabazaVozidiel&,
		DatabazaBiofarmarov&,
		DatabazaNakupov&
	);

	// nacita aktualny stav evidencie zo suboru do programu
	void nacitaj(
		DatabazaObjednavok&,
		DatabazaZakaznikov&,
		DatabazaVozidiel&,
		DatabazaBiofarmarov&,
		DatabazaNakupov&
	);

	//vrat sa na zaciatok kazdeho zo suborov
	void vratSaNaZaciatok();
};

