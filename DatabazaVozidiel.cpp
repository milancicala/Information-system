#include "structures\list\linked_list.h"
#include "DatabazaVozidiel.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

void DatabazaVozidiel::pridajChronologicky(Vozidlo* vkladane) {

	string SPZVlozeneho, SPZVkladaneho;
	// pokial vkladany datum je neskorsi ako vlozeny tak preskakuj
	for (size_t i = 0; i < zoznam->size(); i++) {
		SPZVlozeneho = (*zoznam)[i]->getSPZ();
		SPZVkladaneho = vkladane->getSPZ();

		//SPZ musi byt unikatny 		
		if (SPZVlozeneho ==SPZVkladaneho) {
			cout << "Chyba: zvolená ŠPZ už existuje!" << endl;
			cout << "Zadané vozidlo nebolo vložené" << endl;
			return;
		}

		if (
			vkladane->getDatumZaradenia()
	>
			(*zoznam)[i]->getDatumZaradenia()
			) {
			continue;
		}
		else {
			zoznam->insert(vkladane, i);
			return;
		}
	}

	zoznam->add(vkladane);

	cout << "Vozidlo bolo úspešne pridané." << endl;
}

DatabazaVozidiel::DatabazaVozidiel()
	: zoznam(new structures::LinkedList<Vozidlo*>) {}

DatabazaVozidiel::~DatabazaVozidiel() noexcept {
	for (auto pVozidlo : *zoznam) {
		delete pVozidlo;
		pVozidlo = nullptr;
	}

	delete zoznam;
	zoznam = nullptr;
}

void DatabazaVozidiel::pridaj() {
	string SPZ;
	cout << "Vložte SPZ: ";
	cin >> SPZ;

	char typ;
	cout << "Vložte typ ('h' pre hranolceky, 'l' pre lupienky): ";
	cin >> typ;

	if (typ == 'h') {
		pridajChronologicky(new VozidloHranolceky(SPZ));
	}
	else
		if (typ == 'l') {
			pridajChronologicky(new VozidloLupienky(SPZ));
		}
		else {
			cout << "Typ vozidla nebol platný, vozidlo nebolo pridané!" << endl;
		}
}

void DatabazaVozidiel::vypis() const {

	if (zoznam->isEmpty()) {
		cout << "Databáza vozidiel je prázdna." << endl;
		return;
	}

	for (auto vozidlo : *zoznam) {
		time_t datumZaradenia = vozidlo->getDatumZaradenia();

		cout << "Vozidlo:" << endl
			<< "SPZ: '" << vozidlo->getSPZ() << "'" << endl
			<< "den zaradenia: " << asctime(localtime(&datumZaradenia)) << endl
			<< "celkove prevadzkove naklady: " << vozidlo->getPrevadzkoveNaklady() << endl
			<< "aktualne mnozstvo naplneneho tovaru: " << vozidlo->getMnozstvo() << " z maximalnej nostosti: " << vozidlo->getNosnost() << endl;

		switch (vozidlo->getDruh()) {
		case TypTovaru::hranolceky:
			cout << "typ vozidla: na hranolceky" << endl;
			break;
		case TypTovaru::lupienky:
			cout << "typ vozidla: na lupienky" << endl;
			break;
		default:
			cout << "Neznámy typ vozidla!";
			return;
		}

		cout << endl;
	}
}

bool DatabazaVozidiel::volnaKapacita(TypTovaru typTovaru, const DatabazaObjednavok & dbObjednavok, time_t datumDorucenia, ulong mnozstvo) const {
	ulong celkovaKapacita = 0;
	ulong zaplneneMiesto = 0;

	for (auto pVozidlo : *zoznam) {
		if (pVozidlo->getDruh() == typTovaru) {
			celkovaKapacita += pVozidlo->getNosnost();
		}
	}
	for (auto pObjednavka : *dbObjednavok.zoznam) {
		if (epochaNaPocetDni(pObjednavka->getDatumDorucenia()) == epochaNaPocetDni(datumDorucenia) && pObjednavka->getTypTovaru() == typTovaru
			&& !pObjednavka->getZamietnuta() && !pObjednavka->getZrusena())
		{
			zaplneneMiesto += pObjednavka->getMnozstvo();
		}
	}
	return (celkovaKapacita >= (zaplneneMiesto + mnozstvo));
}

void DatabazaVozidiel::naplnVozidla(const DatabazaObjednavok & dbObjednavok) {
	for (auto pVozidlo : *zoznam) {
		for (auto pObjednavka : *dbObjednavok.zoznam) {
			if (epochaNaPocetDni(pObjednavka->getDatumDorucenia()) == epochaNaPocetDni(dnesnyDatum))
			{
				if (pVozidlo->naplnVozidlo(pObjednavka)) {
					cout << "Do vozidla s nosnosou: " << pVozidlo->getNosnost() << "pridané množstvo: " << pObjednavka->getMnozstvo() << endl;
					break;
				}
			}
		}
	}
}

void DatabazaVozidiel::odovzdajTovar(const DatabazaZakaznikov& dbZakaznikov) {
	for (auto pVozidlo : *zoznam) {
		Objednavka* o;
		structures::LinkedList<ulong> navstiveneRegiony;

		while (!pVozidlo->objednavky.isEmpty()) {
			o = pVozidlo->objednavky.peek();
			pVozidlo->spracujObjednavku(o);

			// ak sme este nenavstivili tento region, tak ho pridame
			ulong navstivenyRegion = dbZakaznikov.ziskajRegion(o->getZakaznik());
			if (navstiveneRegiony.getIndexOf(navstivenyRegion) == -1) {
				navstiveneRegiony.add(navstivenyRegion);
			}

			pVozidlo->objednavky.pop();
		}

		pVozidlo->updatePrevadzkoveNaklady(navstiveneRegiony.size());
	}

}

ulong DatabazaVozidiel::prevadzkoveNaklady() const
{
	ulong celkoveNaklady = 0;

	for (auto pVozidlo : *zoznam) {
		celkoveNaklady += pVozidlo->getPrevadzkoveNaklady();
	}

	return celkoveNaklady;
}
