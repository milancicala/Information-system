#pragma once

#include "StrukturyPreVozidlo.h"
#include "DatabazaZakaznikov.h"
#include "DatabazaNakupov.h"

#include <string>
using std::string;

#include <fstream>
using std::fstream;

class Objednavka {

	friend class Vozidlo;

	time_t datumDorucenia; //pozadovany datum dorucenia
	time_t datumZaevidovania;

	TypTovaru typTovaru;
	ulong mnozstvo; //v jednotkach odpovedajucich danemu tovaru (kg/l/g)
	ulong cena; //jednotkova predajna cena

				//unikatny obchodny nazov zakaznika
				//dalsie udaje (region) v pripade potreby dohladatelne v databaze
	string zakaznik;

	bool zamietnuta = false; // bod 6 a 13
	bool zrusena = false; // bod 8 a 13
	bool zrealizovana = false; // bod 10 (a 12, namiesto !zamietnuta&&!zrusena)

public:

	Objednavka(
		time_t datumDorucenia,
		time_t datumZaevidovania,
		TypTovaru typTovaru,
		ulong mnozstvo,
		ulong cena,
		string zakaznik)
		: datumDorucenia(datumDorucenia)
		, datumZaevidovania(datumZaevidovania)
		, typTovaru(typTovaru)
		, mnozstvo(mnozstvo)
		, cena(cena)
		, zakaznik(zakaznik) {}

	// vytvori testovaci objekt triedy objednavka
	Objednavka()
		: datumDorucenia(0)
		, datumZaevidovania(0)
		, typTovaru(TypTovaru::hranolceky)
		, mnozstvo(0)
		, cena(0)
		, zakaznik("Jozef Mak") {}

	time_t getDatumZaevidovania() const {
		return datumZaevidovania;
	}

	time_t getDatumDorucenia() const {
		return datumDorucenia;
	}

	TypTovaru getTypTovaru() const {
		return typTovaru;
	}

	ulong getMnozstvo() const {
		return mnozstvo;
	}

	ulong getCena() const {
		return cena;
	}

	string getZakaznik() const {
		return zakaznik;
	}

	bool getZamietnuta() const {
		return zamietnuta;
	}

	void setZamietnuta() {
		zamietnuta = true;
	}

	bool getZrusena() const {
		return zrusena;
	}

	void setZrusena() {
		zrusena = true;
	}

	bool getZrealizovana() const {
		return zrealizovana;
	}

	// vypise objednavku
	// skontroluje, ci je zakaznik v databaze
	// z databazy zakaznikov zisti aktualne cislo regionu
	// AK bude objednavka zrusena z dovodu nedostatku polotovarov
	// TAK bude zavolane this->setZrusena()
	// ...a preto nie je const
	void vypis(const DatabazaZakaznikov&, const DatabazaNakupov&);

	// vypis objekt Objednavka do suboru
	// umozni napriklad nasledovny prikaz
	// file << objednavka;
	friend fstream& operator<<(fstream&, const Objednavka&);

	// nacitanie objektu Objednavka zo suboru
	// umozni napriklad nasledovny prikaz
	// file >> objednavka;
	friend fstream& operator>>(fstream&, Objednavka&);
};