#pragma once

#include "structures\list\linked_list.h"
#include "StrukturyPrePolotovary.h"
#include "Nakup.h"
#include "StrukturyPreVozidlo.h"
#include "DatabazaObjednavok.h"
class DatabazaObjednavok;
class Objednavka;
#include "DatabazaBiofarmarov.h"
class DatabazaBiofarmarov;
class Biofarmar;

class DatabazaNakupov {
	friend class Perzistencia;

	structures::LinkedList<Nakup*>* zoznam;

	float mnozstvoZemiakov = 0.4;
	float mnozstvoOleja = 0.2;
	float mnozstvoOchucovadla = 0.2;

public:

	// vrati true ak existuje dostatok polotovarov k pokrytiu danej objednavky
	// false inak
	bool existujeDostatokPolotovarov(
		const Objednavka&
	) const;

	void odpocitajMnozstvoTovaru(const Objednavka&);

	//bod 7
	//zabezpeci dostatok polotovarov k pokrytiu tyzdennej dodavky tovarov
	void zabezpecDostatokPolotovarov(
		const DatabazaObjednavok&,
		DatabazaBiofarmarov&);

	void pridaj(const Nakup& nakup); // bod 7

	// priemerna cena polotovaru daneho farmara za poslednych 30 dni
	// poznamka: v databaze su len udaje za poslednych 30 dni
	// toto bude (TODO) zabezpecene funkciou pre prechod na dalsi den
	float priemer(
		string obchodnyNazov,
		DruhyPolotovarov druh
	) const; // bod 2 a 7

	// vypis vsetkych nakupov od biofarmarov
	// NETREBA k splneniu zadania (ale uzitocne na otestovanie)
	void vypis();

	// bod 14
	void vypisNajBiofarmara()const;

	// bod 15
	// pomocna funkcia
	// vrati celkovu cenu nakupenych polotovarov (strata)
	float cenaPolotovarov() const;

	float getMnozstvoZemiakov() const {
		return mnozstvoZemiakov;
	}

	float getMnozstvOleja() const {
		return mnozstvoOleja;
	}

    float getMnozstvoOchucovadla() const {
		return mnozstvoOchucovadla;
	}

	/** Konstruktor */
	DatabazaNakupov();

	/** Kopirovaci constructor */
	DatabazaNakupov(const structures::LinkedList<Nakup*>*&);

	/** Destructor */
	~DatabazaNakupov() noexcept;
};