#pragma once

#include "structures\list\linked_list.h"
#include "Objednavka.h"
class Objednavka;

// forward declaration required
// to solve circular dependency issue
// with class DatabazaVozidiel
// see: https://stackoverflow.com/a/23283140
// Tato dopredna deklaracia je potrebna, aby mohla byt DatabazaVozidiel
// predana ako odkazom/referenciou. Druha moznost je predat ju cez ukazatel.
#include "DatabazaVozidiel.h"
class DatabazaVozidiel;

#include "DatabazaZakaznikov.h"
class DatabazaZakaznikov;
#include "DatabazaNakupov.h"

class DatabazaObjednavok {
	friend class Perzistencia;
	friend class DatabazaVozidiel;

	structures::LinkedList<Objednavka*>* zoznam;


	void pridajPodlaDataRealizacie(const Objednavka&);

public:

	void vypis(const DatabazaZakaznikov&, const DatabazaNakupov&) const;

	// bod 6
	// ziska od zakaznika udaje potrebne k vytvoreniu objednavky
	// vytvori objednavku
	// a preda ju vhodnej funkcii k zaevidovaniu do zoznamu
	bool pridaj(const DatabazaVozidiel&);

	// bod 7
	// vrati mnozstvo tovaru, ktore ma byt dodane do 7 dni
	ulong kDodaniuDoTyzdna(TypTovaru) const;

	//funckia, zrusi zajtrajsie objednavky, z dovodu nedostatku polotovarov
	void zrusObjednavkyPreNedostatokPolotovarov(DatabazaNakupov&);

	// bod 12
	void vypisZrealizovane(time_t od, time_t po, const DatabazaZakaznikov&, const DatabazaNakupov&) const;

	// bod 13
	void vypisNezrealizovane(time_t od, time_t po, const DatabazaZakaznikov & dbZakaznikov, const DatabazaNakupov & dbNakupov) const;

	void vypisPocetHmotnostTrzbyZrusenychAzamietnutych(ulong region, time_t od, time_t po, const DatabazaZakaznikov& dbZakaznikov) const;

	// bod 8
	void vypisNaZrealizovanieZajtra(const DatabazaZakaznikov&, const DatabazaNakupov&) const;

	// bod 15
	ulong zisk() const;

	/** Konstruktor */
	DatabazaObjednavok();

	/** Kopirovaci constructor */
	DatabazaObjednavok(const structures::LinkedList<Objednavka*>*&);

	/** Destructor */
	~DatabazaObjednavok() noexcept;
};
