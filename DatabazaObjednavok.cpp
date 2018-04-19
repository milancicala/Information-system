#include "DatabazaObjednavok.h"

#include <ctime>
using std::ctime;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

const string ODDELOVAC = "- - - - - - - - - -";
extern time_t dnesnyDatum;

DatabazaObjednavok::DatabazaObjednavok()
	: zoznam(new structures::LinkedList<Objednavka*>) {}

DatabazaObjednavok::DatabazaObjednavok(
	const structures::LinkedList<Objednavka*>*& inyZoznam
) {
	zoznam = new structures::LinkedList<Objednavka*>(*inyZoznam);
}

DatabazaObjednavok::~DatabazaObjednavok() noexcept {

	for (auto prvok : *zoznam) {
		delete prvok;
	}

	delete zoznam;

}

void DatabazaObjednavok::pridajPodlaDataRealizacie(const Objednavka& vkladany) {

	Objednavka* ukVkladany = new Objednavka(
		vkladany.getDatumDorucenia(),
		vkladany.getDatumZaevidovania(),
		vkladany.getTypTovaru(),
		vkladany.getMnozstvo(),
		vkladany.getCena(),
		vkladany.getZakaznik()
	);

	vkladany.getZamietnuta() ? ukVkladany->setZamietnuta() : false;
	vkladany.getZrusena() ? ukVkladany->setZrusena() : false;

	// pokial vkladany datum je neskorsi ako vlozeny tak preskakuj
	for (size_t i = 0; i < zoznam->size(); i++) {
		if (
			vkladany.getDatumDorucenia()
	>
			(*zoznam)[i]->getDatumDorucenia()
			) {
			continue;
		}
		else {
			zoznam->insert(ukVkladany, i);
			return;
		}
	}

	zoznam->add(ukVkladany);
}

bool DatabazaObjednavok::pridaj(const DatabazaVozidiel& dbVozidiel) {
	string zakaznik;
	cout << "Vložte obchodný názov zákazníka: ";
	cin >> zakaznik;
	cout << endl;

	char vlozenyTyp;
	cout << "Vložte typ tovaru ('h' pre hranolceky, 'l' pre lupienky): ";
	cin >> vlozenyTyp;

	if (!platnyTypTovaru(vlozenyTyp)) {
		while (!platnyTypTovaru(vlozenyTyp)) {
			cout << endl;
			cout << "Vložili ste nesprávne písmeno pre typ tovaru." << endl;
			cout << "Vložte typ tovaru ('h' pre hranolceky, 'l' pre lupienky): ";
			cin >> vlozenyTyp;
			cout << endl;
		}
	}

	TypTovaru typ;
	switch (vlozenyTyp) {
	case 'h':
		typ = TypTovaru::hranolceky;
		break;
	case 'l':
		typ = TypTovaru::lupienky;
		break;
	default:
		cout << "Typ tovaru nebol platný, objednávka nebola pridaná!" << endl;
		return false;
	}

	ulong mnozstvo;
	cout << "Vlozte množstvo objednávaného tovaru: ";
	cin >> mnozstvo;

	ulong cena;
	cout << "Vlozte predajnú cenu: ";
	cin >> cena;

	ulong pocetDniKuDoruceniu;
	cout << "O koåko dní má by objednávka doruèená? ";
	cin >> pocetDniKuDoruceniu;

	if (pocetDniKuDoruceniu < 7) {
		cout << endl;
		cout << "Objednávka nemôže by doruèená!" << endl;
		cout << "Poèet dní do doruèenia musí by aspoñ 7!" << endl;
		cout << endl;

		return false;
	}

	time_t datumDorucenia;
	datumDorucenia = dnesnyDatum + pocetDniNaEpochu(pocetDniKuDoruceniu);

	Objednavka o(
		datumDorucenia,
		dnesnyDatum,
		typ,
		mnozstvo,
		cena,
		zakaznik
	);

	bool zamietnuta = false;
	if (!dbVozidiel.volnaKapacita(typ, *this, datumDorucenia, mnozstvo)) {
		cout << "Objednávka oznaèená ako zamietnutá!" << endl;
		cout << "Prekroèená celková kapacita vozidiel pre rozvoz daného tovaru." << endl;
		zamietnuta = true;
		o.setZamietnuta();
	}

	pridajPodlaDataRealizacie(o);

	zamietnuta == false ? cout << "Objednávka bola úspešne zadaná do systému." << endl : cout << "Objednávka bola úspešne zadaná do systému a oznaèená ako zamietnutá." << endl;
	cout << endl;

	return true;
}

void DatabazaObjednavok::vypis(const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) const {

	if (zoznam->isEmpty()) {
		cout << "Databáza objednávok je prázdna." << endl;
		cout << endl;

		return;
	}

	for (auto pObjednavka : *zoznam) {
		pObjednavka->vypis(dbZakaznikov, dbNakupov);
	}
}

//zrusenie objednavky na dalsi den, ak je nedostacujuci pocet polotovarov a rezervovanie polotovarov na sklade pre danu objednavku
void DatabazaObjednavok::zrusObjednavkyPreNedostatokPolotovarov(DatabazaNakupov& dbNakupov) {
	ulong pocetZrusenych = 0;
	for (auto pObjednavka : *zoznam) {
		ulong dnesnyDen = epochaNaPocetDni(dnesnyDatum);
		ulong denDorucenia = epochaNaPocetDni(pObjednavka->getDatumDorucenia());

		if (denDorucenia == dnesnyDen + 1) {
			bool existuje = dbNakupov.existujeDostatokPolotovarov(*pObjednavka);
			if (!existuje)
			{
				cout << "Prieha zrušenie objednávky" << endl;
				pocetZrusenych++;
				pObjednavka->setZrusena();
			}
			else 
			{
				dbNakupov.odpocitajMnozstvoTovaru(*pObjednavka);
			}
		}
	}
	cout << "Poèet zrušených objednávok na zajtra: " << pocetZrusenych << endl << endl;
}

