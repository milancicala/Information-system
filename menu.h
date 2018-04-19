
#ifndef MENU_H
#define MENU_H

#include "DatabazaBiofarmarov.h"
#include "DatabazaVozidiel.h"
#include "DatabazaZakaznikov.h"
#include "DatabazaObjednavok.h"

#include "Perzistencia.h"

class Menu {
	DatabazaBiofarmarov& dbBiofarmarov;
	DatabazaNakupov& dbNakupov;
	DatabazaVozidiel& dbVozidiel;
	DatabazaZakaznikov& dbZakaznikov;
	DatabazaObjednavok& dbObjednavok;

	Perzistencia persist;

public:

	Menu(DatabazaBiofarmarov& dbBiofarmarov,
		DatabazaNakupov& dbNakupov,
		DatabazaVozidiel& dbVozidiel,
		DatabazaZakaznikov& dbZakaznikov,
		DatabazaObjednavok& dbObjednavok)
		: dbBiofarmarov(dbBiofarmarov)
		, dbNakupov(dbNakupov)
		, dbVozidiel(dbVozidiel)
		, dbZakaznikov(dbZakaznikov)
		, dbObjednavok(dbObjednavok)
	{
	}

	ulong ziskajVolbu();

	void mHlavne();
	void volbaHlavne(ulong);

	void mBiofarmar();
	void volbaBiofarmar(ulong);

	void mVozidlo();
	void volbaVozidlo(ulong);

	void mZakaznik();
	void volbaZakaznik(ulong);

	void mObjednavka();
	void volbaObjednavka(ulong);
};

#endif