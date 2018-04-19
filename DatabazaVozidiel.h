#ifndef DATABAZAVOZIDIEL_H
#define DATABAZAVOZIDIEL_H

#include "Vozidlo.h"
class Vozidlo;
#include "structures/list/linked_list.h"
#include "StrukturyPreVozidlo.h"
#include "DatabazaObjednavok.h"
class DatabazaObjednavok;

class DatabazaVozidiel {
	friend class Perzistencia;

	structures::LinkedList<Vozidlo*>* zoznam;

	void pridajChronologicky(Vozidlo*); //bod 3

public:
	/** Konstruktor */
	DatabazaVozidiel();

	/** Kopirovaci constructor */
	DatabazaVozidiel(const DatabazaVozidiel&) = delete;

	/** Destructor */
	~DatabazaVozidiel() noexcept;

	void pridaj(); // bod 3
	void vypis() const; //bod 4

	// bod 6
	// vrati true, ak je kapacita vozidiel dostacujuca pre rozvoz objednavok v dany den
	bool volnaKapacita(TypTovaru, const DatabazaObjednavok&, time_t datumDorucenia, ulong mnozstvo) const;

	// bod 9
	void naplnVozidla(const DatabazaObjednavok&);

	// bod 10
	void odovzdajTovar(const DatabazaZakaznikov &);

	// bod 15
	// pomocna funkcia
	// vrati prevadzkove naklady vozidiel
	// teda stratu spolocnosti
	ulong prevadzkoveNaklady() const;
};

#endif