void DatabazaObjednavok::vypisNaZrealizovanieZajtra(const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) const {
	for (auto pObjednavka : *zoznam) {
		ulong dnesnyDen = epochaNaPocetDni(dnesnyDatum);
		ulong denDorucenia = epochaNaPocetDni(pObjednavka->getDatumDorucenia());

		if (denDorucenia == dnesnyDen + 1 && !pObjednavka->getZamietnuta() && !pObjednavka->getZrusena()) {
			pObjednavka->vypis(dbZakaznikov, dbNakupov);
		}
	}
}

ulong DatabazaObjednavok::zisk() const {

	ulong celkoveMnozstvo = 0;
	ulong celkovaCena = 0;

	for (auto pObjednavka : *zoznam) {
		celkoveMnozstvo += pObjednavka->getMnozstvo();
		celkovaCena += pObjednavka->getCena();
	}

	return celkoveMnozstvo * celkovaCena;
}

ulong DatabazaObjednavok::kDodaniuDoTyzdna(TypTovaru typTovaru) const {
	ulong mnozstvoKdodaniu = 0;

	for (auto objednavka : *zoznam) {
		if (typTovaru == objednavka->getTypTovaru()) {
			if (
				objednavka->getDatumDorucenia() >= dnesnyDatum
				&&
				objednavka->getDatumDorucenia() <= dnesnyDatum + pocetSekundVdni * 7
				&&
				!objednavka->getZamietnuta()
				&&
				!objednavka->getZrusena()
				) {
				if (objednavka->getDatumDorucenia() > dnesnyDatum + pocetSekundVdni * 1) //objednavky na zajtrajsi den budu zrusene
				{
					mnozstvoKdodaniu += objednavka->getMnozstvo();
				}
				else 
				{
					objednavka->setZrusena();
					cout << "Objednávka pre zákazníka: " << objednavka->getZakaznik() << " ,ktorá mala by doruèené zajtra, bude zrušená z dôvodu nedostatku polotvarov." << endl;
				}
				
			}
		}
	}

	return mnozstvoKdodaniu;
}

void DatabazaObjednavok::vypisZrealizovane(time_t od, time_t po, const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) const {
	for (auto pObjednavka : *zoznam) {
		if (pObjednavka->getDatumDorucenia() >= od && pObjednavka->getDatumDorucenia() <= po) {
			if (!pObjednavka->getZamietnuta() && !pObjednavka->getZrusena()) {
				pObjednavka->vypis(dbZakaznikov, dbNakupov);
				cout << "Celkové tržby zisk:" << pObjednavka->getCena() * pObjednavka->getMnozstvo() << endl;
			}
		}
	}
}

void DatabazaObjednavok::vypisNezrealizovane(time_t od, time_t po, const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) const {
	for (auto pObjednavka : *zoznam) {
		if (pObjednavka->getDatumDorucenia() >= od && pObjednavka->getDatumDorucenia() <= po) {
			if (pObjednavka->getZamietnuta() || pObjednavka->getZrusena()) {
				pObjednavka->vypis(dbZakaznikov, dbNakupov);
				cout << "Ušlý zisk:" << pObjednavka->getCena() * pObjednavka->getMnozstvo() << endl;
			}
		}
	}
}

void DatabazaObjednavok::vypisPocetHmotnostTrzbyZrusenychAzamietnutych(ulong region, time_t od, time_t po, const DatabazaZakaznikov& dbZakaznikov) const {
	for (auto pZakaznik : *dbZakaznikov.zoznam) {
		if (region == pZakaznik->getRegion()) {
			ulong celkovyPrijem = 0;

			ulong pocetZrusenych = 0;
			ulong hmotnostZrusenych = 0;
			ulong cenaZrusenych = 0;

			ulong pocetZamietnutych = 0;
			ulong hmotnostZamietnutych = 0;
			ulong cenaZamietnutych = 0;

			for (auto pObjednavka : *zoznam) {
				if (epochaNaPocetDni(pObjednavka->getDatumDorucenia()) >= od && epochaNaPocetDni(pObjednavka->getDatumDorucenia()) <= po && pZakaznik->getObchodnyNazov() == pObjednavka->getZakaznik()) {
					if (pObjednavka->getZrealizovana()) {
						celkovyPrijem += pObjednavka->getMnozstvo() * pObjednavka->getCena();
					}

					if (pObjednavka->getZrusena()) {
						++pocetZrusenych;
						hmotnostZrusenych += pObjednavka->getMnozstvo();
						cenaZrusenych += pObjednavka->getCena();
					}

					if (pObjednavka->getZamietnuta()) {
						++pocetZamietnutych;
						hmotnostZamietnutych += pObjednavka->getMnozstvo();
						cenaZamietnutych += pObjednavka->getCena();
					}
				}
			}

			cout << "zákazník: " << pZakaznik->getObchodnyNazov() << endl;

			cout << "celkový príjem: " << celkovyPrijem << endl;

			cout << "zrušené objednávky:" << endl;
			cout << "poèet: " << pocetZrusenych << endl;
			cout << "hmotnos: " << hmotnostZrusenych << endl;
			cout << "tržby: " << hmotnostZrusenych * cenaZrusenych << endl;

			cout << "zamietnuté objednávky:" << endl;
			cout << "poèet: " << pocetZamietnutych << endl;
			cout << "hmotnos: " << hmotnostZamietnutych << endl;
			cout << "tržby: " << hmotnostZamietnutych * cenaZamietnutych << endl;
		}
	}
}