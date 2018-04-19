#pragma once

#include "structures\list\linked_list.h"
#include "Zakaznik.h"

class DatabazaZakaznikov {
	friend class Perzistencia;
	friend class DatabazaObjednavok;

	structures::LinkedList<Zakaznik*>* zoznam;

	// bod 5 a 11
	bool pridajAbecedne(const Zakaznik&);

public:

	/** Konstruktor */
	DatabazaZakaznikov();

	/** Kopirovaci constructor */
	DatabazaZakaznikov(const DatabazaZakaznikov&) = delete;

	/** Destructor */
	~DatabazaZakaznikov() noexcept;

	// bod 5
	// registracia noveho zakaznika
	bool Registruj();

	// bod 11
	// vypisanie zoznamu zakaznikov abecedne podla obchodneho nazvu
	void vypis() const;

	// bod 8
	// k zakaznikovi s danym (unikatnym) obchodnym nazvom
	// vrat region daneho zakaznika
	// vrat 0 ak zakaznik nebol najdeny
	ulong ziskajRegion(string nazovZakaznika) const;
};
